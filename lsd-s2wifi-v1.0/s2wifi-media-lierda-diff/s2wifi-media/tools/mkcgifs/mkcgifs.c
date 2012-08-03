/**
 * @file mkcgifs.c
 * @brief html转换成CGI代码小工具
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
 * @date 2011-10-12 12:01:00
 * @version 0.01
 *
 * This file is pary of toby's open sources.
 */

/**
 * @note
 *  \n
 *
 * \b Change \b Logs: \n
 * 2011.10.12   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_mkcgifs
 * @{
 */

//#include "mkcgifs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG               0
#define FILE_COLUMN_MAX     1000
#define FILE_NAME_MAX       10


/**
 * 大小写相互转换
 *
 * @author toby.zhang (2011-10-12)
 *
 * @param s 要转换的字符串
 * @param type 0:大写/1:小写
 *
 * @return char*
 */
static
char* change(char *s, int type)
{
    char *str = s;
    char *res = s;
    if(type == 0)
    {
        while(*s)
            *str++ = toupper(*s++);
    }
    else
    {
        while(*s)
            *str++ = tolower(*s++);
    }
    return   res;
}

// TODO: Add your code here

/**
 * main function
 *
 * 1. mkcgifs -b x.html outputfile
 * 2. mkcgifs -f outputfile x.c
 *
 * @author Toby (2011-10-12)
 *
 * @param argc
 * @param argv
 *
 * @return int
 */
