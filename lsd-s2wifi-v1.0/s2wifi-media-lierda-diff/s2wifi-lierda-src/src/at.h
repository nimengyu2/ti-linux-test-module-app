/**
 * @file at.h
 * @brief AT commands
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
 * @date 2011-9-26 11:35:45
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

#ifndef __AT_H__
#define __AT_H__
/**
 * @addtogroup api_at_commands
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

// TODO: Add your code here
#define CMDLINE_BUF_SIZE                64 //!< 有效命令的最大长度
#define CMDLINE_HISTORY_CNT             50

extern int at_process(int arg);

#ifdef __cplusplus
}
#endif

/**
 * Close the Doxygen group.
 * @}
 */
#endif // __AT_H__

