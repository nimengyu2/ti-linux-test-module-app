/**
 * @file login.c
 * @brief 网页用户登录表单处理CGI
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
 * @date 2011-9-14 17:24:05
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

/**
 * @note
 * 本文件用于处理用户登入的表单处理，此网页为S2WIFI模 \n
 * 块的一部分。 \n
 *
 * \b Change \b Logs: \n
 * 2011.09.14   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_login_cgi
 * @{
 */

//#include "login.h"

// TODO: Add your code here
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <html2c.h>

#define LOGIN_CFG_FILE         "login.cfg"
#define LOGIN_CFG_FILE_ROW     10         // 配置文件最大的行号
#define LOGIN_CFG_FILE_COLUMN  100        // 配置文件最大的列号

/**
 * 从一个字符串中获取指定变量名的值
 *
 * @author Toby (2011-9-15)
 *
 * @param str_s 从配置文件中获取的字符串
 * @param str_str 变量名
 * @param str_d 输出变量值
 *
 * @return bool 是否正确输出变量，1:表示成功/-1:表示失败
 */
static
int get_value_form_string(const char *str_s,
                          const char *str_str,
                          char *str_d)
{
    int str_s_length = 0;
    int str_str_length = 0;
    int str_length_deta = 0;
    char *p;

    str_s_length = strlen(str_s)-1; // 需要把回车符去掉0x0A
    str_str_length = strlen(str_str);
    str_length_deta = str_s_length-str_str_length;

    //printf("DEBUG: str_s_length = %d\n", str_s_length);
    //printf("DEBUG: str_str_length = %d\n", str_str_length);
    p = strstr(str_s, str_str);
    if(p == NULL)
    {
        /*
        没有找到
        */
        return(-1);
    }
    /*
    偏移到变量值起始位
    */
    p += (str_str_length);

    /*
    程序到达这里，说明已经找到字符串
    */
    //printf("DEBUG: Found string = %s\n", p);
    if(str_length_deta > 10)
    {
        str_length_deta = 10;
    }
    /*
     * NOTE:不使用strcpy的原因是不把每行的回车符也复制进去
     */
    strncpy(str_d, p, str_length_deta);

    return(1);
}

/**
 * 获取CGI数据流的字符串值, 这里不启用POST方式
 *
 * @author Toby (2011-9-15)
 *
 * @param fp 从文件流中获取数据
 * @param request_method 表单提交的方法，GET or POST
 *
 * @return char* 返回CGI数据流字符串的首地址指针
 */
char* get_cgi_data(FILE* fp, char* request_method)
{
    char* input;
    int len;
    int size = 1024;
    int i = 0;

    if (!strcmp(request_method, "GET"))
    {
        input = getenv("QUERY_STRING");
        return input;
    }
    else if (!strcmp(request_method, "POST"))
    {
        len = atoi(getenv("CONTENT_LENGTH"));
        input = (char*)malloc(sizeof(char)*(size + 1));

        if (len == 0)
        {
            input[0] = '\0';
            return input;
        }

        while(1)
        {
            input[i] = (char)fgetc(fp);
            if (i == size)
            {
                input[i+1] = '\0';
                return input;
            }

            --len;
            if (feof(fp) || (!(len)))
            {
                i++;
                input[i] = '\0';
                return input;
            }
            i++;
        }
    }

    return NULL;
}

/**
 * Main Function
 *
 * @author toby.zhang (2011-9-14)
 *
 * @return int
 */
