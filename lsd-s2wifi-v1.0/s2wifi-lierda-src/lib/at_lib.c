/**
 * @file at_lib.c
 * @brief AT指令操作库
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
 * @date 2011-10-10 9:57:54
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI-V1.0.
 */

/**
 * @note
 * AT指令操作库，主要提供一些小的函数封装 \n
 *
 * \b Change \b Logs: \n
 * 2011.10.10   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_at_lib
 * @{
 */

#include "at_lib.h"
#include <data_types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <config_file_lib.h>
#include <uartstdio.h>

// TODO: Add your code here

/**
 * 读取AT的参数表，并进行相应的配置
 *
 * 0: g_bDisableEcho = false && g_bDisableAtq = true
 *    UARTEchoSet(true)         uart_echo_set(false)
 * 1: g_bDisableEcho = false && g_bDisableAtq = false
 *    UARTEchoSet(true)         uart_echo_set(true)
 * 2: g_bDisableEcho = true && g_bDisableAtq = true
 *    UARTEchoSet(false)       uart_echo_set(false)
 * 3: g_bDisableEcho = true && g_bDisableAtq = false
 *    UARTEchoSet(false)       uart_echo_set(true)
 *
 * @author Toby (2011-10-10)
 *
 * @return bool_t
 */
bool_t at_init_lib(void)
{
    int val;

    val = at_param_conf_read();

    if(val == -1)
    {
        return(false);
    }
    else if(val == 0)
    {
        UARTEchoSet(true);
        uart_echo_set(false);
    }
    else if(val == 1)
    {
        UARTEchoSet(true);
        uart_echo_set(true);
    }
    else if(val == 2)
    {
        UARTEchoSet(false);
        uart_echo_set(false);
    }
    else if(val == 3)
    {
        UARTEchoSet(false);
        uart_echo_set(true);
    }

    return(true);
}

/**
 * at参数保存
 *
 * 0: g_bDisableEcho = false && g_bDisableAtq = true
 *    UARTEchoSet(true)         uart_echo_set(false)
 * 1: g_bDisableEcho = false && g_bDisableAtq = false
 *    UARTEchoSet(true)         uart_echo_set(true)
 * 2: g_bDisableEcho = true && g_bDisableAtq = true
 *    UARTEchoSet(false)       uart_echo_set(false)
 * 3: g_bDisableEcho = true && g_bDisableAtq = false
 *    UARTEchoSet(false)       uart_echo_set(true)
 *
 * @author Toby (2011-10-10)
 *
 * @return bool_t
 */
bool_t at_save_lib(void)
{
    bool_t echo_val;
    bool_t aqt_val;

    echo_val = UARTEchoGet();
    aqt_val = uart_echo_get();
    if((echo_val == false) &&
       (aqt_val == true))
    {
        if(at_param_conf_factor_load(0) == false)
        {
            return(false);
        }
    }
    else if((echo_val == false) &&
            (aqt_val == false))
    {
        if(at_param_conf_factor_load(1) == false)
        {
            return(false);
        }
    }
    else if((echo_val == true) &&
            (aqt_val == true))
    {
        if(at_param_conf_factor_load(2) == false)
        {
            return(false);
        }
    }
    else if((echo_val == true) &&
            (aqt_val == false))
    {
        if(at_param_conf_factor_load(3) == false)
        {
            return(false);
        }
    }

    return(true);
}

/**
 * Close the Doxygen group.
 * @}
 */

