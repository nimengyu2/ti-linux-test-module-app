/**
 * @file s2wifi.c
 * @brief Main Function of Serial to WIFI Module
 *
 * Copyright (C) 2001-2011, LSD Science & Technology Co.,Ltd
 * All rights reserved.
 * Software License Agreement
 *
 * LSD Science & Technology (LSD) is supplying this software for use solely
 * its suppliers, and is protected. You may not combine this software with
 * "viral" open software in order to form a larger program.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
 * NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
 * NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. LSD SHALL NOT, UNDER ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 * DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @author toby.zhang <zxj@lierda.com> <toby.chs@gmail.com>
 * @date 2011-9-22 14:19:36
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

/**
 * @note
 * 串口转WIFI模块主程序 \n
 *
 * \b Change \b Logs: \n
 * 2011.09.22   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_s2wifi_api
 * @{
 */

//#include "s2wifi.h"
#include <debug.h>
#include <data_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <at.h>
#include <config_file_lib.h>
#include <serial.h>

/**
 * 串口转WIFI模块Mian函数
 *
 * @author Toby (2011-9-22)
 *
 * @param argc 参数大小
 * @param argv 参数数组
 *
 * @return int
 */
int at_uart_process(void)
{
    u8_t i;
    bool_t is_wlan_connect = false;

    /*
     * 启动主应用服务
     */
    debug_out("[S2WIFI] Start Serial to WIFI Module Server\n");

    /*
     * 开机自检
     */
    //system_check_self();

    /*
     * 参数读取
     */
    //read_config()

    /*
     * A. 串口设置 Serial Port Setup
     */
    // nmy modify 如果是普通学习板,可以用UART0
    // 如果是我们s2wifi板子上的4线,需要用UART1	
    //serial_open("/dev/ttyS0");
    serial_open("/dev/ttyS1");
    // 读取串口配置参数,并设置相应的串口配置信息
    if(1)
    {
        char *pbaudrate;
        char *pdatasize;
        char *pparity;
        char *pstop;
        char *pflow;

        if(serial_port_baudrate_get(&pbaudrate) == false)
        {
            exit(1);
        }
        if(serial_port_datasize_get(&pdatasize) == false)
        {
            exit(1);
        }
        if(serial_port_parity_get(&pparity) == false)
        {
            exit(1);
        }
        if(serial_port_stop_get(&pstop) == false)
        {
            exit(1);
        }
        if(serial_port_flow_get(&pflow) == false)
        {
            exit(1);
        }

        serial_baudrate_datasize_parity_stop_set(atoi(pbaudrate),
                                                 atoi(pdatasize),
                                                 atoi(pparity),
                                                 atoi(pstop));
    }

    /*
     * 无线网络环境设置 WLAN Setup
     */
    //wlan_setup();

    /*
     * B. 如果无线网络可用，启动wifi服务 WIFI Port Setup
     */
    if(is_wlan_connect)
    {
        // 无线网络已连接成功
        // 尝试启动WIFI连接
        //wifi_port_setup();
    }

    /*
     * 系统启动后，默认进入AT指令模式
     */

    //while(1)
  //  {
        at_process(1);
  //  }

    //exit(0);
}


/**
 * Close the Doxygen group.
 * @}
 */