int main(int argc, char **argv)
{
    FILE *fps, *fpd, *fph;
    char buf[FILE_COLUMN_MAX];
    char data;
    int count;
    int status_machine;
    static int b_first = 0;
    int len;
    int row;
    int i, j;
    char file_name_fun[FILE_NAME_MAX], h_point;
    char file_name_fun_copy[FILE_NAME_MAX];


    int opt = 0;

    if(argc != 4)
    {
        printf("USEAGE: mkcgifs -f inputfile outfile\n");
        exit(1);
    }

    if((strncmp("-b", argv[1], 2) == 0) &&
       (strlen(argv[1]) == 2))
    {
        printf("-b\n");
        opt = 1;
    }
    else if((strncmp("-f", argv[1], 2) == 0) &&
            (strlen(argv[1]) == 2))
    {
        printf("-f\n");
        opt = 2;
    }
    else
    {
        printf("USEAGE: mkcgifs -f inputfile outfile\n");
        exit(1);
    }

    /*
     * 打开原始文件
     */
    fps = fopen(argv[2], "rb");
    if(fps == NULL)
    {
        printf("open %s failed\n", argv[2]);
        exit(1);
    }
    printf("Open %s SUCCEED\n", argv[2]);
    fseek(fps, 0, SEEK_SET);

    /*
     * 打开输出文件
     */
    fpd = fopen(argv[3], "wb+");
    if(fpd == NULL)
    {
        printf("open %s failed\n", argv[3]);
        exit(1);
    }
    printf("Create %s SUCCEED\n", argv[3]);
    fseek(fpd, 0, SEEK_SET);

#if DEBUG
    printf("[DEBUG] opt = %d\n", opt);
#endif
    /*
     * 添加所需要的操作，文件转换开始
     */
    if(opt == 1)
    { // -b 美化
        status_machine = 1;
        b_first = 0;
        while((data = fgetc(fps)) != EOF)
        {
            if((data == '\n') ||
               (data == '\r'))
            {
                continue;
            }

            switch(status_machine)
            {
                case 1:{
                    /*
                     * 初始化状态机
                     */
                    if(data == '<')
                    {
                        fputc(data, fpd);
                        status_machine = 2;
                        break;
                    }

                    if(b_first == 1)
                    {
                        fputc(data, fpd);
                        if(data == ';')
                        {
                            fputc('\n', fpd);
                        }
                        break;
                    }
                    else
                    {
                        if(data != ' ')
                        {
                            fputc(data, fpd);
                            b_first = 1;
                        }
                        break;
                    }
                }

                case 2:{
                    /*
                     * 已经找到 '<' 符合
                     */
                    if(data == '>')
                    {
                        fputc(data, fpd);
                        fputc('\n', fpd);
                        b_first = 0;
                        status_machine = 1;
                        break;
                    }

#if 0
                    if(data == '"')
                    {
                        if(opt == 1)
                        {
                            fputc('\\', fpd);
                        }
                    }
#endif

                    fputc(data, fpd);
                    break;
                }
                default:
                {
                    printf("ERROR");
                }
            }
        }

        printf("-b SUCCEED\n");
    }

    row = 0;
    if(opt == 2)
    { // -f 输出C语言代码 (CGI程序可识别的)
        while((data = fgetc(fps)) != EOF)
        {
            if(data == '\n')
            {
                row++;
            }
        }
#if DEBUG
        printf("row = %d\n", row);
#endif

        /*
         * 读写指针复位
         */
        fseek(fps, 0, SEEK_SET);
        fprintf(fpd, "\
//===========================================================================\n\
// %s\n\
//\n\
// This file was automatically generated using the mkcgifs utility.\n\
//\n\
// Build by toby <toby.chs@gmail.com> %s %s\n\
//===========================================================================\n\
\n", argv[3], __DATE__, __TIME__);

        memset(file_name_fun, 0, sizeof(file_name_fun));
        for(i=0; (i<(FILE_NAME_MAX-1)) && (i<(strlen(argv[3])-1)); i++)
        { // 最大取前10位名字
            if(argv[3][i] == '.')
            {
                break;
            }
            file_name_fun[i] = argv[3][i];
        }
        file_name_fun[i] = '\0';
        h_point = i;

        fprintf(fpd, "#include \"%s.h\"\n", file_name_fun);
        fprintf(fpd, "#include <stdio.h>\n");
        fprintf(fpd, "#include <stdlib.h>\n");
        fprintf(fpd, "#include <string.h>\n\n");

        fprintf(fpd, "int print_%s_html(void)\n", file_name_fun);
        fprintf(fpd, "{\n");

        for(i=0; i<row; i++)
        {
            memset(buf, 0, sizeof(buf));
            fgets(buf, FILE_COLUMN_MAX, fps);
#if DEBUG
            printf("ROW %04d: %s\n", i, buf);
#endif
            fputs("    printf(\"", fpd);

            len = strlen(buf);
            for(j=0; j<len; j++)
            {
                /*
                 * 遇到换行，丢弃
                 */
                if(buf[j] == '\n')
                    break;

                /*
                 * 遇到 '"' --> "\""
                 */
                if(buf[j] == '"')
                {
                    fputc('\\', fpd);
                }

                fputc(buf[j], fpd);
            }

            /*
             * 在这里补充上次删除的换行
             */
            fputs("\\n\");\n", fpd);
        }

        fprintf(fpd, "}\n\n");

        /*
         * 创建一个与.c同名的.h文件
         */
        file_name_fun[h_point] = '.';
        file_name_fun[h_point+1] = 'h';
        fph = fopen(file_name_fun, "wb+");
        if(fph == NULL)
        {
            printf("Create %s file faied\n", file_name_fun);
            exit(1);
        }
        printf("Create %s file SUCCEED\n", file_name_fun);
        fseek(fph, 0, SEEK_SET);
        fprintf(fph, "\
//===========================================================================\n\
// %s\n\
//\n\
// This file was automatically generated using the mkcgifs utility.\n\
//\n\
// Build by toby <toby.chs@gmail.com> %s %s\n\
//===========================================================================\n\
\n", file_name_fun, __DATE__, __TIME__);

        file_name_fun[h_point] = '\0';
        strcpy(file_name_fun_copy, file_name_fun);

        fprintf(fph, "#ifndef __%s_H__\n", change(file_name_fun_copy, 0));
        fprintf(fph, "#define __%s_H__\n", change(file_name_fun_copy, 0));
        fprintf(fph, "\n#ifdef __cplusplus\n");
        fprintf(fph, "extern \"C\"\n");
        fprintf(fph, "{\n");
        fprintf(fph, "#endif\n\n");


        fprintf(fph, "extern int print_%s_html(void);\n", file_name_fun);


        fprintf(fph, "\n#ifdef __cplusplus\n");
        fprintf(fph, "}\n");
        fprintf(fph, "#endif\n\n");
        fprintf(fph, "#endif // __%s_H__\n\n", change(file_name_fun_copy, 0));

        fclose(fph);
        printf("-f SUCCEED\n");
    }



    /*
     * 关闭文件
     */
    fclose(fps);
    fclose(fpd);

    exit(0);
}

/**
 * Close the Doxygen group.
 * @}
 */

