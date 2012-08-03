/**
 * @file at.c
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
 * @date 2011-9-26 11:10:21
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

/**
 * @note
 * AT commands \n
 *
 * \b Change \b Logs: \n
 * 2011.09.26   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_at_commands
 * @{
 */

#include "at.h"
#include <debug.h>
#include <data_types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmdline.h>
#include <version.h>
#include <config_file_lib.h>
#include <serial.h>
#include <param_check.h>
#include <at_lib.h>

// TODO: Add your code here
static char g_cmdline_buf[CMDLINE_BUF_SIZE]; //!< 缓存命令

/**
 * 帮助命令
 *
 * @author Toby (2011-9-26)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_help(int argc, char **argv)
{
    struct t_cmdline_entry *pentry;

    UARTprintf("\n");
    UARTprintf("S2WIFI Command List\r\n");
    UARTprintf("===================\r\n");

    pentry = &g_cmdtable[0];

    // 循环打印命令集合
    while(pentry->p_cmd)
    {
        UARTprintf("%s%s\n", pentry->p_cmd, pentry->p_help);

        // 指向下个命令
        pentry++;
    }

    printf("\r\n");
    return(0);
}

/**
 * AT测试
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_test(int argc, char **argv)
{
    return(0);
}

/**
 * 关闭本地回显功能
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_echo_off(int argc, char **argv)
{
    UARTEchoSet(false);

    return(0);
}

/**
 * 打开本地回显功能
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_echo_on(int argc, char **argv)
{
    UARTEchoSet(true);

    return(0);
}

/**
 * 提示模块生产厂家信息
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_ati(int argc, char **argv)
{
    UARTprintf("\n");
    UARTprintf("Lerida.Inc\r\n");
    UARTprintf("Revision: %s\r\n", VERSION);
    UARTprintf("Build Time: %s/%s\r\n", __DATE__, __TIME__);

    return(0);
}

/**
 * 显示命令执行的结果
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_atq_off(int argc, char **argv)
{
    uart_echo_set(false);

    return(0);
}

/**
 * 不显示命令执行的结果
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_atq_on(int argc, char **argv)
{
    uart_echo_set(true);

    return(0);
}

/**
 * 从命令模式切换到数据模式
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_ato(int argc, char **argv)
{
   // return(0);
    return(-100);
}

/**
 * 恢复出厂设置状态
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_factor_load(int argc, char **argv)
{
    if(all_param_factor_load() == false)
    {
        return(-1);
    }

    // 在恢复出厂设置前，先输出提示信息
    UARTprintf("\nOK\r\n");
    sleep(1);

    system("reboot");

    while(1)
    {
        /*
         * 程序永远也达到不到这里
         */
    }
    return(0);
}

/**
 * 保存当前的AT指令参数
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_param_save(int argc, char **argv)
{
    if(at_save_lib() == false)
    {
        return(-1);
    }

    return(0);
}

/**
 * 修改登入用户名和密码
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_login(int argc, char **argv)
{
    bool_t ret;

    if(argc == 1)
    {
        return(-1);
    }

    if(argc == 2)
    {
        if(strcmp(argv[1], "?") == 0)
        {
            // 输出命令格式，和取值范围
            UARTprintf("\n");
            UARTprintf("+LOGIN: (oldusername),(oldpassword),(newusername),\
(newpassword)\r\n");

            return(0);
        }
        else
        {
            return(-1);
        }
    }

    if(argc == 5)
    {
        char *old_username;
        char *old_password;
        int old_username_len;
        int old_password_len;
        int tmp_username_len;
        int tmp_password_len;
        int new_username_len;
        int new_password_len;

        /*
         * 判断输入的参数是否合法
         */
        if((is_usrname_passwd_valid(argv[1]) == false) ||
           (is_usrname_passwd_valid(argv[2]) == false) ||
           (is_usrname_passwd_valid(argv[3]) == false) ||
           (is_usrname_passwd_valid(argv[4]) == false))
        {
            return(-3);
        }

        /*
         * 读取当前使用的用户名和密码
         */
        if(login_username_get(&old_username) == false)
        {
            return(-1);
        }
        if(login_password_get(&old_password) == false)
        {
            return(-1);
        }

        /*
         * 比对用户输入的用户名和密码是否与参数表
         * 中记录的用户名和密码相同
         */
        old_username_len = strlen(old_username);
        old_password_len = strlen(old_password);
        tmp_username_len = strlen(argv[1]);
        tmp_password_len = strlen(argv[2]);
        new_username_len = strlen(argv[3]);
        new_password_len = strlen(argv[4]);

        if((old_username_len == tmp_username_len) &&
           (old_password_len == tmp_password_len))
        {
            if((strcmp(old_username, argv[1]) == 0) &&
               (strcmp(old_password, argv[2]) == 0))
            {
                /*
                 * 用户名和密码比对正确
                 * 存储新的用户名和密码
                 */
                if(login_username_set(argv[3], new_username_len) == false)
                {
                    return(-1);
                }
                if(login_password_set(argv[4], new_password_len) == false)
                {
                    return(-1);
                }

                /*
                 * 新用户名和密码设置成功
                 */
                return(0);
            }
        }
    }

    return(-1);
}

