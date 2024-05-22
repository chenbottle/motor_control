#include "config_main.h"

void Motor_Send_Msg::poi_control(int slave, int id,float poi_,float vel_,float cur_max_){
    this->slave = slave;
    m_id = id;
    pvt = 0;
    poi = poi_;
    vel = vel_;
    tor = 0;
    cur_max = cur_max_;
    _ack = 1; 
    cur = 0; 
}
void Motor_Send_Msg::vel_control(int slave, int id,float vel_,float cur_max_){
    this->slave = slave;
    m_id = id;
    pvt = 1;
    poi = 0;
    vel = vel_;
    tor = 0;
    cur_max = cur_max_;
    _ack = 1; 
    cur = 0; 
}
void Motor_Send_Msg::tor_control(int slave, int id,float tor_){
    this->slave = slave;
    m_id = id;
    pvt = 2;
    poi = 0;
    vel = 0;
    tor = tor_;
    cur_max = 500;
    _ack = 1; 
    cur = 0; 
}

void command_set_toc(Motor_Send_Msg *msm,motor_command *m_command){
    int num = sizeof(msm);
    for(int i = 0; i < num; i++){
        m_command[i].slave= msm[i].slave;
        m_command[i].m_id = msm[i].m_id;
        m_command[i].pvt = msm[i].pvt;
        m_command[i].poi = msm[i].poi;
        m_command[i].vel = msm[i].vel;
        m_command[i].tor = msm[i].tor;
        m_command[i].cur_max = msm[i].cur_max;
        m_command[i]._ack = msm[i]._ack;
        m_command[i].cur = msm[i].cur;
    }
}