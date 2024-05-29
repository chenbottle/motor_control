#ifndef CONFIGMAIN_H
#define CONFIGMAIN_H

#ifdef __cplusplus

#include "config.h"
#include <cstdio>
//发送给电机的指令
class Motor_Send_Msg
{
public:
    Motor_Send_Msg(){
        for(int i = 0; i < 6; i++){
            M_Slave.slave = 0;
            M_Slave.Motor_Msg[i].m_id = 0;
            M_Slave.Motor_Msg[i].pvt = 4;
            M_Slave.Motor_Msg[i].KP = 0;
            M_Slave.Motor_Msg[i].KD = 0;
            M_Slave.Motor_Msg[i].poi = 0;
            M_Slave.Motor_Msg[i].vel = 0;
            M_Slave.Motor_Msg[i].tor = 0;
            M_Slave.Motor_Msg[i].cur_max = 500;
            M_Slave.Motor_Msg[i]._ack = 1; 
            M_Slave.Motor_Msg[i].cur = 0; 
        }      
    }
    void poi_control(int id,float poi_,float vel_,float cur_max_);
    void vel_control(int id,float vel_,float cur_max_);
    void tor_control(int id,float tor_);
    void tor_poi_control(int id, float KP_, float KD_, float poi_, float vel_, float tor_);
    motor_command M_Slave;   
};

void command_set_toc(Motor_Send_Msg msm,motor_command &m_command);
#endif
#endif