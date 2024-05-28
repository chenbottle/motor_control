#include "config_main.h"

void Motor_Send_Msg::poi_control(int id,float poi_,float vel_,float cur_max_){
    M_Slave.Motor_Msg[id].m_id = id;
    M_Slave.Motor_Msg[id].pvt = 0;
    M_Slave.Motor_Msg[id].poi = poi_;
    M_Slave.Motor_Msg[id].vel = vel_;
    M_Slave.Motor_Msg[id].tor = 0;
    M_Slave.Motor_Msg[id].cur_max = cur_max_;
    M_Slave.Motor_Msg[id]._ack = 1; 
    M_Slave.Motor_Msg[id].cur = 0; 
}
void Motor_Send_Msg::vel_control(int id,float vel_,float cur_max_){
    M_Slave.Motor_Msg[id].m_id = id;
    M_Slave.Motor_Msg[id].pvt = 1;
    M_Slave.Motor_Msg[id].poi = 0;
    M_Slave.Motor_Msg[id].vel = vel_;
    M_Slave.Motor_Msg[id].tor = 0;
    M_Slave.Motor_Msg[id].cur_max = cur_max_;
    M_Slave.Motor_Msg[id]._ack = 1; 
    M_Slave.Motor_Msg[id].cur = 0; 
}
void Motor_Send_Msg::tor_control(int id,float tor_){
    M_Slave.Motor_Msg[id].m_id = id;
    M_Slave.Motor_Msg[id].pvt = 2;
    M_Slave.Motor_Msg[id].poi = 0;
    M_Slave.Motor_Msg[id].vel = 0;
    M_Slave.Motor_Msg[id].tor = tor_;
    M_Slave.Motor_Msg[id].cur_max = 500;
    M_Slave.Motor_Msg[id]._ack = 1; 
    M_Slave.Motor_Msg[id].cur = 0; 
}

void command_set_toc(Motor_Send_Msg msm,motor_command &m_command){
    for(int i = 0; i < 6; i++){
        m_command.slave= msm.M_Slave.slave;
        m_command.Motor_Msg[i].m_id = msm.M_Slave.Motor_Msg[i].m_id;
        m_command.Motor_Msg[i].pvt = msm.M_Slave.Motor_Msg[i].pvt;
        m_command.Motor_Msg[i].poi = msm.M_Slave.Motor_Msg[i].poi;
        m_command.Motor_Msg[i].vel = msm.M_Slave.Motor_Msg[i].vel;
        m_command.Motor_Msg[i].tor = msm.M_Slave.Motor_Msg[i].tor;
        m_command.Motor_Msg[i].cur_max = msm.M_Slave.Motor_Msg[i].cur_max;
        m_command.Motor_Msg[i]._ack = msm.M_Slave.Motor_Msg[i]._ack;
        m_command.Motor_Msg[i].cur = msm.M_Slave.Motor_Msg[i].cur;
    } 
       
}