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

}

Interface::~Interface() {

}

void Interface::SendDriver(){               
    // memcpy(msm_motor , twl_robot.move() , sizeof(msm_motor));             
    EtherCAT_Run(_m_control.move());   
}

void Interface::RecvFromDriver()
{
    // rece_driver_mutex.lock();
    // _m_control.get_data(EtherCAT_Data_Get());
    // rece_driver_mutex.unlock();
}

// 将4个字节的char转为float
float ConvertByte2Float(unsigned char *pByte){
    float floatVariable;
    unsigned char i;
    void *pf;
    pf = &floatVariable;

    for (i = 0; i < 4; i++) {
        *((unsigned char *) pf + i) = *(pByte + i);
    }

    return floatVariable;
}
