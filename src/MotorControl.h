#ifndef TWOWHEELLEG_H
#define TWOWHEELLEG_H

#include <iostream>
#include <fstream>
#include <cmath>
#include "config_main.h"
#include <time.h>
#include <numeric>

extern "C" {
#include "config.h"
#include "transmit.h"
#include "motor_control.h"
}

using namespace std;

#define TORQUE 100
#define MAX_TORQUE 5

float uint_to_float(int x_int, float x_min, float x_max, int bits);

class m_control
{
public:
    m_control();   
    ~m_control();  
    void get_data(EtherCAT_Msg *_msg);
    motor_command* move();

    OD_Motor_Msg m_body[6];    
private:
    Motor_Send_Msg msm_[6];
    motor_command M_com[6];
       
    int motor_id; 
    EtherCAT_Msg _ReceiveData[4];
};
#endif//TWOWHEELLEG_H