/**
 * 设置WLAN SSID
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_ssid(int argc, char **argv)
{
    bool_t ret;

    if(argc == 1)
    {
        return(-1);
    }

    if(argc == 2)
    {
        if((strcmp(argv[1], "?") == 0) &&
           (strlen(argv[1]) == 1))
        {
            // 输出命令格式，和取值范围
            UARTprintf("\n");
            UARTprintf("+SSID: (ssid)\r\n");

            return(0);
        }
        else
        {
            /*
             * 检测是否为合法的SSID号码
             */
            if(is_ssid_valid(argv[1]) == false)
            {
                return(-3);
            }

            if(wlan_ssid_set(argv[1], strlen(argv[1])) == false)
            {
                return(-1);
            }

            return(0);
        }
    }

    return(-1);
}

/**
 * 查看当前WLAN SSID
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_ssid_state(int argc, char **argv)
{
    char *pssid;

    if(wlan_ssid_get(&pssid) == false)
    {
        return(-3);
    }

    UARTprintf("\n");
    UARTprintf("+SSID: %s\r\n",
               pssid);

    return(0);
}

/**
 * 设置WLAN 加密方式
 * 0:None/1:WEP-open/2:WEP-share/3:WPA or WPA2
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_encryption_type(int argc, char **argv)
{
    bool_t ret;

    if(argc == 1)
    {
        return(-1);
    }

    if(argc == 2)
    {
        if((strcmp(argv[1], "?") == 0) &&
           (strlen(argv[1]) == 1))
        {
            // 输出命令格式，和取值范围
            UARTprintf("\n");
            UARTprintf("+ENCYPT: (0,1,2,3)\r\n");

            return(0);
        }
        else
        {
            /*
             * 检测是否为合法的SSID号码
             */
            if(is_encryption_type_valid(argv[1]) == false)
            {
                return(-3);
            }

            if(strlen(argv[1]) != 1)
            {
                return(-1);
            }

            if(wlan_encrytype_set(argv[1], strlen(argv[1])) == false)
            {
                return(-1);
            }

            return(0);
        }
    }

    return(-1);
}

/**
 * 查看WLAN 加密方式
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_encryption_type_state(int argc, char **argv)
{
    char *pencrytype;

    if(wlan_encrytype_get(&pencrytype) == false)
    {
        return(-3);
    }

    UARTprintf("\n");
    UARTprintf("+ENCYPT: %s\r\n",
               pencrytype);

    return(0);
}

/**
 * 设置WLAN SSID的密码
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_passwd(int argc, char **argv)
{
    bool_t ret;

    if(argc == 1)
    {
        return(-1);
    }

    if(argc == 2)
    {
        if((strcmp(argv[1], "?") == 0) &&
           (strlen(argv[1]) == 1))
        {
            // 输出命令格式，和取值范围
            UARTprintf("\n");
            UARTprintf("+PASSWD: (wifipassword)\r\n");

            return(0);
        }
        else
        {
            /*
             * 检测是否为合法的WIFI密码
             */
            if(is_wifi_password_valid(argv[1]) == false)
            {
                return(-3);
            }

            if(wlan_wifipassword_set(argv[1], strlen(argv[1])) == false)
            {
                return(-1);
            }

            return(0);
        }
    }

    return(-1);
}

