/**
 * @file debug.c
 * @brief debug for s2wifi module.
 *
 * Copyright (C) 2006-2011, toby
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author toby.zhang <toby.chs@gmail.com>
 * @date 2011-9-23 11:26:37
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

/**
 * @note
 * debug for s2wifi module.\n
 *
 * \b Change \b Logs: \n
 * 2011.09.23   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_debug
 * @{
 */

#include "debug.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

// TODO: Add your code here

void debug_open(void)
{
    openlog("S2WIFI_Msg", LOG_CONS | LOG_PID, 0);
}

void debug_out(const char *format, ...)
{
    va_list va_arg_p;

    // Start the varargs processing.
    va_start(va_arg_p, format);

#ifdef DEBUG_TO_TERMINAL
    vfprintf(stderr, format, va_arg_p);
#endif

#ifdef DEBUG_TO_SYSLOG
    syslog(LOG_DEBUG, format, va_arg_p);
#endif

    // End the varargs processing.
    va_end(va_arg_p);
}

void debug_close(void)
{
    closelog();
}

/**
 * Close the Doxygen group.
 * @}
 */

