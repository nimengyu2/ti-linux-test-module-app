/**
 * @file param_check.h
 * @brief S2WIFI参数合法性检测
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
 * @date 2011-9-29 16:31:22
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

#ifndef __PARAM_CHECK_H__
#define __PARAM_CHECK_H__
/**
 * @addtogroup api_param_check
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <data_types.h>

// TODO: Add your code here
extern bool_t is_usrname_passwd_valid(char *arg);

extern bool_t is_in_range(int val, int min, int max);
extern bool_t is_ssid_valid(void *arg);
extern bool_t is_encryption_type_valid(void *arg);
extern bool_t is_wifi_password_valid(void *arg);
extern bool_t is_address_type_valid(void *arg);
extern bool_t is_ip_valid(void *arg);
extern bool_t is_netmask_valid(void *arg);

extern bool_t is_socket_port_valid(void *arg);
extern bool_t is_socket_mode_valid(void *arg);
extern bool_t is_socket_prol_valid(void *arg);

extern bool_t is_baud_rate_valid(void *arg);
extern bool_t is_data_size_valid(void *arg);
extern bool_t is_parity_valid(void *arg);
extern bool_t is_stop_valid(void *arg);
extern bool_t is_flow_valid(void *arg);

#ifdef __cplusplus
}
#endif

/**
 * Close the Doxygen group.
 * @}
 */
#endif // __PARAM_CHECK_H__

