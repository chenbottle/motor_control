/*
 * @Description:
 * @Author: kx zhang
 * @Date: 2022-09-20 11:17:58
 * @LastEditTime: 2022-11-13 17:16:18
 */
#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "ethercat.h"
#include "sys/time.h"
#include "config_main.h"

void EtherCAT_Transmit();
void EtherCAT_Init(char *ifname);
void EtherCAT_Run(motor_command *m_command);
void EtherCAT_Command_Set(motor_command *m_command);
EtherCAT_Msg *EtherCAT_Data_Get();

#endif // PROJECT_RT_ETHERCAT_H