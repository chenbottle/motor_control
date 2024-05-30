//
// Created by leeqee on 2021/8/23.
//
#include "Interface.h"
#include <iostream>

#define maxVelX 100
#define minVelX -100
#define maxVelY 0.1
#define maxYawRate 0.3

// Motor_Send_Msg msm_motor[6];
// EtherCAT_Msg Rx_Message[SLAVE_NUMBER];
// EtherCAT_Msg Tx_Message[SLAVE_NUMBER];

Interface::Interface() {  
    //imu init
    // imu.initIMU();   
    //lcm init    
    while(!lcm_motor.good() || !lcm_imu.good())
    {
        printf("lcm init wrong!");         
    }  
}

Interface::~Interface() {

}

void Interface::SendDriver(){               
    // memcpy(msm_motor , twl_robot.move() , sizeof(msm_motor));             
    EtherCAT_Run(_m_control.move(&imu.imuData));   
}

void Interface::RecvFromDriver()
{
    // rece_driver_mutex.lock();
    _m_control.get_data(EtherCAT_Data_Get());
    record_to_lcm();
    lcm_motor.publish("motor_data",lcm_motor_data);
    lcm_imu.publish("imu_data",&lcm_imu_data);
    // rece_driver_mutex.unlock();
}

void Interface::getIMUdata(){
    // imu.imuGet();
}

void Interface::record_to_lcm(){
    lcm_imu_data.roll = imu.imuData.roll;
    lcm_imu_data.yaw = imu.imuData.yaw;
    lcm_imu_data.pitch = imu.imuData.pitch;
    for(int i = 0; i < 3; i++){
        lcm_imu_data.acc[i] = imu.imuData.acceleration[i];
        lcm_imu_data.angle_speed[i] = imu.imuData.angularVelo[i];
    }
    _m_control.record_lcm_motor(lcm_motor_data);

}

// // 将4个字节的char转为float
// float ConvertByte2Float(unsigned char *pByte){
//     float floatVariable;
//     unsigned char i;
//     void *pf;
//     pf = &floatVariable;

//     for (i = 0; i < 4; i++) {
//         *((unsigned char *) pf + i) = *(pByte + i);
//     }

//     return floatVariable;
// }