/**
 * 获取当前WLAN SSID密码
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_passwd_state(int argc, char **argv)
{
    char *pwifipassword;

    if(wlan_wifipassword_get(&pwifipassword) == false)
    {
        return(-3);
    }

    UARTprintf("\n");
    UARTprintf("+PASSWD: %s\r\n",
               pwifipassword);

    return(0);
}

/**
 * 设置WLAN IP的获取方式 DHCP or StaticIP
 * 0:DHCP/1:StaticIP
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_dhcp(int argc, char **argv)
{
    bool_t ret;

    if(argc == 1)
    {
        return(-1);
    }

    if(argc == 2)
    {
        if((strcmp(argv[1], "?") == 0) &&
           (strlen(argv[1]) == 1))
        {
            // 输出命令格式，和取值范围
            UARTprintf("\n");
            UARTprintf("+DHCP: (0,1)\r\n");

            return(0);
        }
        else
        {
            /*
             * 检测是否为合法的SSID号码
             */
            if(is_address_type_valid(argv[1]) == false)
            {
                return(-3);
            }

            if(strlen(argv[1]) != 1)
            {
                return(-1);
            }

            if(wlan_staticip_set(argv[1], strlen(argv[1])) == false)
            {
                return(-1);
            }

            return(0);
        }
    }

    return(-1);
}

/**
 * 获取当前WLAN IP的获取方式
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_dhcp_state(int argc, char **argv)
{
    char *pstaticip;

    if(wlan_staticip_get(&pstaticip) == false)
    {
        return(-3);
    }

    UARTprintf("\n");
    UARTprintf("+DHCP: %s\r\n",
               pstaticip);

    return(0);
}

/**
 * 如果使用静态IP，设置所需要的IP信息
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_ipaddr(int argc, char **argv)
{
    bool_t ret;

    if(argc == 1)
    {
        return(-1);
    }

    if(argc == 2)
    {
        if(strcmp(argv[1], "?") == 0)
        {
            // 输出命令格式，和取值范围
            UARTprintf("\n");
            UARTprintf("+IPADDR: ((0-255).(0-255).(0-255).(0-255)),\
((0-255).(0-255).(0-255).(0-255)),((0-255).(0-255).(0-255).(0-255))\r\n" );

            return(0);
        }
        else
        {
            return(-1);
        }
    }

    if(argc == 4)
    {
        if((is_ip_valid(argv[1]) == false) &&
           (is_netmask_valid(argv[2]) == false) &&
           (is_ip_valid(argv[3]) == false) )
        {
            return(-3);
        }

        /*
         * 保存新的参数
         */
        if(wlan_wlansip_set(argv[1], strlen(argv[1])) == false)
        {
            return(-1);
        }
        if(wlan_wlanmip_set(argv[2], strlen(argv[2])) == false)
        {
            return(-1);
        }
        if(wlan_wlangip_set(argv[3], strlen(argv[3])) == false)
        {
            return(-1);
        }

        return(0);
    }

    return(-1);
}

/**
 * 查看当前系统使用的IP地址
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_ipaddr_state(int argc, char **argv)
{
    char *pwlansip;
    char *pwlanmip;
    char *pwlangip;

    if(wlan_wlansip_get(&pwlansip) == false)
    {
        return(-3);
    }
    if(wlan_wlanmip_get(&pwlanmip) == false)
    {
        return(-3);
    }
    if(wlan_wlangip_get(&pwlangip) == false)
    {
        return(-3);
    }

    UARTprintf("\n");
    UARTprintf("+IPADDR: %s, %s, %s\r\n",
               pwlansip,
               pwlanmip,
               pwlangip);

    return(0);
}

/**
 * 设置DNS服务器，这个DNS与DHCP获取的DNS并不冲突
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_dns(int argc, char **argv)
{
    bool_t ret;

    if(argc == 1)
    {
        return(-1);
    }

    if(argc == 2)
    {
        if((strcmp(argv[1], "?") == 0) &&
           (strlen(argv[1]) == 1))
        {
            // 输出命令格式，和取值范围
            UARTprintf("\n");
            UARTprintf("+DNS: ((0-255).(0-255).(0-255).(0-255))\r\n");

            return(0);
        }
        else
        {
            /*
             * 检测是否为合法的IP(DNS)地址
             */
            if(is_ip_valid(argv[1]) == false)
            {
                return(-3);
            }

            if(wlan_wlandip_set(argv[1], strlen(argv[1])) == false)
            {
                return(-1);
            }

            return(0);
        }
    }

    return(-1);
}