int main(void)
{
    int i;
    FILE *fp_login_cfg;
    int file_size;
    // UNIX换行LF，不支持DOS换行CR/LF
    char file_cfg_buf[LOGIN_CFG_FILE_ROW][LOGIN_CFG_FILE_COLUMN+1];
    /*
    用户名和密码最大长度为10，如果超出做大字符串将被
    截断
    */
    char vusername[10+1], cgi_vusername[10+1];
    char vpassword[10+1], cgi_vpassword[10+1];
    int is_login_in = 0; // 是否用户名和密码正确 0:不正确/1:正确

    /* CGI 部分变量 */
    char *cgi_req_method;
    char *cgi_input;
    int cgi_len, cgi_nel;
    char *cgi_q, *cgi_name, *cgi_value;
    //char cgi_buf[2*1024];

    /*
     * 内存初始化
     */
    memset(vusername, 0, sizeof(vusername));
    memset(vpassword, 0, sizeof(vpassword));
    memset(cgi_vusername, 0, sizeof(cgi_vusername));
    memset(cgi_vpassword, 0, sizeof(cgi_vpassword));

    /*
     * 以只读方式打开login.cfg配置文件
     */
    fp_login_cfg = fopen(LOGIN_CFG_FILE, "rb");
    if(fp_login_cfg == NULL)
    {
        /*
         * open file failure.
         */
        //printf("<br>open file failure!\n");
        exit(1);
    }

//  fseek(fp_login_cfg, 0, SEEK_END);  //移动到文件束的位置
//  file_size = ftell(fp_login_cfg);
//  printf("file size: %d\n", file_size);

//  fseek(fp_login_cfg, 0, SEEK_SET);
//  printf("%s", fgets(file_cfg_buf, LOGIN_CFG_FILE_COLUMN, fp_login_cfg));
//  file_size = ftell(fp_login_cfg);
//  printf("file size: %d\n", file_size);

    fseek(fp_login_cfg, 0, SEEK_SET);
    // 1 # S2WIFI Login file. Build by toby@2011.09.14
    fgets(file_cfg_buf[0], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notused
    // 2 # NOTE: There are no spaces between characters and characters.
    fgets(file_cfg_buf[1], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notused
    // 3 # !Begin!
    fgets(file_cfg_buf[2], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notused
    // 4 # LOGIN. Username, Max: 10 characters.
    fgets(file_cfg_buf[3], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notused
    // 5 vusername=admin
    fgets(file_cfg_buf[4], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // used
    get_value_form_string(file_cfg_buf[4], "vusername=", vusername);
    /*
    超出部分被截断
    */
    vusername[10] = '\0';
    // 6 # LOGIN. Password, Max: 10 characters.
    fgets(file_cfg_buf[5], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notused
    // 7 vpassword=1234567890
    fgets(file_cfg_buf[6], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // used
    get_value_form_string(file_cfg_buf[6], "vpassword=", vpassword);
    /*
    超出部分被截断
    */
    vpassword[10] = '\0';
    //printf("DEBUG: vusername = %s\n", vusername);
    //printf("DEBUG: vpassword = %s\n", vpassword);
    // 8 # 0:NotLogin/1:Login. default is 0
    fgets(file_cfg_buf[7], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notused
    // 9 vislogin=0
    fgets(file_cfg_buf[8], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // used
    // 10 # !End!
    fgets(file_cfg_buf[9], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notused

    fclose(fp_login_cfg);

    /*
     * 读取表单中用户输入的用户名和密码
     */
    //printf("Content-type: text/html\n\n");
    //printf("Hello CGI\n");
    //printf("<br>CGI[vusername ] :%s\n", vusername);
    //printf("<br>CGI[vpassword ] :%s\n", vpassword);
    cgi_req_method = getenv("REQUEST_METHOD");
    cgi_input = get_cgi_data(stdin, cgi_req_method);

    //printf("%s\n", cgi_input);
//  printf("strlen(cgi_input) = %d\n", strlen(cgi_input));
//  strncpy(cgi_buf, cgi_input, strlen(cgi_input));
//  cgi_buf[strlen(cgi_input)] = '\0';
//  printf("%s\n", cgi_buf);

    /* Parse into individualassignments */
    cgi_q = cgi_input;
    //printf("<br>CGI[query string] : %s\n", cgi_input);
    cgi_len = strlen(cgi_input);
    cgi_nel = 1;
    while (strsep(&cgi_q, "&"))
        cgi_nel++;

    /*
     * cgi_nel 应该为3才对
     */
    if(cgi_nel != 3)
    {
        /*
         * 参数非法
         */
        html_printf();
        exit(1);
    }

    //printf("<br>CGI[nel string] : %d\n", cgi_nel);
    for(cgi_q=cgi_input; cgi_q<(cgi_input + cgi_len);)
    {
        cgi_value = cgi_name = cgi_q;

        /* Skip to next assignment */
        //printf("<br>CGI[string] :%s\n", cgi_q);
        //printf("<br>[stringlen] : %d\n", strlen(cgi_q));
        //printf("<br>CGI[address] :%x\n", cgi_q);
        for(cgi_q+=strlen(cgi_q);
             cgi_q<(cgi_input+cgi_len) && !*cgi_q;
             cgi_q++);

        /* Assign variable */
        cgi_name = strsep(&cgi_value, "=");
        //printf("<br>CGI[name ] :%s\n", cgi_name);
        //printf("<br>CGI[value] :%s\n", cgi_value);
        if(strncmp(cgi_name, "username", sizeof("username")) == 0)
        {
            /* 找到有效的用户名变量，读取用户名 */
            if(strlen(cgi_value) > 10)
            {
                cgi_value[10] = '\0';
            }
            strcpy(cgi_vusername, cgi_value);
            cgi_vusername[10] = '\0'; // 只读取前10位
            //printf("<br>CGI[cgi_value ] :%s\n", cgi_vusername);
            //printf("<br>CGI[value ] :%s\n", cgi_value);
        }
        if(strncmp(cgi_name, "password", sizeof("password")) == 0)
        {
            /* 找到有效的密码变量，读取密码值 */
            if(strlen(cgi_value) > 10)
            {
                cgi_value[10] = '\0';
            }
            strcpy(cgi_vpassword, cgi_value);
            cgi_vpassword[10] = '\0';
            //printf("<br>CGI[cgi_value ] :%s\n", cgi_vpassword);
            //printf("<br>CGI[value] :%s\n", cgi_value);
        }
    }

    //printf("<br>strlen(vusername) = %d %s\n", strlen(vusername), vusername);
    //printf("<br>strlen(vpassword) = %d %s\n", strlen(vpassword), vpassword);

    //printf("<br>strlen(cgi_vusername) = %d\n", strlen(cgi_vusername));
    //printf("<br>strlen(cgi_vpassword) = %d\n", strlen(cgi_vpassword));

    if((strlen(vusername) == strlen(cgi_vusername)) &&
       (strlen(vpassword) == strlen(cgi_vpassword)))
    {
        if((strncmp(vusername, cgi_vusername, strlen(cgi_vusername))==0) &&
           (strncmp(vpassword, cgi_vpassword, strlen(cgi_vpassword))==0))
        {
            /* 用户名和密码都正确 */
            /* 把登入状态写入配置文件中 */
            strcpy(file_cfg_buf[8], "vislogin=1\n");
            is_login_in = 1;
            //printf("<br>CGI[vloginstat] : Login\n");
        }
        else
        {
            strcpy(file_cfg_buf[8], "vislogin=0\n");
            is_login_in = 0;
            //printf("<br>CGI[vloginstat] : notLogin1\n");
        }
    }
    else
    {
        strcpy(file_cfg_buf[8], "vislogin=0\n");
        is_login_in = 0;
        //printf("<br>CGI[vloginstat] : notLogin2\n");
    }

    /*
     * 以可读写方式打开login.cfg配置文件
     * 配置文件已存在
     */
    fp_login_cfg = fopen(LOGIN_CFG_FILE, "r+b");
    if(fp_login_cfg == NULL)
    {
        /*
         * open file failure.
         */
        //printf("<br>open file failure!\n");
        exit(1);
    }

    /* 写入文件 */
    for(i=0; i<LOGIN_CFG_FILE_ROW; i++)
    {
        fputs(file_cfg_buf[i], fp_login_cfg);
    }

    fclose(fp_login_cfg);

    /* 页面刷新 */
    html_printf();
//  if(is_login_in == 1)
//  {
//      printf("<br>CGI[vloginstat] : Login\n");
//  }
//  else
//  {
//      printf("<br>CGI[vloginstat] : notLogin\n");
//  }

    return(0);
}

/**
 * Close the Doxygen group.
 * @}
 */

