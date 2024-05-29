/*
 * @Description:
 * @Author: kx zhang
 * @Date: 2022-09-13 19:00:55
 * @LastEditTime: 2022-11-13 17:09:03
 */

#include <cstdio>
#include <thread>
#include "Loop.hpp"
#include "Interface.h"
#include <boost/bind.hpp>

extern "C" {
#include "ethercat.h"
#include "transmit.h"
}

int main()
{
    Interface interface;

    printf("SOEM 主站测试\n");

    // 这里填自己电脑上的网卡
    EtherCAT_Init("enp7s0f1");

    if (ec_slavecount <= 0)
    {
        printf("未找到从站, 程序退出！");
        return 1;
    }
    else
        printf("从站数量： %d\r\n", ec_slavecount);
    

    WSH_LOOP::LoopFunction loop_ReceFromDriver("loop_ReceFromDriver",
                                0.002, boost::bind(&Interface::RecvFromDriver, &interface)); 
    WSH_LOOP::LoopFunction loop_SendDriver("loop_SendDriver",
                                0.002, boost::bind(&Interface::SendDriver, &interface));   
    WSH_LOOP::LoopFunction loop_ReceFromIMU("loop_ReceFromIMU",
                                0.002, boost::bind(&Interface::getIMUdata, &interface));                        

    loop_ReceFromIMU.start();

    loop_ReceFromDriver.start();

    loop_SendDriver.start();  

    while(1){
        usleep(2000);
    }
    return 0;
}
