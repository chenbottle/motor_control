#ifndef CONFIGMAIN_H
#define CONFIGMAIN_H

#ifdef __cplusplus

#include "config.h"

//发送给电机的指令
class Motor_Send_Msg
{
public:
    Motor_Send_Msg()
    {
        slave = 0;
        m_id = 0;
        pvt = 3;
        poi = 0;
        vel = 0;
        tor = 0;
        cur_max = 500;
        _ack = 1; 
        cur = 0;        
    }
    void poi_control(int id,float poi_,float vel_,float cur_max_);
    void vel_control(int id,float vel_,float cur_max_);
    void tor_control(int id,float tor_);
    int slave;
    int m_id;
    int pvt;
    float poi;
    float vel;
    float tor;
    float cur_max ;
    int _ack;  
    int cur;       
};

void command_set_toc(Motor_Send_Msg msm,motor_command &m_command);
#endif
#endif