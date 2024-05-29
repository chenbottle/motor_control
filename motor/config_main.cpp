#include "config_main.h"

//id,poi,vel,cur_max
void Motor_Send_Msg::poi_control(int id,float poi_,float vel_,float cur_max_){
    int num_id =  id;
    M_Slave.Motor_Msg[num_id].m_id = id;
    M_Slave.Motor_Msg[num_id].pvt = 0;
    M_Slave.Motor_Msg[num_id].poi = poi_;
    M_Slave.Motor_Msg[num_id].vel = vel_;
    M_Slave.Motor_Msg[num_id].tor = 0;
    M_Slave.Motor_Msg[num_id].cur_max = cur_max_;
    M_Slave.Motor_Msg[num_id]._ack = 1; 
    M_Slave.Motor_Msg[num_id].cur = 0; 
}

//id,vel,cur_max
void Motor_Send_Msg::vel_control(int id,float vel_,float cur_max_){
    int num_id =  id;
    M_Slave.Motor_Msg[num_id].m_id = num_id;
    M_Slave.Motor_Msg[num_id].pvt = 1;
    M_Slave.Motor_Msg[num_id].poi = 0;
    M_Slave.Motor_Msg[num_id].vel = vel_;
    M_Slave.Motor_Msg[num_id].tor = 0;
    M_Slave.Motor_Msg[num_id].cur_max = cur_max_;
    M_Slave.Motor_Msg[num_id]._ack = 1; 
    M_Slave.Motor_Msg[num_id].cur = 0; 
}

//id,tor
void Motor_Send_Msg::tor_control(int id,float tor_){
    int num_id =  id;
    M_Slave.Motor_Msg[num_id].m_id = id;
    M_Slave.Motor_Msg[num_id].pvt = 2;
    M_Slave.Motor_Msg[num_id].poi = 0;
    M_Slave.Motor_Msg[num_id].vel = 0;
    M_Slave.Motor_Msg[num_id].tor = tor_;
    M_Slave.Motor_Msg[num_id].cur_max = 500;
    M_Slave.Motor_Msg[num_id]._ack = 1; 
    M_Slave.Motor_Msg[num_id].cur = 0; 
}

//id,KP,KD,poi,vel,tor
void Motor_Send_Msg::tor_poi_control(int id, float KP_, float KD_, float poi_, float vel_, float tor_){
    int num_id =  id;
    M_Slave.Motor_Msg[num_id].m_id = id;
    M_Slave.Motor_Msg[num_id].pvt = 3;
    M_Slave.Motor_Msg[num_id].KP = KP_;
    M_Slave.Motor_Msg[num_id].KD = KD_;
    M_Slave.Motor_Msg[num_id].poi = poi_;
    M_Slave.Motor_Msg[num_id].vel = vel_;
    M_Slave.Motor_Msg[num_id].tor = tor_;
}

void command_set_toc(Motor_Send_Msg msm,motor_command &m_command){
    for(int i = 0; i < 6; i++){
        m_command.Motor_Msg[i].m_id = msm.M_Slave.Motor_Msg[i].m_id;
        m_command.Motor_Msg[i].pvt = msm.M_Slave.Motor_Msg[i].pvt;
        m_command.Motor_Msg[i].poi = msm.M_Slave.Motor_Msg[i].poi;
        m_command.Motor_Msg[i].vel = msm.M_Slave.Motor_Msg[i].vel;
        m_command.Motor_Msg[i].tor = msm.M_Slave.Motor_Msg[i].tor;
        m_command.Motor_Msg[i].cur_max = msm.M_Slave.Motor_Msg[i].cur_max;
        m_command.Motor_Msg[i]._ack = msm.M_Slave.Motor_Msg[i]._ack;
        m_command.Motor_Msg[i].cur = msm.M_Slave.Motor_Msg[i].cur;
        // printf("m_id[%d]:%d\t", i, m_command.Motor_Msg[i].m_id);
    } 
       
}