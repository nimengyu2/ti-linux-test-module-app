/**
 * @file serial.h
 * @brief 串口设置
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
 * @date 2011-9-22 18:25:44
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__
/**
 * @addtogroup api_serial
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <data_types.h>

// TODO: Add your code here
extern bool_t serial_open(const char *serial_device);
extern bool_t serial_close(void);
extern bool_t serial_baudrate_set(int baudrate);
extern bool_t serial_baudrate_datasize_parity_stop_set(int baudrate,
                                                       int datasize,
                                                       int parity,
                                                       int stop);
extern bool_t serial_read_char(char *byte);
extern bool_t serial_read(char *byte, int num);
extern bool_t serial_send_char(char byte);
extern bool_t serial_send(char *byte, int num);

#ifdef __cplusplus
}
#endif

/**
 * Close the Doxygen group.
 * @}
 */
#endif // __SERIAL_H__