/**
 * 获取当前设置的DNS服务器,但并不是返回的DHCP获取的DNS
 * 服务器地址
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_dns_state(int argc, char **argv)
{
    char *pwlandip;

    if(wlan_wlandip_get(&pwlandip) == false)
    {
        return(-3);
    }

    UARTprintf("\n");
    UARTprintf("+DNS: %s\r\n",
               pwlandip);

    return(0);
}

/**
 * WIFI port 配置
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_wifi(int argc, char **argv)
{
    bool_t ret;

    if(argc == 1)
    {
        return(-1);
    }

    if(argc == 2)
    {
        if(strcmp(argv[1], "?") == 0)
        {
            // 输出命令格式，和取值范围
            UARTprintf("\n");
            UARTprintf("+WIFI: (1-65535),(1-65535),(0,1),(0,1),((0-255).\
(0-255).(0-255).(0-255))\r\n");

            return(0);
        }
        else
        {
            return(-1);
        }
    }

    if(argc == 6)
    {
        if(is_socket_port_valid(argv[1]) == false)
        {
            return(-3);
        }

        if(is_socket_port_valid(argv[2]) == false)
        {
            return(-3);
        }

        if(is_socket_mode_valid(argv[3]) == false)
        {
            return(-3);
        }

        if(is_socket_prol_valid(argv[4]) == false)
        {
            return(-3);
        }

        if(is_ip_valid(argv[5]) == false)
        {
            return(-3);
        }

        /*
         * 保存新的参数
         */
        if(wifi_port_socketlp_set(argv[1], strlen(argv[1])) == false)
        {
            return(-1);
        }
        if(wifi_port_socketrp_set(argv[2], strlen(argv[2])) == false)
        {
            return(-1);
        }
        if(wifi_port_socketmode_set(argv[3], strlen(argv[3])) == false)
        {
            return(-1);
        }
        if(wifi_port_socketprol_set(argv[4], strlen(argv[4])) == false)
        {
            return(-1);
        }
        if(wifi_port_socketrip_set(argv[5], strlen(argv[5])) == false)
        {
            return(-1);
        }

        return(0);
    }

    return(-1);
}

