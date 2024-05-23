#include "config_main.h"

void Motor_Send_Msg::poi_control(int id,float poi_,float vel_,float cur_max_){
    m_id = id;
    pvt = 0;
    poi = poi_;
    vel = vel_;
    tor = 0;
    cur_max = cur_max_;
    _ack = 1; 
    cur = 0; 
}
void Motor_Send_Msg::vel_control(int id,float vel_,float cur_max_){
    m_id = id;
    pvt = 1;
    poi = 0;
    vel = vel_;
    tor = 0;
    cur_max = cur_max_;
    _ack = 1; 
    cur = 0; 
}
void Motor_Send_Msg::tor_control(int id,float tor_){
    m_id = id;
    pvt = 2;
    poi = 0;
    vel = 0;
    tor = tor_;
    cur_max = 500;
    _ack = 1; 
    cur = 0; 
}

void command_set_toc(Motor_Send_Msg msm,motor_command &m_command){
    m_command.slave= msm.slave;
    m_command.m_id = msm.m_id;
    m_command.pvt = msm.pvt;
    m_command.poi = msm.poi;
    m_command.vel = msm.vel;
    m_command.tor = msm.tor;
    m_command.cur_max = msm.cur_max;
    m_command._ack = msm._ack;
    m_command.cur = msm.cur;
}