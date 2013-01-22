/**
 * @file debug.h
 * @brief debug for s2wifi module.
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
 * @author toby.zhang <zxj@lierda.com>
 * @date 2011-9-23 11:36:21
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__
/**
 * @addtogroup api_debug
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

// 调试等级，0-7级别
// 0级表示禁止调试输出 1-3 数字越大输出信息越多

#define PIPE_DEBUG 		0
#define SHM_DEBUG  		0
#define CONFIG_DEBUG 	3
#define WPA_CFG_DEBUG 	3
#define WIFI_CONNECT_ROUTER_DEBUG 3
#define UART_DEBUG		3
#define WIFI_DEBUG		3
#define WIFI_MODE_DEBUG  3
#define TCP_SERVER_DEBUG 3
#define TCP_CLIENT_DEBUG 3
#define UDP_DEBUG        3
#define LED_DEBUG	3

#define MTD_SPI_FLASH


#define         DEBUG_TO_TERMINAL         1
#define         DEBUG_TO_SYSLOG           1

// TODO: Add your code here
extern void debug_open(void);
extern void debug_out(const char *format, ...);
extern void debug_close(void);

#ifdef __cplusplus
}
#endif

/**
 * Close the Doxygen group.
 * @}
 */
#endif // __DEBUG_H__

