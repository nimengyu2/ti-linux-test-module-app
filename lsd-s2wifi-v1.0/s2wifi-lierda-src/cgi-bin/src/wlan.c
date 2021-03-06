/**
 * @file serial.c
 * @brief WLAN配置参数CGI
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
 * @date 2011-9-21 17:21:12
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

/**
 * @note
 * WLAN配置CGI \n
 *
 * \b Change \b Logs: \n
 * 2011.09.19   创建初始 by toby \n
 * 2011.09.21   copy from serial.c by toby \n
 * 2011.09.21   copy form wifi.c by toby \n
 *
 */

/**
 * @addtogroup api_wlan_cgi
 * @{
 */

//#include "wlan.h"
#include <html2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mtd_ops.h"

// TODO: Add your code here
// 登录配置文件
#define LOGIN_CFG_FILE           "login.cfg"
#define LOGIN_CFG_FILE_ROW       10         // 配置文件最大的行号
#define LOGIN_CFG_FILE_COLUMN    100        // 配置文件最大的列号
#define LOGIN_CFG_VUSERNAME      (5-1)
#define LOGIN_CFG_VPASSWORD      (7-1)
#define LOGIN_CFG_VISLOGIN       (9-1)

// 核心配置文件
#define S2WIFI_CFG_FILE          "s2wifi.cfg"
#define S2WIFI_CFG_FILE_ROW      41
#define S2WIFI_CFG_FILE_COLUMN   100
#define S2WIFI_CFG_VSSID         (5-1)
#define S2WIFI_CFG_VENCRYTYPE    (7-1)
#define S2WIFI_CFG_VWIFIPASSWORD (9-1)
#define S2WIFI_CFG_VSTATICIP     (11-1)
#define S2WIFI_CFG_VWLANSIP      (13-1)
#define S2WIFI_CFG_VWLANMIP      (15-1)
#define S2WIFI_CFG_VWLANGIP      (17-1)
#define S2WIFI_CFG_VWLANDIP      (19-1)
#define S2WIFI_CFG_VSOCKETLP     (21-1)
#define S2WIFI_CFG_VSOCKETRP     (23-1)
#define S2WIFI_CFG_VSOCKETMODE   (25-1)
#define S2WIFI_CFG_VSOCKETPROL   (27-1)
#define S2WIFI_CFG_VSOCKETRIP    (29-1)
#define S2WIFI_CFG_VBAUDRATE     (32-1)
#define S2WIFI_CFG_VDATASIZE     (34-1)
#define S2WIFI_CFG_VPARITY       (36-1)
#define S2WIFI_CFG_VSTOP         (38-1)
#define S2WIFI_CFG_VFLOW         (40-1)


#define WLAN_CGI_DEBUG                     0

/**
 * 参数配置表结构体
 *
 * @author Toby (2011-9-19)
 */
struct param_table_cfg
{
    char index; //! 参数索引
    char *param; //! 参数
    char *buf; //! 参数值缓存
    char size; //! 缓存大小
    int flag; //! 0:无效/1:有效
    int (* param_valid_proc)(void *arg); //! 参数合法性测试
};
#define TABLE_SIZE_MAX                  20

/**
 * 从一个字符串中获取指定变量名的值
 *
 * @author Toby (2011-9-15)
 *
 * @param str_s 从配置文件中获取的字符串
 * @param str_str 变量名
 * @param str_d 输出变量值
 * @param size str_d的存储空间大小
 *
 * @return bool 是否正确输出变量，1:表示成功/-1:表示失败
 */
static
int get_value_form_string(const char *str_s,
                          const char *str_str,
                          char *str_d, int size)
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

    if(str_length_deta > size)
    {
        str_length_deta = size;
    }
    /*
     * NOTE:不使用strcpy的原因是不把每行的回车符也复制进去
     */
    strncpy(str_d, p, str_length_deta);

    return(1);
}

/**
 * 在参数列表中查询，并做相应的记录
 *
 * @author Toby (2011-9-19)
 *
 * @param table 参数表，预先设置好
 * @param table_size 参数表的大小
 * @param pfind_str 需要查询的参数
 * @param pvalue 需要存储的参数值
 *
 * @return int 1:查询成功/-1:查询失败
 */
