/*
 * @Description:
 * @Author: kx zhang
 * @Date: 2022-09-20 09:21:54
 * @LastEditTime: 2022-11-13 17:00:10
 */
#pragma once

#include <inttypes.h>
#include <string.h>

//********************************************//
//***********EtherCAT Message*****************//
//********************************************//

#pragma pack(push, 1)

struct Motor_Msg
{
    uint32_t id;
    uint8_t rtr;
    uint8_t dlc;
    uint8_t data[8];
};

typedef struct
{
    uint8_t motor_num;
    uint8_t can_ide;
    struct Motor_Msg motor[6];

} EtherCAT_Msg;

typedef struct{
    int m_id;
    int pvt;
    float poi;
    float vel;
    float tor;
    float cur_max;
    int _ack;  
    int cur;  
} motor_msg;

typedef struct{
    int slave;
    motor_msg Motor_Msg[6];
} motor_command;

#pragma pack(pop)