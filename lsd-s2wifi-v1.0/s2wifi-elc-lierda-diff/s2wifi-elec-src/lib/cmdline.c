/**
 * @file cmdline.c
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
 * @date 2011-9-22 21:49:15
 * @version 0.01
 *
 * This file is pary of toby's open source.
 */

/**
 * @note
 * command line utils \n
 *
 * \b Change \b Logs: \n
 * 2011.09.22   init by toby \n
 *
 */

/**
 * @addtogroup api_cmdline
 * @{
 */

#include "cmdline.h"
#include <string.h>
#include <at.h>

// TODO: Add your code here
#ifndef CMDLINE_MAX_ARGS
#define CMDLINE_MAX_ARGS                    32
#endif

static char g_cmdline_tmp[CMDLINE_BUF_SIZE];

/**
 * 字符串小写转大写
 *
 * @author Toby (2011-9-28)
 *
 * @param s 需要转换的字符串
 */
void str_to_upper(char* s)
{
    while(*s != '\0')
    {
        if((*s >= 'a') && (*s <='z'))
            *s -= 32;
        s++;
    }
}

/**
 * Process a command line string into arguments and execute the command.
 * This function will take the supplied command line string and break it up
 * into individual arguments.  The first argument is treated as a command and is
 * searched for in the command table.  If the command is found, then the command
 * function is called and all of the command line arguments are passed in the
 * normal argc, argv form.
 * The command table is contained in an array named <tt>g_cmdtable</tt> which
 * must be provided by the application.
 *
 * @author toby.zhang (2011-9-22)
 *
 * @param pcmdline points to a string that contains a command line that was
 * obtained by an application by some means.
 *
 * @return int \b CMDLINE_BAD_CMD if the command is not found,
 * \b CMDLINE_TOO_MANY_ARGS if there are more arguments than can be parsed.
 * Otherwise it returns the code that was returned by the command function.
 */
int cmdline_process(char *pcmdline)
{
    static char *argv[CMDLINE_MAX_ARGS + 1];
    char *pchar;
    int argc;
    int is_find_arg = 1;
    struct t_cmdline_entry *pcmd_entry;

    /*
     * Initialize the argument counter, and point to the beginning of the
     * command line string.
     */
    argc = 0;
    pchar = pcmdline;

    /*
     * Advance through the command line until a zero character is found.
     */
    while(*pchar)
    {
        /*
         * If there is a space, then replace it with a zero, and set the flag to
         * search for the next argument.
         */
        if((*pchar == ' ') || (*pchar == '=') || (*pchar == ','))
        {
            *pchar = 0;
            is_find_arg = 1;
        }

        /*
         * Otherwise it is not a space, so it must be a character that is part
         * of an argument.
         */
        else
        {
            /*
             * If is_find_arg is set, then that means we are looking for the
             * start of the next argument.
             */
            if(is_find_arg)
            {
                /*
                 * As long as the maximum number of arguments has not been
                 * reached, then save the pointer to the start of this new arg
                 * in the argv array, and increment the count of args, argc.
                 */
                if(argc < CMDLINE_MAX_ARGS)
                {
                    argv[argc] = pchar;
                    argc++;
                    is_find_arg = 0;
                }

                /*
                 * The maximum number of arguments has been reached so return
                 * the error.
                 */
                else
                {
                    return(CMDLINE_TOO_MANY_ARGS);
                }
            }
        }

        /*
         * Advance to the next character in the command line.
         */
        pchar++;
    }

    /*
     * If one or more arguments was found, then process the command.
     */
    if(argc)
    {
        /*
         * Start at the beginning of the command table, to look for a matching
         * command.
         */
        pcmd_entry = &g_cmdtable[0];

        /*
         * Search through the command table until a null command string is
         * found, which marks the end of the table.
         */
        while(pcmd_entry->p_cmd)
        {
            /*
             * If this command entry command string matches argv[0], then call
             * the function for this command, passing the command line
             * arguments.
             */
            strcpy(g_cmdline_tmp, argv[0]);
            str_to_upper(g_cmdline_tmp);
            if(!strcmp(g_cmdline_tmp, pcmd_entry->p_cmd))
            {
                return(pcmd_entry->pfn_cmd(argc, argv));
            }

            /*
             * Not found, so advance to the next entry.
             */
            pcmd_entry++;
        }
    }

    /*
     * Fall through to here means that no matching command was found, so return
     * an error.
     */
    return(CMDLINE_BAD_CMD);
}

/**
 * Close the Doxygen group.
 * @}
 */

