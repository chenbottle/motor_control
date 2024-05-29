#include "MotorControl.h"

#define X_MIN -0.1482f
#define X_MAX 0.4f
#define Y_MIN -0.5f
#define Y_MAX -0.099f
#define POS_MAX 12.5f
#define POS_MIN -12.5f
#define SPD_MIN -18.0f
#define SPD_MAX 18.0f
#define T_MIN -30.0f
#define T_MAX 30.0f
#define I_MIN_10020 -70.0f
#define I_MAX_10020 70.0f
#define I_MIN_8120 -40.0f
#define I_MAX_8120 40.0f
#define CUR_MAX 14.0f

#define CENTER_DIS_MIN 0.23f
#define CENTER_DIS_MAX 0.49f
#define HIP_DIFF_ANGLE -0.0f
#define KNEE_DIFF_ANGLE 0.0f

OD_Motor_Msg rv_motor_msg[6];

// float uint_to_float(int x_int, float x_min, float x_max, int bits) {
//     /// converts unsigned int to float, given range and number of bits ///
//     float span = x_max - x_min;
//     float offset = x_min;
//     return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
// }

m_control::m_control(){
    // row = get_ec_slavecount();
    row = ec_slavecount;
    msm_ = new Motor_Send_Msg[row];
    M_com = new motor_command[row];
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 6; j++){
            m_body[i][j].angle_actual_float = 0;
            m_body[i][j].motor_id = 0;
            m_body[i][j].speed_actual_float = 0;
            m_body[i][j].current_actual_float = 0;
            m_body[i][j].temperature = 0;
        }
    }
    // m_body = new OD_Motor_Msg*[row];
    // for(int i = 0; i < row; i++){
    //     *(m_body + 1) = new OD_Motor_Msg[col];
    // }
    // for(int i = 0; i < row; i++){
    //     msm_[i].M_Slave.slave = i;
    // }
    
}

m_control::~m_control(){
//    delete []msm_;
//    delete []M_com;
}

void m_control::get_data(EtherCAT_Msg *_msg){
    row = ec_slavecount;   
    
    // printf("------------------------------------\n");
    // for(int i = 0; i < ec_slavecount; i++){
    //     printf("RX_slzve:%d\t",i);
    //     for(int j = 0; j < 6; j++){
    //         printf("motor_id[%d].data[0]:%x\t",_msg[i].motor[j].id,_msg[i].motor[j].data[0]);
    //         printf("data[1]:%x\t",_msg[i].motor[j].data[1]);
    //         printf("data[2]:%x\t",_msg[i].motor[j].data[2]);
    //         printf("data[3]:%x\t",_msg[i].motor[j].data[3]);
    //         printf("data[4]:%x\t",_msg[i].motor[j].data[4]);
    //         printf("data[5]:%x\t",_msg[i].motor[j].data[5]);
    //         printf("data[6]:%x\t",_msg[i].motor[j].data[6]);
    //         printf("\n"); 
    //     }     
    // }
    // printf("------------------------------------\n");
    // memcpy(_ReceiveData,_msg,sizeof(_ReceiveData));
    for (int slave = 0; slave < row; slave++){
        for (int i = 0;i < col;i++){              
            if(_msg[slave].motor[i].id < 6 && _msg[slave].motor[i].id > 0){  
                // printf("mbody_id:%d\t",m_body[slave][i].motor_id);
                // printf("id:%d\t",_msg[slave].motor[i].id);
                m_body[slave][i].motor_id = _msg[slave].motor[i].id;                
                m_body[slave][i].angle_actual_int = _msg[slave].motor[i].data[1] << 8 | _msg[slave].motor[i].data[2];
                m_body[slave][i].speed_actual_int = _msg[slave].motor[i].data[3] << 4 | (_msg[slave].motor[i].data[4] & 0xF0) >> 4;
                m_body[slave][i].current_actual_int = (_msg[slave].motor[i].data[4] & 0x0F) << 8 | _msg[slave].motor[i].data[5];

                m_body[slave][i].angle_actual_rad = uint_to_float(m_body[slave][i].angle_actual_int, POS_MIN, POS_MAX, 16);
                m_body[slave][i].speed_actual_rad = uint_to_float(m_body[slave][i].speed_actual_int, SPD_MIN, SPD_MAX, 12);
                m_body[slave][i].current_actual_float = uint_to_float(m_body[slave][i].current_actual_int, I_MIN_8120, I_MAX_8120, 12);
                m_body[slave][i].temperature = (_msg[slave].motor[i].data[6] - 50) / 2;                     
            }             
            // for(int j = 0;j < 8; j++){
            //     // cout << "data[" << i << "]:" << hex << _ReceiveData[slave].motor[i].data[j] << "\t";
            //     printf("motor[%d]data[%d]:%x\t",i,j,_ReceiveData[slave].motor[i].data[j]);
            // }
            // cout << endl;
        }
    } 
    motor_data_cout();  
    IMU_data_cout();  
}

void m_control::motor_data_cout(){
    for(int i = 0; i < row; i++){
        printf("slave[%d]:\n", i);
        for(int j = 0; j < col; j++){
            printf("id:%d\t", m_body[i][j].motor_id);
            printf("poi:%f\t", m_body[i][j].angle_actual_rad);
            printf("vel:%f\t", m_body[i][j].speed_actual_rad);
            printf("current:%f\t", m_body[i][j].current_actual_float);
            printf("temperature:%d\n", m_body[i][j].temperature);
        }
    }
}

void m_control::IMU_data_cout(){
    // imu->write();
}

motor_command* m_control::move(IMUState* _imu){  
    this->imu = _imu;
    row = ec_slavecount;
    //力位混控
    // msm_[0].tor_poi_control(2, 0, 0, 0, 0, 0.05);
    // msm_[1].tor_control(2,0*100);

    //力矩控制
    // msm_[0].tor_control(2,1*100);
    // msm_[1].tor_control(2,0*100);
    // msm_[2].tor_control(2,0*100);
    // msm_[3].tor_control(3,0*100); 
    // msm_[4].tor_control(4,0*100);
    // msm_[5].tor_control(5,0*100); 

    //位置控制
    // msm_[0].poi_control(2,0,0.5*100,500); 
    // msm_[2].poi_control(2,100,50,500);    
    // msm_[3].poi_control(3,0,50,500);
    // msm_[4].poi_control(4,0,50,500); 
    // msm_[5].poi_control(5,0,50,500);

    //速度控制
    msm_[0].vel_control(2,-10,500);
    // msm_[1].vel_control(1,0.5*10,500);
    // msm_[0][1].vel_control(1,10,500);
    
    // msm_[4].vel_control(4,50,500);
    // msm_[5].vel_control(5,50,500);
    // cout << "row:" << row << endl;
    for(int i = 0; i < row; i++){
        M_com[i].slave = i;
        command_set_toc(msm_[i],M_com[i]);
    }    
    return M_com;
}