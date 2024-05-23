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
    msm_ = new Motor_Send_Msg*[row];
    M_com = new motor_command*[row];
    for(int i = 0; i < row; i++){
        *(msm_ + 1) = new Motor_Send_Msg[col];
        *(M_com + 1) = new motor_command[col];
    }
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            msm_[i][j].slave = i;
        }
    }
}

m_control::~m_control(){
   delete []msm_;
   delete []M_com;
}

void m_control::get_data(EtherCAT_Msg *_msg)
{   
    memcpy(_ReceiveData,_msg,sizeof(_ReceiveData));
    for (int slave = 0; slave < 6;slave++){
        for (int i = 0;i < 6;i++){              
            if(_ReceiveData[slave].motor[i].id < 7){                 
                motor_id = _ReceiveData[slave].motor[i].id; 
                // printf("id:%d\t",_ReceiveData[slave].motor[i].id);
                m_body[motor_id].motor_id = motor_id;                
                m_body[motor_id].angle_actual_int = _ReceiveData[slave].motor[i].data[1] << 8 | _ReceiveData[slave].motor[i].data[2];
                m_body[motor_id].speed_actual_int = _ReceiveData[slave].motor[i].data[3] << 4 | (_ReceiveData[slave].motor[i].data[4] & 0xF0) >> 4;
                m_body[motor_id].current_actual_int = (_ReceiveData[slave].motor[i].data[4] & 0x0F) << 8 | _ReceiveData[slave].motor[i].data[5];

                m_body[motor_id].angle_actual_rad = uint_to_float(m_body[motor_id].angle_actual_int, POS_MIN, POS_MAX, 16);
                m_body[motor_id].speed_actual_rad = uint_to_float(m_body[motor_id].speed_actual_int, SPD_MIN, SPD_MAX, 12);
                m_body[motor_id].current_actual_float = uint_to_float(m_body[motor_id].current_actual_int, I_MIN_10020, I_MAX_10020, 12);
                m_body[motor_id].temperature = (_ReceiveData->motor[i].data[6] - 50) / 2;                     
            }             
            // for(int j = 0;j < 8; j++){
            //     // cout << "data[" << i << "]:" << hex << _ReceiveData[slave].motor[i].data[j] << "\t";
            //     printf("motor[%d]data[%d]:%x\t",i,j,_ReceiveData[slave].motor[i].data[j]);
            // }
            // cout << endl;
        }
    }     
}

motor_command** m_control::move(){  
    //力矩控制
    // msm_[0].cur = (int16_t)(0*819.2);
    // msm_[1].cur = (int16_t)(0*819.2); 
    // msm_[2].tor_control(2,0*100);
    // msm_[3].tor_control(3,0*100); 
    // msm_[4].tor_control(4,0*100);
    // msm_[5].tor_control(5,0*100); 

    //位置控制
    // msm_[1].poi_control(1,0,50,500); 
    // msm_[2].poi_control(2,100,50,500);    
    // msm_[3].poi_control(3,0,50,500);
    // msm_[4].poi_control(4,0,50,500); 
    // msm_[5].poi_control(5,0,50,500);

    //速度控制
    msm_[0][0].vel_control(1,-5,500);
    msm_[0][1].vel_control(1,10,500);
    msm_[1][1].vel_control(2,5,500);
    // msm_[4].vel_control(4,50,500);
    // msm_[5].vel_control(5,50,500);
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            command_set_toc(msm_[i][j],M_com[i][j]);
        }
    }    
    return M_com;
}