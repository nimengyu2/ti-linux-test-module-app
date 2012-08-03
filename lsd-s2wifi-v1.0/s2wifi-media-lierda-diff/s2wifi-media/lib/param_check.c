/**
 * @file param_check.c
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
 * @date 2011-9-29 16:33:13
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

/**
 * @note
 * S2WIFI参数合法性检测 \n
 *
 * \b Change \b Logs: \n
 * 2011.09.29   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_param_check
 * @{
 */

#include "param_check.h"
#include <data_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// TODO: Add your code here

/**
 * 检测用户名和密码格式是否合法
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param arg 需要检测的字符串
 *
 * @return bool_t true:格式合法/false:格式不合法
 */
bool_t is_usrname_passwd_valid(char *arg)
{
    char *p;
    int len;

    p = arg;
    len = strlen(p);

    if((len>0) && (len<11))
    {
        return(true);
    }

    return(false);
}

/**
 * 范围比较函数，包含min和max值
 *
 * @author Toby (2011-9-21)
 *
 * @param val 需要比较的值
 * @param min 范围最小值
 * @param max 范围最大值
 *
 * @return int true:在此范围中/false:不再指定的范围中
 */
bool_t is_in_range(int val, int min, int max)
{
    if((val >= min) && (val <= max))
    {
        /*
         * 在指定范围中
         */
        return(true);
    }
    else
    {
        return(false);
    }

}

