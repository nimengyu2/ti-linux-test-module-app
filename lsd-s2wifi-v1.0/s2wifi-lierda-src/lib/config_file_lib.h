/**
 * @file config_file_lib.h
 * @brief 配置文件操作函数库
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
 * @date 2011-9-28 15:51:34
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

#ifndef __CONFIG_FILE_LIB_H__
#define __CONFIG_FILE_LIB_H__
/**
 * @addtogroup api_config_file_lib
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

// TODO: Add your code here
#include <data_types.h>

/*
 * Serial Port Configure
 */
extern bool_t fn_first_read_config(void);
extern bool_t fn_write_login_config_to_mtd(void);
extern bool_t fn_write_s2wifi_config_to_mtd(void);

extern bool_t login_username_get(char **val);
extern bool_t login_username_set(char *str, int len);
extern bool_t login_password_get(char **val);
extern bool_t login_password_set(char *str, int len);
extern bool_t login_islogin_get(char **val);
extern bool_t login_islogin_set(char *str, int len);

extern bool_t wlan_ssid_get(char **val);
extern bool_t wlan_ssid_set(char *str, int len);
extern bool_t wlan_encrytype_get(char **val);
extern bool_t wlan_encrytype_set(char *str, int len);
extern bool_t wlan_wifipassword_get(char **val);
extern bool_t wlan_wifipassword_set(char *str, int len);
extern bool_t wlan_staticip_get(char **val);
extern bool_t wlan_staticip_set(char *str, int len);
extern bool_t wlan_wlansip_get(char **val);
extern bool_t wlan_wlansip_set(char *str, int len);
extern bool_t wlan_wlanmip_get(char **val);
extern bool_t wlan_wlanmip_set(char *str, int len);
extern bool_t wlan_wlangip_get(char **val);
extern bool_t wlan_wlangip_set(char *str, int len);
extern bool_t wlan_wlandip_get(char **val);
extern bool_t wlan_wlandip_set(char *str, int len);

extern bool_t wifi_port_socketlp_get(char **val);
extern bool_t wifi_port_socketlp_set(char *str, int len);
extern bool_t wifi_port_socketrp_get(char **val);
extern bool_t wifi_port_socketlp_set(char *str, int len);
extern bool_t wifi_port_socketmode_get(char **val);
extern bool_t wifi_port_socketmode_set(char *str, int len);
extern bool_t wifi_port_socketprol_get(char **val);
extern bool_t wifi_port_socketprol_set(char *str, int len);
extern bool_t wifi_port_socketrip_get(char **val);
extern bool_t wifi_port_socketrip_set(char *str,  int len);

extern bool_t serial_port_baudrate_get(char **val);
extern bool_t serial_port_baudrate_set(char *str, int len);
extern bool_t serial_port_datasize_get(char **val);
extern bool_t serial_port_datasize_set(char *str, int len);
extern bool_t serial_port_parity_get(char **val);
extern bool_t serial_port_parity_set(char *str, int len);
extern bool_t serial_port_stop_get(char **val);
extern bool_t serial_port_stop_set(char *str, int len);
extern bool_t serial_port_flow_get(char **val);
extern bool_t serial_port_flow_set(char *str, int len);

extern bool_t login_file_factor_load(void);
extern bool_t s2wifi_file_factor_load(void);
extern bool_t wifi_mode_file_factor_load(void);
extern bool_t at_param_conf_factor_load(int val);
extern int at_param_conf_read(void);
extern bool_t all_param_factor_load(void);

#ifdef __cplusplus
}
#endif

/**
 * Close the Doxygen group.
 * @}
 */
#endif // __CONFIG_FILE_LIB_H__

