#include "transmit.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "ethercat.h"
#include "motor_control.h"
#include "sys/time.h"

#define EC_TIMEOUTM

char IOmap[4096];
OSAL_THREAD_HANDLE thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;
uint64_t num;

#define EC_TIMEOUTMON 500

EtherCAT_Msg* EtherCAT_Data_Get();

void EtherCAT_Command_Set(motor_command *m_command);

static void degraded_handler()
{
    printf("[EtherCAT Error] Logging error...\n");
    time_t current_time = time(NULL);
    char *time_str = ctime(&current_time);
    printf("ESTOP. EtherCAT became degraded at %s.\n", time_str);
    printf("[EtherCAT Error] Stopping RT process.\n");
}

static int run_ethercat(const char *ifname)
{
    int i;
    int oloop, iloop, chk;
    needlf = FALSE;
    inOP = FALSE;

    num = 1;

    /* initialise SOEM, bind socket to ifname */
    if (ec_init(ifname))
    {
        printf("[EtherCAT Init] Initialization on device %s succeeded.\n", ifname);
        /* find and auto-config slaves */

        if (ec_config_init(FALSE) > 0)
        {
            printf("[EtherCAT Init] %d slaves found and configured.\n", ec_slavecount);
            if (ec_slavecount < 4)
            {
                printf("[RT EtherCAT] Warning: Expected %d slaves, found %d.\n", 4, ec_slavecount);
            }
            for (int slave_idx = 0; slave_idx < ec_slavecount; slave_idx++)
            ec_slave[slave_idx+1].CoEdetails &= ~ECT_COEDET_SDOCA;
            ec_config_map(&IOmap);
            ec_configdc();

            printf("[EtherCAT Init] Mapped slaves.\n");
            /* wait for all slaves to reach SAFE_OP state */
            ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);

            for (int slave_idx = 0; slave_idx < ec_slavecount; slave_idx++)
            {
                printf("[SLAVE %d]\n", slave_idx);
                printf("  IN  %d bytes, %d bits\n", ec_slave[slave_idx].Ibytes, ec_slave[slave_idx].Ibits);
                printf("  OUT %d bytes, %d bits\n", ec_slave[slave_idx].Obytes, ec_slave[slave_idx].Obits);
                printf("\n");
            }

            oloop = ec_slave[0].Obytes;
            if ((oloop == 0) && (ec_slave[0].Obits > 0))
                oloop = 1;
            if (oloop > 8)
                oloop = 8;
            iloop = ec_slave[0].Ibytes;
            if ((iloop == 0) && (ec_slave[0].Ibits > 0))
                iloop = 1;
            if (iloop > 8)
                iloop = 8;

            printf("[EtherCAT Init] segments : %d : %d %d %d %d\n", ec_group[0].nsegments, ec_group[0].IOsegment[0], ec_group[0].IOsegment[1], ec_group[0].IOsegment[2], ec_group[0].IOsegment[3]);

            printf("[EtherCAT Init] Requesting operational state for all slaves...\n");
            expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
            printf("[EtherCAT Init] Calculated workcounter %d\n", expectedWKC);
            ec_slave[0].state = EC_STATE_OPERATIONAL;
            /* send one valid process data to make outputs in slaves happy*/
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);
            /* request OP state for all slaves */
            ec_writestate(0);
            chk = 40;
            /* wait for all slaves to reach OP state */
            do
            {
                ec_send_processdata();
                ec_receive_processdata(EC_TIMEOUTRET);
                ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
            } while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));

            if (ec_slave[0].state == EC_STATE_OPERATIONAL)
            {
                printf("[EtherCAT Init] Operational state reached for all slaves.\n");
                inOP = TRUE;
                return 1;
            }
            else
            {
                printf("[EtherCAT Error] Not all slaves reached operational state.\n");
                ec_readstate();
                for (i = 1; i <= ec_slavecount; i++)
                {
                    if (ec_slave[i].state != EC_STATE_OPERATIONAL)
                    {
                        printf("[EtherCAT Error] Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                               i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                    }
                }
            }
        }
        else
        {
            printf("[EtherCAT Error] No slaves found!\n");
        }
    }
    else
    {
        printf("[EtherCAT Error] No socket connection on %s - are you running run.sh?\n", ifname);
    }
    return 0;
}

static int err_count = 0;
static int err_iteration_count = 0;
/**@brief EtherCAT errors are measured over this period of loop iterations */
#define K_ETHERCAT_ERR_PERIOD 100

/**@brief Maximum number of etherCAT errors before a fault per period of loop iterations */
#define K_ETHERCAT_ERR_MAX 20