/**
 * WIFI SSID检测
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_ssid_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    if((len>0) && (len<64))
    {
        return(true);
    }

    return(false);
}

/**
 * WIFI 加密方式检测
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_encryption_type_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;
    int encryption_type;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    /*
     * 数据是否为十进制数
     */
    for(i=0; i<len; i++)
    {
        if(isdigit(*p++) == 0)
        {
            /*
             * 非数字值
             */
            return(false);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    encryption_type = atoi(p);

    // 0:None/1:WEP-open/2:WEP-share/3:WPA or WPA2
    switch(encryption_type)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        {
            return(true);
            break;
        }
        default:
        {
            return(false);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * WIFI密码格式检测
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_wifi_password_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    if((len>0) && (len<33))
    {
        return(true);
    }

    return(false);
}

/**
 * wifi ip地址获取方式
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_address_type_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;
    int address_type;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    /*
     * 数据是否为十进制数
     */
    for(i=0; i<len; i++)
    {
        if(isdigit(*p++) == 0)
        {
            /*
             * 非数字值
             */
            return(false);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    address_type = atoi(p);

    // 0:DHCP/1:StaticIP.
    switch(address_type)
    {
        case 0:
        case 1:
        {
            return(true);
            break;
        }
        default:
        {
            return(false);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * IP地址是否有效检测
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_ip_valid(void *arg)
{
    char *p;
    char arg_tmp[100];
    int len;
    int i, j;
    int ret;
    int _ip[4] = {0}; // 整形数组
    int count = 0;
    char __ip[4][4] = {0}; // 字符串数组
    char *tmp;


    //p = (char *)arg;
    p = arg;

    /*
     * 为了不让strsep损坏原始数据，必须把原始数据备份
     */
    memset(arg_tmp, 0, sizeof(arg_tmp));
    strncpy(arg_tmp, p, strlen(p));
    p = arg_tmp;

    //memset(__ip, 0, sizeof(__ip));

    while((tmp = strsep(&p, ".")) != NULL)
    {
        if(count > 3)
        {
            /*
             * 发生IP格式错误
             */
            return(false);
        }

        if((strlen(tmp) > 3) && (strlen(tmp) < 1))
        {
            /*
             * 发生IP格式错误
             */
            return(false);
        }
        else
        {
            sprintf(__ip[count], "%s", tmp);
        }

        count++;
    }

    /*
     * 数据是否为十进制数
     */
    for(j=0; j<4; j++)
    {
        len = strlen(__ip[j]);
        p = __ip[j];

        for(i=0; i<len; i++)
        {
            if(isdigit(*p++) == 0)
            {
                /*
                 * 非数字值
                 */
                return(false);
            }
        }
    }

    /*
     * 是数字，字符转换成整形
     */
    for(i=0; i<4; i++)
    {
        _ip[i] = atoi(__ip[i]);
    }

    if(is_in_range(_ip[0], 1, 223) == false)
        return(false);
    if(_ip[0] == 127)
        return(false);
    if(is_in_range(_ip[1], 0, 255) == false)
        return(false);
    if(is_in_range(_ip[2], 0, 255) == false)
        return(false);
    if(is_in_range(_ip[3], 1, 254) == false)
        return(false);

    /*
     * 格式正确
     */
    return(true);
}

/**
 * 检测是否为一个合法的掩码
 *
 * @author Toby (2011-9-21)
 *
 * @param d 需要检测的值
 *
 * @return int true:有效/false:无效
 */
static
bool_t __is_mask(int d)
{
    if((d==0) || (d==128) ||
       (d==192) || (d==224) ||
       (d==240) || (d==248) ||
       (d==252) || (d==254) ||
       (d==255)
       )
    {
        return(true);
    }

    return(false);
}

/**
 * netmask地址是否有效检测
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_netmask_valid(void *arg)
{
    char *p;
    char arg_tmp[100];
    int len;
    int i, j;
    int ret;
    int _ip[4] = {0};
    int count = 0;
    char __ip[4][4] = {0};
    char *tmp;


    //p = (char *)arg;
    p = arg;

    memset(arg_tmp, 0, sizeof(arg_tmp));
    strncpy(arg_tmp, p, strlen(p));
    p = arg_tmp;

    while((tmp = strsep(&p, ".")) != NULL)
    {
        if(count > 3)
        {
            /*
             * 发生IP格式错误
             */
            return(false);
        }

        if((strlen(tmp) > 3) && (strlen(tmp) < 1))
        {
            /*
             * 发生IP格式错误
             */
            return(false);
        }
        else
        {
            sprintf(__ip[count], "%s", tmp);
        }

        count++;
    }

    /*
     * 数据是否为十进制数
     */
    for(j=0; j<4; j++)
    {
        len = strlen(__ip[j]);
        p = __ip[j];

        for(i=0; i<len; i++)
        {
            if(isdigit(*p++) == 0)
            {
                /*
                 * 非数字值
                 */
                return(false);
            }
        }
    }

    /*
     * 是数字，字符转换成整形
     */
    for(i=0; i<4; i++)
    {
        _ip[i] = atoi(__ip[i]);
    }

    if(__is_mask(_ip[0]) == false)
        return(false);
    if(_ip[0] == 0)
        return(false);
    if(_ip[0] != 255)
    {
        if((_ip[1]==0) && (_ip[2]==0) && (_ip[3]==0))
            return(true);
        return(false);
    }

    if(__is_mask(_ip[1]) == false)
        return(false);
    if(_ip[1] != 255)
    {
        if((_ip[2]==0) && (_ip[3]==0))
            return(true);
        return(false);
    }

    if(__is_mask(_ip[2]) == false)
        return(false);
    if(_ip[2] != 255)
    {
        if(_ip[3] == 0)
            return(true);
        return(false);
    }

    if(__is_mask(_ip[3]) == false)
        return(false);
    return(true);

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * socket端口参数合法性测试
 *
 * @author Toby (2011-9-20)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_socket_port_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;
    int socket_port;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    /*
     * 数据是否为十进制数
     */
    for(i=0; i<len; i++)
    {
        if(isdigit(*p++) == 0)
        {
            /*
             * 非数字值
             */
            return(false);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    socket_port = atoi(p);

    // socket_port <- 1~65535
    ret = is_in_range(socket_port, 1, 65535);
    if(ret == true)
    {
        return(true);
    }
    else
    {
        return(false);
    }

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * wifi port 工作模式 0:Server/1:Client 校验
 *
 * @author Toby (2011-9-20)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_socket_mode_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;
    int socket_mode;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    /*
     * 数据是否为十进制数
     */
    for(i=0; i<len; i++)
    {
        if(isdigit(*p++) == 0)
        {
            /*
             * 非数字值
             */
            return(false);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    socket_mode = atoi(p);

    // 0:Server/1:Client
    switch(socket_mode)
    {
        case 0:
        case 1:
        {
            return(true);
            break;
        }
        default:
        {
            return(false);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * wifi port 工作协议模式 0:TCP/1:UDP 检测
 *
 * @author Toby (2011-9-20)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_socket_prol_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;
    int socket_prol;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    /*
     * 数据是否为十进制数
     */
    for(i=0; i<len; i++)
    {
        if(isdigit(*p++) == 0)
        {
            /*
             * 非数字值
             */
            return(false);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    socket_prol = atoi(p);

    // 0:TCP/1:UDP
    switch(socket_prol)
    {
        case 0:
        case 1:
        {
            return(true);
            break;
        }
        default:
        {
            return(false);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * 检测波特率参数值是否正确
 *
 * @author Toby (2011-9-20)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_baud_rate_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;
    int baud_rate;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    /*
     * 数据是否为十进制数
     */
    for(i=0; i<len; i++)
    {
        if(isdigit(*p++) == 0)
        {
            /*
             * 非数字值
             */
            return(false);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    baud_rate = atoi(p);

    // 110/300/600/1200/2400/4800/9600/14400/19200/38400/57600/
    // 115200/230400
    switch(baud_rate)
    {
        case 110:
        case 300:
        case 600:
        case 1200:
        case 2400:
        case 4800:
        case 9600:
        case 14400:
        case 19200:
        case 38400:
        case 57600:
        case 115200:
        case 230400:
        {
            return(true);
            break;
        }
        default:
        {
            return(false);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * 串口数据位是否有效
 *
 * @author Toby (2011-9-20)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_data_size_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;
    int data_size;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    /*
     * 数据是否为十进制数
     */
    for(i=0; i<len; i++)
    {
        if(isdigit(*p++) == 0)
        {
            /*
             * 非数字值
             */
            return(false);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    data_size = atoi(p);

    // 5/6/7/8
    switch(data_size)
    {
        case 5:
        case 6:
        case 7:
        case 8:
        {
            return(true);
            break;
        }
        default:
        {
            return(false);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * 串口校验位是否正确
 *
 * @author Toby (2011-9-20)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_parity_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;
    int parity;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    /*
     * 数据是否为十进制数
     */
    for(i=0; i<len; i++)
    {
        if(isdigit(*p++) == 0)
        {
            /*
             * 非数字值
             */
            return(false);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    parity = atoi(p);

    // 0/1/2/3
    switch(parity)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        {
            return(true);
            break;
        }
        default:
        {
            return(false);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * 串口停止位是否正确
 *
 * @author Toby (2011-9-20)
 *
 * @param arg 需要检测的参数值
 *
 * @return int true:有效/false:无效
 */
bool_t is_stop_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;
    int stop;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    /*
     * 数据是否为十进制数
     */
    for(i=0; i<len; i++)
    {
        if(isdigit(*p++) == 0)
        {
            /*
             * 非数字值
             */
            return(false);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    stop = atoi(p);

    // 0/1/2/3
    switch(stop)
    {
        case 1:
        case 2:
        {
            return(true);
            break;
        }
        default:
        {
            return(false);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * 串口流控制是否正确
 *
 * @author Toby (2011-9-20)
 *
 * @param arg
 *
 * @return int true:有效/false:无效
 */
bool_t is_flow_valid(void *arg)
{
    char *p;
    int len;
    int i;
    int ret;
    int flow;

    //p = (char *)arg;
    p = arg;
    len = strlen(p);

    /*
     * 数据是否为十进制数
     */
    for(i=0; i<len; i++)
    {
        if(isdigit(*p++) == 0)
        {
            /*
             * 非数字值
             */
            return(false);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    flow = atoi(p);

    // 0/1/2/3
    switch(flow)
    {
        case 0:
        case 1:
        {
            return(true);
            break;
        }
        default:
        {
            return(false);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    return(true);
}

/**
 * Close the Doxygen group.
 * @}
 */

