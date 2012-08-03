/**
 * @file s2wifi.c
 * @brief GGI程序，输出系统配置参数状态
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
 * @date 2011-9-16 16:33:22
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

/**
 * @note
 * GGI程序，输出系统配置参数状态 \n
 *
 * \b Change \b Logs: \n
 * 2011.09.16   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_s2wifi_cgi
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mtd_ops.h"

// TODO: Add your code here

/**
 * Main Function
 *
 * @author Toby (2011-9-16)
 *
 * @return int
 */
int main(void)
{
 	printf("Content-type: text/html\n\n");
	printf("Module Reset\n");
	//system("/bin/echo wifi_mode=STA > /etc/wifi_mode.conf");
	debug_open();
	fn_mtd_write(M_CFG_BLOCK_WIFI_MODE_CONF,"#wifi_mode=STA\n",strlen("#wifi_mode=STA\n"));
	debug_close();  
	system("/bin/sync");
	system("/sbin/reboot -d 1&");

    	return(0);
}

/**
 * Close the Doxygen group.
 * @}
 */

