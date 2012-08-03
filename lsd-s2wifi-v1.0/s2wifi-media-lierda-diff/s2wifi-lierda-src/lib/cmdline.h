/**
 * @file cmdline.h
 * @brief command line utils
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
 * @date 2011-9-22 21:33:49
 * @version 0.01
 *
 * This file is pary of toby's open source.
 */

#ifndef __CMDLINE_H__
#define __CMDLINE_H__
/**
 * @addtogroup api_cmdline
 * @{
 */
#ifdef __cplusplus
extern "C"
{
#endif

// TODO: Add your code here
/**
 * Defines the value that is returned if the command is not found.
 */
#define CMDLINE_BAD_CMD              (-1)
/**
 * Defines the value that is returned if there are too many arguments.
 */
#define CMDLINE_TOO_MANY_ARGS        (-2)

/**
 * Define the value that is returned if serial port change the config.
 */
#define CMDLINE_SERIAL_PORT_CHANGE   (-10)

typedef int (*pfn_cmdline)(int argc, char **argv);

struct t_cmdline_entry
{
    /**
     * A pointer to a string containing the name of the command.
     */
    const char *p_cmd;

    /**
     * A function pointer to the implementation of the command.
     */
    pfn_cmdline pfn_cmd;

    /**
     * A pointer to a string of brief help text for the command.
     */
    const char *p_help;
};

/**
 * This is the command table that must be provided by the application.
 */
extern struct t_cmdline_entry g_cmdtable[];

/**
 * Prototypes for the APIs.
 */
extern int cmdline_process(char *pcmdline);

#ifdef __cplusplus
}
#endif

/**
 * Close the Doxygen group.
 * @}
 */
#endif // __CMDLINE_H__