/**
 * 查询当前的WIFI端口配置状态
 *
 * @author Toby (2011-10-6)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_wifi_state(int argc, char **argv)
{
    char *psocketlp;
    char *psocketrp;
    char *psocketmode;
    char *psocketprol;
    char *psocketrip;

    if(wifi_port_socketlp_get(&psocketlp) == false)
    {
        return(-3);
    }
    if(wifi_port_socketrp_get(&psocketrp) == false)
    {
        return(-3);
    }
    if(wifi_port_socketmode_get(&psocketmode) == false)
    {
        return(-3);
    }
    if(wifi_port_socketprol_get(&psocketprol) == false)
    {
        return(-3);
    }
    if(wifi_port_socketrip_get(&psocketrip) == false)
    {
        return(-3);
    }

    UARTprintf("\n");
    UARTprintf("+WIFI: %s, %s, %s, %s, %s\r\n",
               psocketlp,
               psocketrp,
               psocketmode,
               psocketprol,
               psocketrip);

    return(0);
}

/**
 * 串口配置
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_serial(int argc, char **argv)
{
    bool_t ret;

    if(argc == 1)
    {
        return(-1);
    }

    if(argc == 2)
    {
        if(strcmp(argv[1], "?") == 0)
        {
            // 输出命令格式，和取值范围
            UARTprintf("\n");
            UARTprintf("+SERIAL: (110,300,600,1200,2400,4800,9600,14400,\
19200,38400,57600,115200,230400),(5,6,7,8),(0,1,2,3,4),(1,2),(0,1)\r\n");

            return(0);
        }
        else
        {
            return(-1);
        }
    }

    if(argc == 6)
    {
        int baudrate;
        int datasize;
        int parity;
        int stop;
        int flow;

        if(is_baud_rate_valid(argv[1]) == false)
        {
            return(-3);
        }
        baudrate = atoi(argv[1]);

        if(is_data_size_valid(argv[2]) == false)
        {
            return(-3);
        }
        datasize = atoi(argv[2]);

        if(is_parity_valid(argv[3]) == false)
        {
            return(-3);
        }
        parity = atoi(argv[3]);

        if(is_stop_valid(argv[4]) == false)
        {
            return(-3);
        }
        stop = atoi(argv[4]);

        if(is_flow_valid(argv[5]) == false)
        {
            return(-3);
        }
        flow = atoi(argv[5]);

        /*
         * 保存新的参数
         */
        if(serial_port_baudrate_set(argv[1], strlen(argv[1])) == false)
        {
            return(-1);
        }
        if(serial_port_datasize_set(argv[2], strlen(argv[2])) == false)
        {
            return(-1);
        }
        if(serial_port_parity_set(argv[3], strlen(argv[3])) == false)
        {
            return(-1);
        }
        if(serial_port_stop_set(argv[4], strlen(argv[4])) == false)
        {
            return(-1);
        }
        if(serial_port_flow_set(argv[5], strlen(argv[5])) == false)
        {
            return(-1);
        }

        // 在改变串口配置之前，先输出提示信息
        UARTprintf("\nOK\r\n");
        sleep(1);

        ret = serial_baudrate_datasize_parity_stop_set(
            baudrate, datasize, parity, stop);
        if(ret == false)
        {
            // 此步骤比较危险???
            return(-3);
        }

        return(CMDLINE_SERIAL_PORT_CHANGE);
    }

    return(-1);
}

/**
 * 查询当前的串口配置状态
 *
 * @author Toby (2011-9-28)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
static
int at_serial_state(int argc, char **argv)
{
    char *pbaudrate;
    char *pdatasize;
    char *pparity;
    char *pstop;
    char *pflow;

    if(serial_port_baudrate_get(&pbaudrate) == false)
    {
        return(-3);
    }
    if(serial_port_datasize_get(&pdatasize) == false)
    {
        return(-3);
    }
    if(serial_port_parity_get(&pparity) == false)
    {
        return(-3);
    }
    if(serial_port_stop_get(&pstop) == false)
    {
        return(-3);
    }
    if(serial_port_flow_get(&pflow) == false)
    {
        return(-3);
    }

    UARTprintf("\n");
    UARTprintf("+SERIAL: %s, %s, %s, %s, %s\r\n",
               pbaudrate,
               pdatasize,
               pparity,
               pstop,
               pflow);

    return(0);
}

/**
 * AT模式和透传模式处理函数
 *
 * @author Toby (2011-9-26)
 *
 * @param arg 1为AT模式，否则为透传模式
 *
 * @return int 不会返回
 */
