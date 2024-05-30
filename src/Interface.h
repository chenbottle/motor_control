/************************************************************************
Copyright (c) 2020, Unitree Robotics.Co.Ltd. All rights reserved.
Use of this source code is governed by the MPL-2.0 license, see LICENSE.
************************************************************************/

#ifndef INTERFACE_H
#define INTERFACE_H

#include <mutex>
#include "config_main.h"
#include <iostream>
#include "MotorControl.h"
#include "imu_get_data.h"
#include <lcm/lcm-cpp.hpp>
#include "lcm-types/cpp/imu_data.hpp"
#include "lcm-types/cpp/motor_data.hpp"

extern "C" {
#include "config.h"
#include "transmit.h"
}

using namespace std;

class Interface {
public:
    Interface();
    ~Interface();

    void RecvFromDriver();
    void SendDriver(); 
    void getIMUdata();
    void record_to_lcm();
private:     

    //Driver
    // EtherCAT_Msg _ReceiveData[4];    // 驱动器的反馈数据    
    // Motor_Send_Msg _msm[6];    // 发送给驱动器的指令
    //************************************************************//

    IMU imu;

    m_control _m_control;

    //lcm_data
    lcm::LCM lcm_motor;
    lcm::LCM lcm_imu;
    motor_data lcm_motor_data[6];
    imu_data lcm_imu_data;

    std::mutex IMUData_mutex, remote_mutex, driver_mutex, rece_driver_mutex;
};

// float ConvertByte2Float(unsigned char *pByte);

#endif