static OSAL_THREAD_FUNC ecatcheck(void *ptr)
{
    (void)ptr;
    int slave = 0;
    while (1)
    {
        // count errors
        if (err_iteration_count > K_ETHERCAT_ERR_PERIOD)
        {
            err_iteration_count = 0;
            err_count = 0;
        }

        if (err_count > K_ETHERCAT_ERR_MAX)
        {
            // possibly shut down
            printf("[EtherCAT Error] EtherCAT connection degraded.\n");
            printf("[Simulink-Linux] Shutting down....\n");
            degraded_handler();
            break;
        }
        err_iteration_count++;

        if (inOP && ((wkc < expectedWKC) || ec_group[currentgroup].docheckstate))
        {
            if (needlf)
            {
                needlf = FALSE;
                printf("\n");
            }
            /* one ore more slaves are not responding */
            ec_group[currentgroup].docheckstate = FALSE;
            ec_readstate();
            for (slave = 1; slave <= ec_slavecount; slave++)
            {
                if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
                {
                    ec_group[currentgroup].docheckstate = TRUE;
                    if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                    {
                        printf("[EtherCAT Error] Slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
                        ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                        ec_writestate(slave);
                        err_count++;
                    }
                    else if (ec_slave[slave].state == EC_STATE_SAFE_OP)
                    {
                        printf("[EtherCAT Error] Slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
                        ec_slave[slave].state = EC_STATE_OPERATIONAL;
                        ec_writestate(slave);
                        err_count++;
                    }
                    else if (ec_slave[slave].state > 0)
                    {
                        if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
                        {
                            ec_slave[slave].islost = FALSE;
                            printf("[EtherCAT Status] Slave %d reconfigured\n", slave);
                        }
                    }
                    else if (!ec_slave[slave].islost)
                    {
                        /* re-check state */
                        ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                        if (!ec_slave[slave].state)
                        {
                            ec_slave[slave].islost = TRUE;
                            printf("[EtherCAT Error] Slave %d lost\n", slave);
                            err_count++;
                        }
                    }
                }
                if (ec_slave[slave].islost)
                {
                    if (!ec_slave[slave].state)
                    {
                        if (ec_recover_slave(slave, EC_TIMEOUTMON))
                        {
                            ec_slave[slave].islost = FALSE;
                            printf("[EtherCAT Status] Slave %d recovered\n", slave);
                        }
                    }
                    else
                    {
                        ec_slave[slave].islost = FALSE;
                        printf("[EtherCAT Status] Slave %d found\n", slave);
                    }
                }
            }
            if (!ec_group[currentgroup].docheckstate)
                printf("[EtherCAT Status] All slaves resumed OPERATIONAL.\n");
        }
        osal_usleep(50000);
    }
}

void EtherCAT_Init(char *ifname)
{
    int i;
    int rc;
    printf("[EtherCAT] Initializing EtherCAT\n");
    osal_thread_create((void *)&thread1, 128000, (void *)&ecatcheck, (void *)&ctime);
    for (i = 1; i < 10; i++)
    {
        printf("[EtherCAT] Attempting to start EtherCAT, try %d of 10.\n", i);
        rc = run_ethercat(ifname);
        if (rc)
            break;
        osal_usleep(1000000);
    }
    if (rc)
        printf("[EtherCAT] EtherCAT successfully initialized on attempt %d \n", i);
    else
    {
        printf("[EtherCAT Error] Failed to initialize EtherCAT after 100 tries. \n");
    }
}

void EtherCAT_Transmit(EtherCAT_Msg *MasterCommand)
{
    for (int i = 0; i < ec_slavecount; i++)
    {
        memcpy((void *)(ec_slave[0].outputs + i * sizeof(EtherCAT_Msg)), (void *)&(MasterCommand[i]), sizeof(EtherCAT_Msg));
    }
    ec_send_processdata();
}

static int wkc_err_count = 0;
static int wkc_err_iteration_count = 0;

//数组大小根据从站数量确定
EtherCAT_Msg Rx_Message[4];
EtherCAT_Msg Tx_Message[4];
/**
 * @description:
 * @return {*}
 */
void EtherCAT_Run(motor_command *m_command)
{
    if (wkc_err_iteration_count > K_ETHERCAT_ERR_PERIOD)
    {
        wkc_err_count = 0;
        wkc_err_iteration_count = 0;
    }
    if (wkc_err_count > K_ETHERCAT_ERR_MAX)
    {
        printf("[EtherCAT Error] Error count too high!\n");
        degraded_handler();
    }
    // send
    EtherCAT_Command_Set(m_command);
    // printf("------------------------------------\n");
    // for(int i = 0; i < ec_slavecount; i++){
    //     printf("TX_slzve:%d\t",i);
    //     for(int j = 0; j < 6; j++){
    //         printf("motor_id[%d].data[0]:%d\t",Tx_Message[i].motor[j].id,Tx_Message[i].motor[j].data[0]);
    //     } 
    //     printf("\n");       
    // }
    // printf("------------------------------------\n");
    ec_send_processdata();
    // receive
    wkc = ec_receive_processdata(EC_TIMEOUTRET);
    // EtherCAT_Data_Get();
    //  check for dropped packet
    if (wkc < expectedWKC)
    {
        printf("\x1b[31m[EtherCAT Error] Dropped packet (Bad WKC!)\x1b[0m\n");
        wkc_err_count++;
    }
    else
    {
        needlf = TRUE;
    }
    wkc_err_iteration_count++;
}

/**
 * @description: slave data get
 * @return {*}
 * @author: Kx Zhang
 */
EtherCAT_Msg *EtherCAT_Data_Get()
{
    for (int slave = 0; slave < ec_slavecount; ++slave)
    {
        EtherCAT_Msg *slave_src = (EtherCAT_Msg *)(ec_slave[slave + 1].inputs);
        if (slave_src)
            Rx_Message[slave] = *(EtherCAT_Msg *)(ec_slave[slave + 1].inputs);
        // RV_can_data_repack(&Rx_Message[slave], comm_ack, slave);
    }
    return Rx_Message;
}

/**
 * @description: slave command set
 * @return {*}
 * @author: Kx Zhang
 */
void EtherCAT_Command_Set(motor_command *m_command)
{
    int col = 6;
    // for (int i = 0;i < num;i++){
    //     // printf("slave:%d\tid:%d\tpoi:%f\tvel:%f\t",m_command[i].slave,m_command[i].m_id,m_command[i].poi,m_command[i].vel);
    // }
    for(int i = 0; i < ec_slavecount; i++){
        for(int j = 0; j < col; j++){
            switch(m_command[i].Motor_Msg[j].pvt)
            {
                case 0:
                    set_motor_position(&Tx_Message[m_command[i].slave], m_command[i].Motor_Msg[j].m_id, 
                    m_command[i].Motor_Msg[j].m_id, m_command[i].Motor_Msg[j].poi, m_command[i].Motor_Msg[j].vel, 
                    m_command[i].Motor_Msg[j].cur_max, m_command[i].Motor_Msg[j]._ack);
                    // printf("id:%d\tpoi:%f\n",m_command[i].m_id,m_command[i].poi);
                    break;
                case 1:
                    set_motor_speed(&Tx_Message[m_command[i].slave], m_command[i].Motor_Msg[j].m_id, 
                    m_command[i].Motor_Msg[j].m_id, m_command[i].Motor_Msg[j].vel, m_command[i].Motor_Msg[j].cur_max,
                    m_command[i].Motor_Msg[j]._ack);
                    // printf("id:%d\tspeed:%f\n",m_command[i].Motor_Msg[j].m_id,m_command[i].Motor_Msg[j].vel);
                    // printf("slave:%d\t",m_command[i].slave);
                    break;
                case 2:
                    set_motor_cur_tor(&Tx_Message[m_command[i].slave], m_command[i].Motor_Msg[j].m_id, 
                    m_command[i].Motor_Msg[j].m_id, m_command[i].Motor_Msg[j].tor, 1, m_command[i].Motor_Msg[j]._ack);
                    break;
                case 3:
                    send_motor_ctrl_cmd(&Tx_Message[m_command[i].slave], m_command[i].Motor_Msg[j].m_id, 
                    m_command[i].Motor_Msg[j].m_id, m_command[i].Motor_Msg[j].KP, m_command[i].Motor_Msg[j].KD, 
                    m_command[i].Motor_Msg[j].poi, m_command[i].Motor_Msg[j].vel, m_command[i].Motor_Msg[j].tor);
                    break;
                default:
                    break;
            }
        }
    }
    for (int slave = 0; slave < ec_slavecount; ++slave)
    {

        // for (int i = 1; i < 6; i++)
        // {
            //这里设置控制电机指令
            // MotorIDReading(&Tx_Message[slave]);
            // MotorCommModeReading(&Tx_Message[slave], 1);
            // MotorSetting(&Tx_Message[slave], 1, 3);
            // MotorIDReset(&Tx_Message[slave]);
            // MotorIDSetting(&Tx_Message[slave], 2, 1);
            // send_motor_ctrl_cmd(&Tx_Message[slave], 1,1, 0, 0, 0, 0, 0);
                            
            // send_motor_ctrl_cmd(&Tx_Message[slave], 2,2, 0, 0, 0, 0, 0.05);

            // send_motor_ctrl_cmd(&Tx_Message[slave], 4,3, 0, 0, 0, 0, 0);

            // send_motor_ctrl_cmd(&Tx_Message[slave], 5,4, 0, 0, 0, 0, 0);

            // send_motor_ctrl_cmd(&Tx_Message[slave], 6,5, 0, 0, 0, 0, 0);

            // set_motor_position(&Tx_Message[slave], 1,2, 360, 50, 100, 2);
            set_motor_speed(&Tx_Message[0], 1,2, 5, 500, 2);
            // set_motor_speed(&Tx_Message[1], 2,2, 5, 500, 2);
            // set_motor_cur_tor(&Tx_Message[slave], 1,2, 0.5*100, 1, 1);
            // get_motor_parameter(&Tx_Message[slave],1, 2, 2);

            EtherCAT_Msg *slave_dest = (EtherCAT_Msg *)(ec_slave[slave + 1].outputs);
            if (slave_dest)
                *(EtherCAT_Msg *)(ec_slave[slave + 1].outputs) = Tx_Message[slave];
    }
}