int at_process(int arg)
{
    int cmdline_status;

    while(1)
    {
        if(arg == 1) // AT 模式
        {
            debug_out("[CMDLINE] AT mode\n");
#if 0
            UARTprintf("\r\n+---------------------------------------+\r\n");
            UARTprintf("|     Welcome To S2WIFI Config Tool     |\r\n");
            UARTprintf("|            Lierda @ 2011              |\r\n");
            UARTprintf("+---------------------------------------+\r\n");
            UARTprintf("--Factory MAC Address:%02X-%02X-%02X-%02X-%02X-%02X\r\n"
                       , 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF);
            UARTprintf("--Firmware Version   :%d\r\n", 20110926);
            UARTprintf("--Build Time         :%s/%s\r\n", __DATE__, __TIME__);
            UARTprintf("--Using Baud Rate 115200 8N1\r\n");
            UARTprintf("\r\nPress \'help\' to display list of commands\r\n");
#endif
            /*
             * 加载AT参数配置
             */
            if(at_init_lib() == false)
            {
                debug_out("[CMDLINE] at_init_lib failed\n");
            }
            UARTprintf("\r\n+CFUN: 1\r\n");
            while(1)
            {
#if 0
                UARTprintf("> ");
#endif
                /**
                 * 读取用户输入的指令
                 */
                UARTgets(g_cmdline_buf, sizeof(g_cmdline_buf));

                /*
                 * 命令行处理
                 */
                cmdline_status = cmdline_process(g_cmdline_buf);

                /*
                 * 错误命令处理
                 */
#ifdef DEBUG_TO_TERMINAL
        printf("[CMDLINE] cmdline_status = %d\n", cmdline_status);
#endif
                if(cmdline_status == 0)
                {
                    UARTprintf("\nOK\r\n");
                }
                else if(cmdline_status == CMDLINE_BAD_CMD)
                {
                    // 错误命令
                    UARTprintf("\nERROR\r\n");
                }
                else if(cmdline_status == CMDLINE_TOO_MANY_ARGS)
                {
                    // 参数错误
                    UARTprintf("\nERROR\r\n");
                }
                else if(cmdline_status == CMDLINE_SERIAL_PORT_CHANGE)
                {
                    // Do noting
                }
        else if(cmdline_status == -100)
        {
            debug_out("now exit ap mode \n");
            return 0;
        }
                else if(cmdline_status != 0)
                {
                    // 命令返回错误
                    UARTprintf("\nERROR\r\n");
                }
            }
        }
        else // 透传模式
        {
            debug_out("[CMDLINE] Transparent mode\n");
            while(1)
            {
                // 透传模式处理
            }
        }
    }
}

struct t_cmdline_entry g_cmdtable[] =
{
    /*
     * Standard
     */
    { "AT+HELP", at_help, ": List AT commands." },
    { "AT+LIST", at_help, ": Alias for AT+HELP"},
    { "AT", at_test, ": AT Test." },
    { "ATE0", at_echo_off, ": Do not echo characters locally." },
    { "ATE1", at_echo_on, ": Echo characters locally." },
    { "ATI", at_ati, ": List manufacturer." },
    { "ATQ0", at_atq_off, ": DCE transmits result codes." },
    { "ATQ1", at_atq_on, ": Result codes are suppressed and not transmitted." },
    { "ATO", at_ato, ": This command issued to return to online \
mode from command mode." },
    { "AT&F", at_factor_load, ": Set All Parameters to Factory Configure." },
    { "AT&W", at_param_save, ": Save Current Settings." },

    /*
     * Login: Username/Password
     */
    { "AT+LOGIN", at_login, ": Modify the Login username and password." },

    /*
     * WLAN Configure
     */
    { "AT+SSID", at_ssid, ": WLAN SSID Configure." },
    { "AT+SSID?", at_ssid_state, ": Show Current WLAN SSID Configure." },
    { "AT+ENCYPT", at_encryption_type, ": WLAN Encryption Type Configure." },
    { "AT+ENCYPT?", at_encryption_type_state, ": Show Current WLAN \
Encryption Type Configure." },
    { "AT+PASSWD", at_passwd, ": WLAN Password Configure." },
    { "AT+PASSWD?", at_passwd_state, ": Show Current WLAN Password Configure."},

    { "AT+DHCP", at_dhcp, ": WLAN DHCP Configure." },
    { "AT+DHCP?", at_dhcp_state, ": Show Current WLAN DHCP Configure." },
    { "AT+IPADDR", at_ipaddr, ": WLAN Static IP Configure." },
    { "AT+IPADDR?",at_ipaddr_state,": Show Current WLAN Static IP Configure."},
    { "AT+DNS", at_dns, ": WLAN DNS Server IP Configure." },
    { "AT+DNS?", at_dns_state, ": Show Current WLAN DNS IP Configure." },

    /*
     * WIFI Port Configure
     */
    { "AT+WIFI", at_wifi, ": WIFI Port Configure." },
    { "AT+WIFI?", at_wifi_state, ": Show Current WIFI Port Configure." },

    /*
     * Serial Port Configure
     */
    { "AT+SERIAL", at_serial, ": Serial Port Configure." },
    { "AT+SERIAL?", at_serial_state, ": Show Current Serial Port Configure." },


    /*
     * Other
     */

    { 0, 0, 0 }
};

/**
 * Close the Doxygen group.
 * @}
 */