static
int find_param(struct param_table_cfg *ptable,
               int table_size,
               char *pfind_str,
               char *pvaule)
{
    int i, ret = 0;
    int err = 0;
    int index; // 参数索引标记
    int is_find = 0; // 参数是否找到 0:未找到/1:找到

    for(i=0; i<table_size; i++)
    {
        /*
         * 参数比对，参数比较少，未采用优化算法
         * 1. 字符长度一样
         * 2. 各个字符一样
         */
        if((strncmp(ptable[i].param, pfind_str, strlen(ptable[i].param)) == 0)
           && (strlen(ptable[i].param) == strlen(pfind_str)))
        {
            /*
             * 参数已找到
             * 标记参数已找到，并存储参数值到缓存中
             */
            if(ptable[i].flag == 1)
            {
                err = 1; // 参数重复，退出
                break;
            }
            else
            {
                /*
                 * 参数已找到，进行参数合法性测试
                 */
                strncpy(ptable[i].buf, pvaule, ptable[i].size);
                if(ptable[i].param_valid_proc != NULL)
                {
                    /*
                     * 已定义参数合法性测试函数，调用之
                     */
                    ret = ptable[i].param_valid_proc(ptable[i].buf);
                    if(ret == -1)
                    {
                        /*
                         * 参数不合法
                         */
                        err = 1;
                        break;
                    }

                    /*
                     * 程序到此，说明参数已找到，并且参数也是
                     * 合法的
                     */
                    ptable[i].flag = 1;
                }
                break;
            }
        }
    }

    if((i < table_size) && (err != 1))
    {
        /*
         * 参数已找到
         */
        return(1);
    }
    else
    {
        /*
         * 参数未找到或者是参数重复
         */
        return(-1);
    }
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

/*
 * 参数合法性测试小程序
 */

/**
 * 范围比较函数，包含min和max值
 *
 * @author Toby (2011-9-21)
 *
 * @param val 需要比较的值
 * @param min 范围最小值
 * @param max 范围最大值
 *
 * @return int 1:在此范围中/-1:不再指定的范围中
 */
static
int is_in_range(int val, int min, int max)
{
    if((val >= min) && (val <= max))
    {
        /*
         * 在指定范围中
         */
        return(1);
    }
    else
    {
        return(-1);
    }

}

/**
 * WIFI SSID检测
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int 1:有效/-1:无效
 */
int ssid_valid(void *arg)
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
        return(1);
    }

    return(-1);
}

/**
 * WIFI 加密方式检测
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int 1:有效/-1:无效
 */
int encryption_type_valid(void *arg)
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
            return(-1);
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
            return(1);
            break;
        }
        default:
        {
            return(-1);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    //return(1);
}

/**
 * WIFI密码格式检测
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int 1:有效/-1:无效
 */
int wifi_password_valid(void *arg)
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
        return(1);
    }

    return(-1);
}

/**
 * wifi ip地址获取方式
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int 1:有效/-1:无效
 */
int address_type_valid(void *arg)
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
            return(-1);
        }
    }

    /*
     * 是数字
     */
    p = arg;
    address_type = atoi(p);

    // 0:None/1:WEP-open/2:WEP-share/3:WPA or WPA2
    switch(address_type)
    {
        case 0:
        case 1:
        {
            return(1);
            break;
        }
        default:
        {
            return(-1);
            break;
        }
    }

    /*
     * 程序不可能执行到这里
     */
    //return(1);
}

/**
 * IP地址是否有效检测
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int 1:有效/-1:无效
 */
static
int is_ip_valid(void *arg)
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
#if WLAN_CGI_DEBUG
    printf("<br> is_ip_vaild-arg = %s\n", p);
#endif

    /*
     * 为了不让strsep损坏原始数据，必须把原始数据备份
     */
    memset(arg_tmp, 0, sizeof(arg_tmp));
    strncpy(arg_tmp, p, strlen(p));
    p = arg_tmp;
#if WLAN_CGI_DEBUG
    printf("<br> is_ip_vaild-arg_tmp = %s\n", p);
#endif

    //memset(__ip, 0, sizeof(__ip));

    while((tmp = strsep(&p, ".")) != NULL)
    {
#if WLAN_CGI_DEBUG
        printf("<br> tmp = %s, p = %s\n",tmp, p);
#endif

        if(count > 3)
        {
            /*
             * 发生IP格式错误
             */
            return(-1);
        }

        if((strlen(tmp) > 3) && (strlen(tmp) < 1))
        {
            /*
             * 发生IP格式错误
             */
            return(-1);
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

#if WLAN_CGI_DEBUG
        printf("<br> len = %d\n", len);
        printf("<br> p = %s\n", p);
#endif
        for(i=0; i<len; i++)
        {
            if(isdigit(*p++) == 0)
            {
                /*
                 * 非数字值
                 */
                return(-1);
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

    if(is_in_range(_ip[0], 1, 223) == -1)
        return(-1);
    if(_ip[0] == 127)
        return(-1);
    if(is_in_range(_ip[1], 0, 255) == -1)
        return(-1);
    if(is_in_range(_ip[2], 0, 255) == -1)
        return(-1);
    if(is_in_range(_ip[3], 1, 254) == -1)
        return(-1);

    /*
     * 程序不可能执行到这里
     */
    //return(1);
}

/**
 * 检测是否为一个合法的掩码
 *
 * @author Toby (2011-9-21)
 *
 * @param d 需要检测的值
 *
 * @return int 1:有效/-1:无效
 */
static
int __is_mask(int d)
{
    if((d==0) || (d==128) ||
       (d==192) || (d==224) ||
       (d==240) || (d==248) ||
       (d==252) || (d==254) ||
       (d==255)
       )
    {
        return(1);
    }

    return(-1);
}

/**
 * netmask地址是否有效检测
 *
 * @author Toby (2011-9-21)
 *
 * @param arg 需要检测的参数值
 *
 * @return int 1:有效/-1:无效
 */
static
int is_netmask_valid(void *arg)
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
#if WLAN_CGI_DEBUG
    printf("<br> is_netmask_valid-arg_tmp = %s\n", p);
#endif

    while((tmp = strsep(&p, ".")) != NULL)
    {
        if(count > 3)
        {
            /*
             * 发生IP格式错误
             */
            return(-1);
        }

        if((strlen(tmp) > 3) && (strlen(tmp) < 1))
        {
            /*
             * 发生IP格式错误
             */
            return(-1);
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
                return(-1);
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
#if WLAN_CGI_DEBUG
    for(i=0; i<4; i++)
        printf("<br> _ip[%d] = %d\n", i, _ip[i]);
#endif

    if(__is_mask(_ip[0]) == -1)
        return(-1);
    if(_ip[0] == 0)
        return(-1);
    if(_ip[0] != 255)
    {
        if((_ip[1]==0) && (_ip[2]==0) && (_ip[3]==0))
            return(1);
        return(-1);
    }

    if(__is_mask(_ip[1]) == -1)
        return(-1);
    if(_ip[1] != 255)
    {
        if((_ip[2]==0) && (_ip[3]==0))
            return(1);
        return(-1);
    }

    if(__is_mask(_ip[2]) == -1)
        return(-1);
    if(_ip[2] != 255)
    {
        if(_ip[3] == 0)
            return(1);
        return(-1);
    }

    if(__is_mask(_ip[3]) == -1)
        return(-1);
    return(1);

    /*
     * 程序不可能执行到这里
     */
    //return(1);
}

/**
 * Main Function
 *
 * @author Toby (2011-9-19)
 *
 * @return int
 */
int main(void)
{
    int i, ret;

    FILE *fp_login_cfg, *fp_s2wifi_cfg;
    // UNIX换行LF，不支持DOS换行CR/LF
    // 宏已定义
    char file_login_cfg_buf[LOGIN_CFG_FILE_ROW][LOGIN_CFG_FILE_COLUMN+1];
    char file_s2wifi_cfg_buf[S2WIFI_CFG_FILE_ROW][S2WIFI_CFG_FILE_COLUMN+1];

    int user_islogin = 0;

    /* CGI 部分变量 */
    char *cgi_req_method;
    char *cgi_input;
    int cgi_len, cgi_nel;
    char *cgi_q, *cgi_name, *cgi_value;

    /*
     * 字符变量区
     */
    /* login.cfg */
/*01*/char vusername[10+1] = {0}, cgi_vusername[10+1] = {0}; // 用户名
/*02*/char vpassword[10+1] = {0}, cgi_vpassword[10+1] = {0}; // 密码
      char vislogin[1+1] = {0}, cgi_vislogin[1+1] = {0}; //用户是否已登入

    /* s2wifi.cfg */
/*03*/char vssid[32+1] = {0}, cgi_vssid[32+1] = {0}; // SSID
/*04*/char vencrytype[1+1] = {0}, cgi_vencrytype[1+1] = {0}; // Encryption type
/*05*/char vwifipassword[63+1] = {0};
           char cgi_vwifipassword[63+1] = {0}; // WIFI passworld
/*06*/char vstaticip[1+1] = {0}, cgi_vstaticip[1+1] = {0}; // Address type
/*07*/char vwlansip[15+1] = {0}, cgi_vwlansip[15+1] = {0}; // Static IP address
/*08*/char vwlanmip[15+1] = {0}, cgi_vwlanmip[15+1] = {0}; // Subnet Mask
/*09*/char vwlangip[15+1] = {0}, cgi_vwlangip[15+1] = {0}; // Default Gateway
/*10*/char vwlandip[15+1] = {0}, cgi_vwlandip[15+1] = {0}; // DNS Server
/*11*/char vsocketlp[5+1] = {0};
           char cgi_vsocketlp[5+1] = {0}; // Local Socket Port Number
/*12*/char vsocketrp[5+1] = {0};
           char cgi_vsocketrp[5+1] = {0}; // Remote Socket Port Numbe
/*13*/char vsocketmode[1+1] = {0}, cgi_vsocketmode[1+1] = {0}; // Socket Mode
/*14*/char vsocketprol[1+1] = {0}, cgi_vsocketprol[1+1] = {0}; // Socket Protoco
/*15*/char vsocketrip[15+1] = {0}, cgi_vsocketrip[15+1] = {0}; // Remote Server
/*16*/char vbaudrate[6+1] = {0}, cgi_vbaudrate[6+1] = {0}; // Baud Rate
/*17*/char vdatasize[1+1] = {0}, cgi_vdatasize[1+1] = {0}; // Data Size
/*18*/char vparity[1+1] = {0}, cgi_vparity[1+1] = {0}; // Parity
/*19*/char vstop[1+1] = {0}, cgi_vstop[1+1] = {0}; // Stop Bits
/*20*/char vflow[1+1] = {0}, cgi_vflow[1+1] = {0}; // Flow Control

    /* CGI Table */
    struct param_table_cfg cgi_table[TABLE_SIZE_MAX] =
    {
        {
            1-1, "username", cgi_vusername, sizeof(cgi_vusername), 0,
            NULL
        },
        {
            2-1, "password", cgi_vpassword, sizeof(cgi_vpassword), 0,
            NULL
        },
        {
            3-1, "ssid", cgi_vssid, sizeof(cgi_vssid), 0,
            ssid_valid
        },
        {
            4-1, "encrytype", cgi_vencrytype, sizeof(cgi_vencrytype), 0,
            encryption_type_valid
        },
        {
            5-1, "wifipassword", cgi_vwifipassword, sizeof(cgi_vwifipassword),
            0,
            wifi_password_valid
        },
        {
            6-1, "staticip", cgi_vstaticip, sizeof(cgi_vstaticip), 0,
            address_type_valid
        },
        {
            7-1, "wlansip", cgi_vwlansip, sizeof(cgi_vwlansip), 0,
            is_ip_valid
        },
        {
            8-1, "wlanmip", cgi_vwlanmip, sizeof(cgi_vwlanmip), 0,
            is_netmask_valid
        },
        {
            9-1, "wlangip", cgi_vwlangip, sizeof(cgi_vwlangip), 0,
            is_ip_valid
        },
        {
            10-1, "wlandip", cgi_vwlandip, sizeof(cgi_vwlandip), 0,
            is_ip_valid
        },
        {
            11-1, "socketlp", cgi_vsocketlp, sizeof(cgi_vsocketlp), 0,
            NULL
        },
        {
            12-1, "socketrp", cgi_vsocketrp, sizeof(cgi_vsocketrp), 0,
            NULL
        },
        {
            13-1, "socketmode", cgi_vsocketmode, sizeof(cgi_vsocketmode), 0,
            NULL
        },
        {
            14-1, "socketprol", cgi_vsocketprol, sizeof(cgi_vsocketprol), 0,
            NULL
        },
        {
            15-1, "socketrip", cgi_vsocketrip, sizeof(cgi_vsocketrip), 0,
            NULL
        },
        {
            16-1, "baudrate", cgi_vbaudrate, sizeof(cgi_vbaudrate), 0,
            NULL
        },
        {
            17-1, "datasize", cgi_vdatasize, sizeof(cgi_vdatasize), 0,
            NULL
        },
        {
            18-1, "parity", cgi_vparity, sizeof(cgi_vparity), 0,
            NULL
        },
        {
            19-1, "stop", cgi_vstop, sizeof(cgi_vstop), 0,
            NULL
        },
        {
            20-1, "flow", cgi_vflow, sizeof(cgi_vflow), 0,
            NULL
        },
    };

    /*
     * 读取配置值=====================================================>>>>>
     */

    /*
     * 以只读方式打开login.cfg配置文件
     */
    fp_login_cfg = fopen(LOGIN_CFG_FILE, "rb");
    if(fp_login_cfg == NULL)
    {
        html_printf();
        return(1);
    }
    fseek(fp_login_cfg, 0, SEEK_SET);
    // 1 # S2WIFI Login file. Build by toby@2011.09.14
    fgets(file_login_cfg_buf[0], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notuse
    // 2 # NOTE: There are no spaces between characters and characters.
    fgets(file_login_cfg_buf[1], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notuse
    // 3 # !Begin!
    fgets(file_login_cfg_buf[2], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notuse
    // 4 # LOGIN. Username, Max: 10 characters.
    fgets(file_login_cfg_buf[3], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notuse
    // 5 vusername=admin
    fgets(file_login_cfg_buf[4], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // used
    get_value_form_string(file_login_cfg_buf[4], "vusername=", vusername,
                          sizeof(vusername)-1);
    vusername[10] = '\0';
    // 6 # LOGIN. Password, Max: 10 characters.
    fgets(file_login_cfg_buf[5], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notuse
    // 7 vpassword=1234567890
    fgets(file_login_cfg_buf[6], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // used
    get_value_form_string(file_login_cfg_buf[6], "vpassword=", vpassword,
                          sizeof(vpassword)-1);
    vpassword[10] = '\0';
    // 8 # 0:NotLogin/1:Login. default is 0
    fgets(file_login_cfg_buf[7], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notuse
    // 9 vislogin=0
    fgets(file_login_cfg_buf[8], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // used
    get_value_form_string(file_login_cfg_buf[8], "vislogin=", vislogin,
                          sizeof(vislogin)-1);
    vislogin[strlen(vislogin)] = '\0';
    if(vislogin[0] == '1')
    {
        user_islogin = 1;
    }
    else
    {
        user_islogin = 0;
    }
    // 10 # !End!
    fgets(file_login_cfg_buf[9], LOGIN_CFG_FILE_COLUMN, fp_login_cfg); // notuse
    fclose(fp_login_cfg);

    /*
     * 以只读方式打开s2wifi.cfg配置文件
     */
    fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "rb");
    if(fp_s2wifi_cfg == NULL)
    {
        html_printf();
        exit(1);
    }
    fseek(fp_s2wifi_cfg, 0, SEEK_SET);
    // 1 # S2WIFI Config file. Build by toby@2011.09.14
    fgets(file_s2wifi_cfg_buf[0], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    // 2 # NOTE: There are no spaces between characters and characters.
    fgets(file_s2wifi_cfg_buf[1], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    // 3 # !Begin!
    fgets(file_s2wifi_cfg_buf[2], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    // 4 # WLAN. SSID, Max: 32 characters. e.g: TEST-15.
    fgets(file_s2wifi_cfg_buf[3], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    // 5 vssid=TEST-15
/**/fgets(file_s2wifi_cfg_buf[4], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[4], "vssid=", vssid,
                          sizeof(vssid)-1);
    vssid[strlen(vssid)] = '\0';
    // 6 # WLAN. Encryption type. 0:None/1:WEP-open/2:WEP-share/3:WPA or WPA2.
    fgets(file_s2wifi_cfg_buf[5], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    // 7 vencrytype=3
/**/fgets(file_s2wifi_cfg_buf[6], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[6], "vencrytype=", vencrytype,
                          sizeof(vencrytype)-1);
    vencrytype[strlen(vencrytype)] = '\0';
    // 8 # WLAN. WIFI password. Max: 63 characters. WPA or WPA2 Min:
    //     8 characters.
    fgets(file_s2wifi_cfg_buf[7], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    // 9 vwifipassword=zhangxiaojie
/**/fgets(file_s2wifi_cfg_buf[8], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[8], "vwifipassword=",
                          vwifipassword, sizeof(vwifipassword)-1);
    vwifipassword[strlen(vwifipassword)] = '\0';
    //10 # WLAN. Address type. 0:DHCP/1:StaticIP.
    fgets(file_s2wifi_cfg_buf[9], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //11 vstaticip=1
/**/fgets(file_s2wifi_cfg_buf[10], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[10], "vstaticip=", vstaticip,
                          sizeof(vstaticip)-1);
    vstaticip[strlen(vstaticip)] = '\0';
    //12 # WLAN. Static IP address. e.g:192.168.1.80.
    fgets(file_s2wifi_cfg_buf[11], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //13 vwlansip=192.168.1.80
/**/fgets(file_s2wifi_cfg_buf[12], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[12], "vwlansip=", vwlansip,
                          sizeof(vwlansip)-1);
    vwlansip[strlen(vwlansip)] = '\0';
    //14 # WLAN. Subnet Mask. e.g:255.255.255.0.
    fgets(file_s2wifi_cfg_buf[13], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //15 vwlanmip=255.255.255.0
/**/fgets(file_s2wifi_cfg_buf[14], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[14], "vwlanmip=", vwlanmip,
                          sizeof(vwlanmip)-1);
    vwlanmip[strlen(vwlanmip)] = '\0';
    //16 # WLAN. Default Gateway. e.g:192.168.1.1.
    fgets(file_s2wifi_cfg_buf[15], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //17 vwlangip=192.168.1.1
/**/fgets(file_s2wifi_cfg_buf[16], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[16], "vwlangip=", vwlangip,
                          sizeof(vwlangip)-1);
    vwlangip[strlen(vwlangip)] = '\0';
    //18 # WLAN. DNS Server. e.g:192.168.1.1.
    fgets(file_s2wifi_cfg_buf[17], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //19 vwlandip=192.168.1.1
/**/fgets(file_s2wifi_cfg_buf[18], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[18], "vwlandip=", vwlandip,
                          sizeof(vwlandip)-1);
    vwlandip[strlen(vwlandip)] = '\0';
    //20 # WIFI. Local Socket Port Number. (1, 65535).
    fgets(file_s2wifi_cfg_buf[19], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //21 vsocketlp=1234
/**/fgets(file_s2wifi_cfg_buf[20], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[20], "vsocketlp=", vsocketlp,
                          sizeof(vsocketlp)-1);
    vsocketlp[strlen(vsocketlp)] = '\0';
    //22 # WIFI. Remote Socket Port Number. (1, 65535).
    fgets(file_s2wifi_cfg_buf[21], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //23 vsocketrp=5689
/**/fgets(file_s2wifi_cfg_buf[22], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[22], "vsocketrp=", vsocketrp,
                          sizeof(vsocketrp)-1);
    vsocketrp[strlen(vsocketrp)] = '\0';
    //24 # WIFI. Socket Mode. 0:Server/1:Client.
    fgets(file_s2wifi_cfg_buf[23], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //25 vsocketmode=0
/**/fgets(file_s2wifi_cfg_buf[24], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[24], "vsocketmode=", vsocketmode,
                          sizeof(vsocketmode)-1);
    vsocketmode[strlen(vsocketmode)] = '\0';
    //26 # WIFI. Socket Protocol. 0:TCP/1:UDP.
    fgets(file_s2wifi_cfg_buf[25], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //27 vsocketprol=0
/**/fgets(file_s2wifi_cfg_buf[26], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[26], "vsocketprol=", vsocketprol,
                          sizeof(vsocketprol)-1);
    vsocketprol[strlen(vsocketprol)] = '\0';
    //28 # WIFI. Remote Server IP. e.g:192.168.1.100.
    fgets(file_s2wifi_cfg_buf[27], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //29 vsocketrip=192.168.1.100
/**/fgets(file_s2wifi_cfg_buf[28], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[28], "vsocketrip=", vsocketrip,
                          sizeof(vsocketrip)-1);
    vsocketrip[strlen(vsocketrip)] = '\0';
    //30 # SERIAL. Baud Rate. 110/300/600/1200/2400/4800/9600/14400/19200/38400/
    fgets(file_s2wifi_cfg_buf[29], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //31 # 57600/115200/230400.
    fgets(file_s2wifi_cfg_buf[30], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //32 vbaudrate=115200
/**/fgets(file_s2wifi_cfg_buf[31], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[31], "vbaudrate=", vbaudrate,
                          sizeof(vbaudrate)-1);
    vbaudrate[strlen(vbaudrate)] = '\0';
    //33 # SERIAL. Data Size. 5/6/7/8
    fgets(file_s2wifi_cfg_buf[32], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //34 vdatasize=8
/**/fgets(file_s2wifi_cfg_buf[33], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[33], "vdatasize=", vdatasize,
                          sizeof(vdatasize)-1);
    vdatasize[strlen(vdatasize)] = '\0';
    //35 # SERIAL. Parity. 0:None/1:Odd/2:Even/3:Mark/4:Space.
    fgets(file_s2wifi_cfg_buf[34], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //36 vparity=0
/**/fgets(file_s2wifi_cfg_buf[35], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[35], "vparity=", vparity,
                          sizeof(vparity)-1);
    vparity[strlen(vparity)] = '\0';
    //37 # SERIAL. Stop Bits. 1/2.
    fgets(file_s2wifi_cfg_buf[36], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //38 vstop=1
/**/fgets(file_s2wifi_cfg_buf[37], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[37], "vstop=", vstop,
                          sizeof(vstop)-1);
    vstop[strlen(vstop)] = '\0';
    //39 # SERIAL. Flow Control. 0:None/1:Hardware.
    fgets(file_s2wifi_cfg_buf[38], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    //40 vflow=0
/**/fgets(file_s2wifi_cfg_buf[39], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // used
    get_value_form_string(file_s2wifi_cfg_buf[39], "vflow=", vflow,
                          sizeof(vflow)-1);
    vflow[strlen(vflow)] = '\0';
    //41 # !End!
    fgets(file_s2wifi_cfg_buf[40], LOGIN_CFG_FILE_COLUMN,
          fp_s2wifi_cfg); // notused
    fclose(fp_s2wifi_cfg);

    /*
     * 参数读取完毕==================================================<<<<<
     */

    /*
     * 读取表单中用户输入的用户名和密码
     */
#if WLAN_CGI_DEBUG
    printf("Content-type: text/html\n\n");
#endif
    cgi_req_method = getenv("REQUEST_METHOD");
    cgi_input = get_cgi_data(stdin, cgi_req_method);

#if WLAN_CGI_DEBUG
    printf("%s\n", cgi_input);
#endif

    /* Parse into individualassignments */
    cgi_q = cgi_input;
    //printf("<br>CGI[query string] : %s\n", cgi_input);
    cgi_len = strlen(cgi_input);
    cgi_nel = 1;
    while (strsep(&cgi_q, "&"))
        cgi_nel++;

#if WLAN_CGI_DEBUG
    printf("<br> cgi_nel = %d\n", cgi_nel);
#endif

    /*
     * cgi_nel
     */
    if((cgi_nel == 4) || (cgi_nel == 5) ||
       (cgi_nel == 8) || (cgi_nel == 9))
    {
        // do nothing;
    }
    else
    {
        /*
         * 参数非法
         */
        // 此错误信息提示，可根据具体需要修改
        // 这里只输出当前的配置信息，如果配置错误参数将不
        // 会改变
        html_printf();
        exit(1);
    }

#if WLAN_CGI_DEBUG
    printf("<br> CGI PROC START\n");
#endif
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

        /*
         * 在这里需要添加一个参数搜索函数
         *
         * 用于比对参数值和参数合法性检测，一旦检测到参
         * 数不合法，或者参数缺失，都将强制退出程序运行
         */
        ret = find_param(cgi_table, TABLE_SIZE_MAX, cgi_name, cgi_value);
#if WLAN_CGI_DEBUG
    printf("<br> find_param = %d\n", ret);
#endif
        if(ret == -1)
        {
            /*
             * 遇到错误，需要退出
             */
            html_printf();
            exit(1);
        }

//      if(strncmp(cgi_name, "baudrate", sizeof("baudrate")) == 0)
//      {
//          /* 找到有效的用户名变量，读取用户名 */
//          if(strlen(cgi_value) > 6)
//          {
//              cgi_value[6] = '\0';
//          }
//          strcpy(cgi_baudrate, cgi_value);
//          cgi_vusername[10] = '\0'; // 只读取前10位
//          //printf("<br>CGI[cgi_value ] :%s\n", cgi_vusername);
//          //printf("<br>CGI[value ] :%s\n", cgi_value);
//      }
//      if(strncmp(cgi_name, "password", sizeof("password")) == 0)
//      {
//          /* 找到有效的密码变量，读取密码值 */
//          if(strlen(cgi_value) > 10)
//          {
//              cgi_value[10] = '\0';
//          }
//          strcpy(cgi_vpassword, cgi_value);
//          cgi_vpassword[10] = '\0';
//          //printf("<br>CGI[cgi_value ] :%s\n", cgi_vpassword);
//          //printf("<br>CGI[value] :%s\n", cgi_value);
//      }
    }

    /*
     * 程序执行到这里，说明所有操作都是成功的
     * 需要把新的配置值存储到配置文件中
     */

    /*
     * 在参数存储前，需要检查所需要的参数是否完备
     * 1. SSID                   3-1
     * 2. Encryption type        4-1
     * 3. WIFI passworld         5-1
     * 4. Address type           6-1
     * 5. Static IP address      7-1
     * 6. Subnet Mask            8-1
     * 7. Default Gateway        9-1
     * 8. DNS Server            10-1
     */
    if(cgi_table[4-1].buf[0] == '0') // Encryption type: None
    {
        if(cgi_table[6-1].buf[0] == '0') // DHCP
        {
            if((cgi_table[3-1].flag == 1) &&
               (cgi_table[4-1].flag == 1) &&
               //(cgi_table[5-1].flag == 1) &&
               (cgi_table[6-1].flag == 1) //&&
               //(cgi_table[7-1].flag == 1) &&
               //(cgi_table[8-1].flag == 1) &&
               //(cgi_table[9-1].flag == 1) &&
               //(cgi_table[10-1].flag == 1)
               )
            {
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE]));
//  memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD], 0,
//         sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP]));
//  memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP], 0,
//         sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP]));
//  memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP], 0,
//         sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP]));
//  memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP], 0,
//         sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP]));
//  memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP], 0,
//         sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP]));

    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID], "vssid=%s\n",
            cgi_table[3-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE], "vencrytype=%s\n",
            cgi_table[4-1].buf);
//  sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD], "vwifipassword=%s\n",
//          cgi_table[5-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP], "vstaticip=%s\n",
            cgi_table[6-1].buf);
//  sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP], "vwlansip=%s\n",
//          cgi_table[7-1].buf);
//  sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP], "vwlanmip=%s\n",
//          cgi_table[8-1].buf);
//  sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP], "vwlangip=%s\n",
//          cgi_table[9-1].buf);
//  sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP], "vwlandip=%s\n",
//          cgi_table[10-1].buf);
            }
            else
            {
                html_printf();
                exit(1);
            }
        }
        else // Static IP
        {
            if((cgi_table[3-1].flag == 1) &&
               (cgi_table[4-1].flag == 1) &&
               //(cgi_table[5-1].flag == 1) &&
               (cgi_table[6-1].flag == 1) &&
               (cgi_table[7-1].flag == 1) &&
               (cgi_table[8-1].flag == 1) &&
               (cgi_table[9-1].flag == 1) &&
               (cgi_table[10-1].flag == 1)
               )
            {
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE]));
//  memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD], 0,
//         sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP]));

    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID], "vssid=%s\n",
            cgi_table[3-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE], "vencrytype=%s\n",
            cgi_table[4-1].buf);
//  sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD], "vwifipassword=%s\n",
//          cgi_table[5-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP], "vstaticip=%s\n",
            cgi_table[6-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP], "vwlansip=%s\n",
            cgi_table[7-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP], "vwlanmip=%s\n",
            cgi_table[8-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP], "vwlangip=%s\n",
            cgi_table[9-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP], "vwlandip=%s\n",
            cgi_table[10-1].buf);
            }
            else
            {
                html_printf();
                exit(1);
            }
        }

    }
    else // Encryption type: WEP-open/WEP-share/WPA or WPA2
    {
        if(cgi_table[6-1].buf[0] == '0') // DHCP
        {
            if((cgi_table[3-1].flag == 1) &&
               (cgi_table[4-1].flag == 1) &&
               (cgi_table[5-1].flag == 1) &&
               (cgi_table[6-1].flag == 1) //&&
               //(cgi_table[7-1].flag == 1) &&
               //(cgi_table[8-1].flag == 1) &&
               //(cgi_table[9-1].flag == 1) &&
               //(cgi_table[10-1].flag == 1)
               )
            {
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP]));
//  memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP], 0,
//         sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP]));
//  memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP], 0,
//         sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP]));
//  memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP], 0,
//         sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP]));
//  memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP], 0,
//         sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP]));

    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID], "vssid=%s\n",
            cgi_table[3-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE], "vencrytype=%s\n",
            cgi_table[4-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD], "vwifipassword=%s\n",
            cgi_table[5-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP], "vstaticip=%s\n",
            cgi_table[6-1].buf);
//  sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP], "vwlansip=%s\n",
//          cgi_table[7-1].buf);
//  sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP], "vwlanmip=%s\n",
//          cgi_table[8-1].buf);
//  sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP], "vwlangip=%s\n",
//          cgi_table[9-1].buf);
//  sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP], "vwlandip=%s\n",
//          cgi_table[10-1].buf);
            }
            else
            {
                html_printf();
                exit(1);
            }
        }
        else // Static IP
        {
            if((cgi_table[3-1].flag == 1) &&
               (cgi_table[4-1].flag == 1) &&
               (cgi_table[5-1].flag == 1) &&
               (cgi_table[6-1].flag == 1) &&
               (cgi_table[7-1].flag == 1) &&
               (cgi_table[8-1].flag == 1) &&
               (cgi_table[9-1].flag == 1) &&
               (cgi_table[10-1].flag == 1)
               )
            {
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP]));
    memset(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP], 0,
           sizeof(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP]));

    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VSSID], "vssid=%s\n",
            cgi_table[3-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VENCRYTYPE], "vencrytype=%s\n",
            cgi_table[4-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWIFIPASSWORD], "vwifipassword=%s\n",
            cgi_table[5-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VSTATICIP], "vstaticip=%s\n",
            cgi_table[6-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANSIP], "vwlansip=%s\n",
            cgi_table[7-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANMIP], "vwlanmip=%s\n",
            cgi_table[8-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANGIP], "vwlangip=%s\n",
            cgi_table[9-1].buf);
    sprintf(file_s2wifi_cfg_buf[S2WIFI_CFG_VWLANDIP], "vwlandip=%s\n",
            cgi_table[10-1].buf);
            }
            else
            {
                html_printf();
                exit(1);
            }
        }
    }

    /*
     * 以可读写方式打开login.cfg配置文件
     * 配置文件已存在
     */
    fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "w+b");
    if(fp_s2wifi_cfg == NULL)
    {
        html_printf();
        exit(-1);
    }

    /* 写入文件 */
    for(i=0; i<S2WIFI_CFG_FILE_ROW; i++)
    {
        fputs(file_s2wifi_cfg_buf[i], fp_s2wifi_cfg);
    }

    fflush(fp_s2wifi_cfg);
    fclose(fp_s2wifi_cfg);
    system("cat /var/www/cgi-bin/s2wifi.cfg 1 > /wifi/log2");

    //config_file_save(); 
    //usleep(1000*10);
    debug_open();
    fn_cat_s2wifi_cfg();
    fn_write_s2wifi_config_to_mtd();
    debug_close();

    html_printf();
    exit(0);
}

/**
 * Close the Doxygen group.
 * @}
 */

