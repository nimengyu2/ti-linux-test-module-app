/**
 * @file config_file_lib.c
 * @brief 配置文件操作函数库
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
 * @date 2011-9-28 16:43:17
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

/**
 * @note
 * 配置文件操作函数库 \n
 *
 * \b Change \b Logs: \n
 * 2011.09.28   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_config_file_lib
 * @{
 */

#include "config_file_lib.h"
#include <data_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtd_ops.h"
#include "debug.h"


// 所有配置文件集合
#define LOGIN_CFG_FILE           "/var/www/cgi-bin/login.cfg"
#define S2WIFI_CFG_FILE          "/var/www/cgi-bin/s2wifi.cfg"
#define WIFI_MODE_FILE           "/etc/wifi_mode.conf"
#define AT_PARAM_FILE            "/etc/s2wifi/at_param.conf"

// TODO: Add your code here
// 登录配置文件
//#define LOGIN_CFG_FILE           "/var/www/cgi-bin/login.cfg"
#define LOGIN_CFG_FILE_ROW       10         // 配置文件最大的行号
#define LOGIN_CFG_FILE_COLUMN    100        // 配置文件最大的列号
#define LOGIN_CFG_VUSERNAME      (5-1)
#define LOGIN_CFG_VPASSWORD      (7-1)
#define LOGIN_CFG_VISLOGIN       (9-1)

// 核心配置文件
//#define S2WIFI_CFG_FILE          "/var/www/cgi-bin/s2wifi.cfg"
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

// 其他配置文件
//#define WIFI_MODE_FILE           "/etc/wifi_mode.conf"
//#define AT_PARAM_FILE            "/etc/s2wifi/at_param.conf"
/*
 * 字符变量区
 */
/* login.cfg */
char vusername[10+1] = {0}; // 用户名
char vpassword[10+1] = {0}; // 密码
char vislogin[1+1] = {0}; // 用户是否已登录

/* s2wifi.cfg */
char vssid[32+1] = {0}; // SSID
char vencrytype[1+1] = {0}; // Encryption type
char vwifipassword[63+1] = {0}; // WIFI password
char vstaticip[1+1] = {0}; // Address type
char vwlansip[15+1] = {0}; // Static IP address
char vwlanmip[15+1] = {0}; // Subnet Mask
char vwlangip[15+1] = {0}; // Default Gateway
char vwlandip[15+1] = {0}; // DNS Server
char vsocketlp[5+1] = {0}; // Local Socket Port Number
char vsocketrp[5+1] = {0}; // Remote Socket Port Number
char vsocketmode[1+1] = {0}; // Socket Mode
char vsocketprol[1+1] = {0}; // Socket Protocol
char vsocketrip[15+1] = {0}; // Remote Server IP
char vbaudrate[6+1] = {0}; // Baud Rate
char vdatasize[1+1] = {0}; // Data Size
char vparity[1+1] = {0}; // Parity
char vstop[1+1] = {0}; // Stop Bits
char vflow[1+1] = {0}; // Flow Control

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
 * @return bool_t 是否正确输出变量true:表示成功/false:表示失败
 */
static
bool_t get_value_form_string(const char *str_s,
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
        return(false);
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

    return(true);
}

bool_t fn_first_read_config(void)
{

    int i;
    FILE *fp_login_cfg, *fp_s2wifi_cfg;
    int ret;
    FILE * fp;
    int mtd_error_flag = 0;
    unsigned char pu8_para_buf[4096];
#if 0
    mtd_error_flag = 0;
    ret = fn_mtd_read(M_CFG_BLOCK_LOGIN_CFG,g_pu8_flush_mtd_buf_login_cfg,
		sizeof(g_pu8_flush_mtd_buf_login_cfg));
    if(ret == 0)
    {
	if(g_pu8_flush_mtd_buf_login_cfg[0] == '#')
	{
		printf("first_read_config: read mtd file ok\n");
		fp = fopen(LOGIN_CFG_FILE,"w+");
		ret = fwrite((char*)g_pu8_flush_mtd_buf_login_cfg,1,sizeof(g_pu8_flush_mtd_buf_login_cfg),fp);
		if(ret == sizeof(g_pu8_flush_mtd_buf_login_cfg))
		{
			printf("first_read_config: get mode from mtd and wirte to ramdisk ok\n");
		}
		else
		{
			printf("first_read_config: get mode from mtd and wirte to ramdisk failed\n");
		}
		// 注意如果使用close(fp)，则会发现写进去的数据不对，因此后来改成fclose(fp)，或者fflush即可
		fflush(fp);
		fclose(fp);
	}   
	else
	{
		mtd_error_flag = 1;
	}      
	
    }
    else   // 读取错误
    {
		mtd_error_flag = 1;
    }
    if(mtd_error_flag == 1)
    {
	mtd_error_flag = 0;	
	printf("first_read_config:detect mtd error\n");
	fp_login_cfg = fopen(LOGIN_CFG_FILE, "r+");
	if(fp_login_cfg == NULL)
	{
		return(false);
	}
	ret = fread(pu8_para_buf,1,sizeof(pu8_para_buf),fp_login_cfg);
	if(ret <= 0)
	{
		printf("first_read_config:fread from read_config error\n");
		return(false);
	}	
	else
	{
		printf("first_read_config:fread from ramdisk read_config ok\n");
		fn_mtd_write(M_CFG_BLOCK_LOGIN_CFG,pu8_para_buf,strlen(pu8_para_buf));
		printf("first_read_config:write to mtd \n");
	}
	fclose(fp_login_cfg);	
    }
#endif
    mtd_error_flag = 0;
    ret = fn_mtd_read(M_CFG_BLOCK_S2WIFI_CFG,g_pu8_flush_mtd_buf_s2wifi_cfg,
		sizeof(g_pu8_flush_mtd_buf_s2wifi_cfg));
    if(ret == 0)
    {
	if(g_pu8_flush_mtd_buf_s2wifi_cfg[0] == '#')
	{
		printf("read_config s2wifi: read mtd file ok\n");
		fp = fopen(S2WIFI_CFG_FILE,"w+");
		ret = fwrite(g_pu8_flush_mtd_buf_s2wifi_cfg,1,sizeof(g_pu8_flush_mtd_buf_s2wifi_cfg),fp);
		if(ret == sizeof(g_pu8_flush_mtd_buf_s2wifi_cfg))
		{
			printf("read_config s2wifi: get mode from mtd and wirte to ramdisk ok\n");
		}
		else
		{
			printf("read_config s2wifi: get mode from mtd and wirte to ramdisk failed\n");
		}
		// 注意如果使用close(fp)，则会发现写进去的数据不对，因此后来改成fclose(fp)，或者fflush即可
		fflush(fp);
		fclose(fp);
	}   
	else
	{
		mtd_error_flag = 1;
	}      
	
    }
    else   // 读取错误
    {
		mtd_error_flag = 1;
    }
    if(mtd_error_flag == 1)
    {
	mtd_error_flag = 0;	
	printf("read_config s2wifi: detect mtd error\n");
	fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "r+");
	if(fp_s2wifi_cfg == NULL)
	{
		return(false);
	}
	ret = fread(pu8_para_buf,1,sizeof(pu8_para_buf),fp_s2wifi_cfg);
	if(ret <= 0)
	{
		printf("read_config s2wifi:fread from read_config error\n");
		return(false);
	}	
	else
	{
		printf("read_config s2wifi:fread from ramdisk read_config ok\n");
		fn_mtd_write(M_CFG_BLOCK_S2WIFI_CFG,pu8_para_buf,strlen(pu8_para_buf));
		printf("read_config s2wifi:write to mtd block %d\n",M_CFG_BLOCK_S2WIFI_CFG);
	}
	fclose(fp_s2wifi_cfg);	
    }

}

bool_t fn_write_login_config_to_mtd(void)
{
      	FILE *fp_login_cfg, *fp_s2wifi_cfg;
	int ret;
	unsigned char pu8_para_buf[1024];
        fp_login_cfg = fopen(LOGIN_CFG_FILE, "r+");
	if(fp_login_cfg == NULL)
	{
		return(false);
	}
	ret = fread(pu8_para_buf,1,sizeof(pu8_para_buf),fp_login_cfg);
	if(ret <= 0)
	{
		printf("read_config:fread from read_config error\n");
		return(false);
	}	
	else
	{
		printf("read_config:fread from ramdisk read_config ok\n");
		fn_mtd_write(M_CFG_BLOCK_LOGIN_CFG,pu8_para_buf,strlen(pu8_para_buf));
		printf("read_config:write to mtd \n");
	}
	fclose(fp_login_cfg);
}
bool_t fn_write_s2wifi_config_to_mtd(void)
{
	FILE *fp_login_cfg, *fp_s2wifi_cfg;
	int ret;
	unsigned char pu8_para_buf[1024];	
	fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "r+");
	if(fp_s2wifi_cfg == NULL)
	{
		return(false);
	}
	ret = fread(pu8_para_buf,1,sizeof(pu8_para_buf),fp_s2wifi_cfg);
	if(ret <= 0)
	{
		printf("read_config s2wifi:fread from read_config error\n");
		return(false);
	}	
	else
	{
		printf("read_config s2wifi:fread from ramdisk read_config ok\n");
		fn_mtd_write(M_CFG_BLOCK_S2WIFI_CFG ,pu8_para_buf,strlen(pu8_para_buf));
		printf("read_config s2wifi:write to mtd \n");
	}
	fclose(fp_s2wifi_cfg);	
}

/**
 * 配置文件读取
 *
 * @author Toby (2011-9-16)
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
static
bool_t read_config(void)
{
    int i;
    FILE *fp_login_cfg, *fp_s2wifi_cfg;
    // UNIX换行LF，不支持DOS换行CR/LF
    char file_login_cfg_buf[LOGIN_CFG_FILE_ROW][LOGIN_CFG_FILE_COLUMN+1];
    char file_s2wifi_cfg_buf[S2WIFI_CFG_FILE_ROW][S2WIFI_CFG_FILE_COLUMN+1];
    int user_islogin = 0;

#if 0
    int ret;
    FILE * fp;
    int mtd_error_flag = 0;
    unsigned char pu8_para_buf[1024];
    mtd_error_flag = 0;
    ret = fn_mtd_read(M_CFG_BLOCK_LOGIN_CFG,g_pu8_flush_mtd_buf_login_cfg,
		sizeof(g_pu8_flush_mtd_buf_login_cfg));
    if(ret == 0)
    {
	if(g_pu8_flush_mtd_buf_login_cfg[0] == '#')
	{
		printf("read_config: read mtd file ok\n");
		fp = fopen(LOGIN_CFG_FILE,"w+");
		ret = fwrite((char*)g_pu8_flush_mtd_buf_login_cfg,1,sizeof(g_pu8_flush_mtd_buf_login_cfg),fp);
		if(ret == sizeof(g_pu8_flush_mtd_buf_login_cfg))
		{
			printf("read_config: get mode from mtd and wirte to ramdisk ok\n");
		}
		else
		{
			printf("read_config: get mode from mtd and wirte to ramdisk failed\n");
		}
		// 注意如果使用close(fp)，则会发现写进去的数据不对，因此后来改成fclose(fp)，或者fflush即可
		fflush(fp);
		fclose(fp);
	}   
	else
	{
		mtd_error_flag = 1;
	}      
	
    }
    else   // 读取错误
    {
		mtd_error_flag = 1;
    }
    if(mtd_error_flag == 1)
    {
	mtd_error_flag = 0;	
	printf("read_config: detect mtd error\n");
	fp_login_cfg = fopen(LOGIN_CFG_FILE, "r+");
	if(fp_login_cfg == NULL)
	{
		return(false);
	}
	ret = fread(pu8_para_buf,1,sizeof(pu8_para_buf),fp_login_cfg);
	if(ret <= 0)
	{
		printf("read_config:fread from read_config error\n");
		return(false);
	}	
	else
	{
		printf("read_config:fread from ramdisk read_config ok\n");
		fn_mtd_write(M_CFG_BLOCK_LOGIN_CFG,pu8_para_buf,strlen(pu8_para_buf));
		printf("read_config:write to mtd \n");
	}
	fclose(fp_login_cfg);	
    }
#endif    

    /*
     * 以只读方式打开login.cfg配置文件
     */
    fp_login_cfg = fopen(LOGIN_CFG_FILE, "rb");
    if(fp_login_cfg == NULL)
    {
        return(false);
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

    
#if 0
    mtd_error_flag = 0;
    ret = fn_mtd_read(M_CFG_BLOCK_S2WIFI_CFG,g_pu8_flush_mtd_buf_s2wifi_cfg,
		sizeof(g_pu8_flush_mtd_buf_s2wifi_cfg));
    if(ret == 0)
    {
	if(g_pu8_flush_mtd_buf_s2wifi_cfg[0] == '#')
	{
		printf("read_config s2wifi: read mtd file ok\n");
		fp = fopen(S2WIFI_CFG_FILE,"w+");
		ret = fwrite(g_pu8_flush_mtd_buf_s2wifi_cfg,1,sizeof(g_pu8_flush_mtd_buf_s2wifi_cfg),fp);
		if(ret == sizeof(g_pu8_flush_mtd_buf_s2wifi_cfg))
		{
			printf("read_config s2wifi: get mode from mtd and wirte to ramdisk ok\n");
		}
		else
		{
			printf("read_config s2wifi: get mode from mtd and wirte to ramdisk failed\n");
		}
		// 注意如果使用close(fp)，则会发现写进去的数据不对，因此后来改成fclose(fp)，或者fflush即可
		fflush(fp);
		fclose(fp);
	}   
	else
	{
		mtd_error_flag = 1;
	}      
	
    }
    else   // 读取错误
    {
		mtd_error_flag = 1;
    }
    if(mtd_error_flag == 1)
    {
	mtd_error_flag = 0;	
	printf("read_config s2wifi: detect mtd error\n");
	fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "r+");
	if(fp_s2wifi_cfg == NULL)
	{
		return(false);
	}
	ret = fread(pu8_para_buf,1,sizeof(pu8_para_buf),fp_s2wifi_cfg);
	if(ret <= 0)
	{
		printf("read_config s2wifi:fread from read_config error\n");
		return(false);
	}	
	else
	{
		printf("read_config s2wifi:fread from ramdisk read_config ok\n");
		fn_mtd_write(M_CFG_BLOCK_S2WIFI_CFG ,pu8_para_buf,strlen(pu8_para_buf));
		printf("read_config s2wifi:write to mtd \n");
	}
	fclose(fp_s2wifi_cfg);	
    }
#endif

    /*
     * 以只读方式打开s2wifi.cfg配置文件
     */
    fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "rb");
    if(fp_s2wifi_cfg == NULL)
    {
        return(false);
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

#if 0
printf("var vssid = \"%s\";\n", vssid);
printf("var vencrytype = %s;\n", vencrytype);
printf("var vwifipassword = \"\";\n");
printf("var vstaticip = %s;\n", vstaticip);
printf("var vwlansip = \"%s\";\n", vwlansip);
printf("var vwlanmip = \"%s\";\n", vwlanmip);
printf("var vwlangip = \"%s\";\n", vwlangip);
printf("var vwlandip = \"%s\";\n", vwlandip);
printf("\n");
printf("var vsocketlp = %s;\n", vsocketlp);
printf("var vsocketrp = %s;\n", vsocketrp);
printf("var vsocketmode = %s;\n", vsocketmode);
printf("var vsocketprol = %s;\n", vsocketprol);
printf("var vsocketrip = \"%s\";\n", vsocketrip);
printf("\n");
printf("var vbaudrate = %s;\n", vbaudrate);
printf("var vdatasize = %s;\n", vdatasize);
printf("var vparity = %s;\n", vparity);
printf("var vstop = %s;\n", vstop);
printf("var vflow = %s;\n", vflow);
#endif

    return(true);
}

/**
 * 保存一个登陆配置文件，调用前需要先初始化一些变量值
 * 也就是先调用read_config()函数，然后修改需要修改的变量
 * 值，最后调用本函数来保存新的参数值。
 *
 * @author toby.zhang (2011-10-5)
 *
 * @return bool_t true:保存成功/false:保存失败
 */
static
bool_t login_cfg_save(void)
{
    FILE *fp_login_cfg;
    #if 0
    int ret;
    int mtd_error_flag = 0;
    unsigned char pu8_para_buf[1024];
    #endif

    fp_login_cfg = fopen(LOGIN_CFG_FILE, "w+b");
    if(fp_login_cfg == NULL)
    {
        return(false);
    }

    /* 写入文件 */
    // # S2WIFI Login file. Build by toby@2011.09.14
    fprintf(fp_login_cfg,
            "# S2WIFI Login file. Build by toby@2011.09.14\n");
    // # NOTE: There are no spaces between characters and characters.
    fprintf(fp_login_cfg,
            "# NOTE: There are no spaces between characters and characters.\n");
    // # !Begin!
    fprintf(fp_login_cfg,
            "# !Begin!\n");
    // # LOGIN. Username, Max: 10 characters.
    fprintf(fp_login_cfg,
            "# LOGIN. Username, Max: 10 characters.\n");
    // vusername=admin
    fprintf(fp_login_cfg,
            "vusername=%s\n",
            vusername);
    // # LOGIN. Password, Max: 10 characters.
    fprintf(fp_login_cfg,
            "# LOGIN. Password, Max: 10 characters.\n");
    // vpassword=1234567890
    fprintf(fp_login_cfg,
            "vpassword=%s\n",
            vpassword);
    // # 0:NotLogin/1:Login. default is 0
    fprintf(fp_login_cfg,
            "# 0:NotLogin/1:Login. default is 0\n");
    // vislogin=0
    fprintf(fp_login_cfg,
            "vislogin=%s\n",
            vislogin);
    // # !End!
    fprintf(fp_login_cfg,
            "# !End!\n");

    fclose(fp_login_cfg);
    
    #if 0
    	fp_login_cfg = fopen(LOGIN_CFG_FILE, "r+");
	if(fp_login_cfg == NULL)
	{
		return(false);
	}
	ret = fread(pu8_para_buf,1,sizeof(pu8_para_buf),fp_login_cfg);
	if(ret <= 0)
	{
		printf("read_config:fread from read_config error\n");
		return(false);
	}	
	else
	{
		printf("read_config:fread from ramdisk read_config ok\n");
		fn_mtd_write(M_CFG_BLOCK_LOGIN_CFG,pu8_para_buf,strlen(pu8_para_buf));
		printf("read_config:write to mtd \n");
	}
	fclose(fp_login_cfg);
    #endif

    return(true);
}

/**
 * 保存一个s2wifi配置文件，调用前需要先初始化一些变量值
 * 也就是先调用read_config()函数，然后修改需要修改的变量
 * 值，最后调用本函数来保存新的参数值。
 *
 * @author toby.zhang (2011-10-5)
 *
 * @return bool_t true:保存成功/false:保存失败
 */
static
bool_t s2wifi_cfg_save(void)
{
    FILE *fp_s2wifi_cfg;
    #if 0
    int ret;
    int mtd_error_flag = 0;
    unsigned char pu8_para_buf[1024];
    #endif

    fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "w+b");
    if(fp_s2wifi_cfg == NULL)
    {
        return(false);
    }

    /* 写入文件 */
// # S2WIFI Config file. Build by toby@2011.09.14
    fprintf(fp_s2wifi_cfg,
"# S2WIFI Config file. Build by toby@2011.09.14\n");
// # NOTE: There are no spaces between characters and characters.
    fprintf(fp_s2wifi_cfg,
"# NOTE: There are no spaces between characters and characters.\n");
// # !Begin!
    fprintf(fp_s2wifi_cfg,
"# !Begin!\n");
// # WLAN. SSID, Max: 32 characters. e.g: TEST-15.
    fprintf(fp_s2wifi_cfg,
"# WLAN. SSID, Max: 32 characters. e.g: TEST-15.\n");
// vssid=TEST-15
    fprintf(fp_s2wifi_cfg,
            "vssid=%s\n",
            vssid);
// # WLAN. Encryption type. 0:None/1:WEP-open/2:WEP-share/3:WPA or WPA2.
    fprintf(fp_s2wifi_cfg,
"# WLAN. Encryption type. 0:None/1:WEP-open/2:WEP-share/3:WPA or WPA2.\n");
// vencrytype=3
    fprintf(fp_s2wifi_cfg,
            "vencrytype=%s\n",
            vencrytype);
// # WLAN. WIFI password. Max: 63 characters. WPA or WPA2 Min: 8 characters.
    fprintf(fp_s2wifi_cfg,
"# WLAN. WIFI password. Max: 63 characters. WPA or WPA2 Min: 8 characters.\n");
// vwifipassword=zhangxiaojie
    fprintf(fp_s2wifi_cfg,
            "vwifipassword=%s\n",
            vwifipassword);
// # WLAN. Address type. 0:DHCP/1:StaticIP.
    fprintf(fp_s2wifi_cfg,
"# WLAN. Address type. 0:DHCP/1:StaticIP.\n");
// vstaticip=1
    fprintf(fp_s2wifi_cfg,
            "vstaticip=%s\n",
            vstaticip);
// # WLAN. Static IP address. e.g:192.168.1.80.
    fprintf(fp_s2wifi_cfg,
"# WLAN. Static IP address. e.g:192.168.1.80.\n");
// vwlansip=192.168.1.80
    fprintf(fp_s2wifi_cfg,
            "vwlansip=%s\n",
            vwlansip);
// # WLAN. Subnet Mask. e.g:255.255.255.0.
    fprintf(fp_s2wifi_cfg,
"# WLAN. Subnet Mask. e.g:255.255.255.0.\n");
// vwlanmip=255.255.255.0
    fprintf(fp_s2wifi_cfg,
            "vwlanmip=%s\n",
            vwlanmip);
// # WLAN. Default Gateway. e.g:192.168.1.1.
    fprintf(fp_s2wifi_cfg,
"# WLAN. Default Gateway. e.g:192.168.1.1.\n");
// vwlangip=192.168.1.1
    fprintf(fp_s2wifi_cfg,
            "vwlangip=%s\n",
            vwlangip);
// # WLAN. DNS Server. e.g:192.168.1.1.
    fprintf(fp_s2wifi_cfg,
"# WLAN. DNS Server. e.g:192.168.1.1.\n");
// vwlandip=192.168.1.1
    fprintf(fp_s2wifi_cfg,
            "vwlandip=%s\n",
            vwlandip);
// # WIFI. Local Socket Port Number. (1, 65535).
    fprintf(fp_s2wifi_cfg,
"# WIFI. Local Socket Port Number. (1, 65535).\n");
// vsocketlp=1234
    fprintf(fp_s2wifi_cfg,
            "vsocketlp=%s\n",
            vsocketlp);
// # WIFI. Remote Socket Port Number. (1, 65535).
    fprintf(fp_s2wifi_cfg,
"# WIFI. Remote Socket Port Number. (1, 65535).\n");
// vsocketrp=5689
    fprintf(fp_s2wifi_cfg,
            "vsocketrp=%s\n",
            vsocketrp);
// # WIFI. Socket Mode. 0:Server/1:Client.
    fprintf(fp_s2wifi_cfg,
"# WIFI. Socket Mode. 0:Server/1:Client.\n");
// vsocketmode=0
    fprintf(fp_s2wifi_cfg,
            "vsocketmode=%s\n",
            vsocketmode);
// # WIFI. Socket Protocol. 0:TCP/1:UDP.
    fprintf(fp_s2wifi_cfg,
"# WIFI. Socket Protocol. 0:TCP/1:UDP.\n");
// vsocketprol=0
    fprintf(fp_s2wifi_cfg,
            "vsocketprol=%s\n",
            vsocketprol);
// # WIFI. Remote Server IP. e.g:192.168.1.100.
    fprintf(fp_s2wifi_cfg,
"# WIFI. Remote Server IP. e.g:192.168.1.100.\n");
// vsocketrip=192.168.1.100
    fprintf(fp_s2wifi_cfg,
            "vsocketrip=%s\n",
            vsocketrip);
// # SERIAL. Baud Rate. 110/300/600/1200/2400/4800/9600/14400/19200/38400/57600/
    fprintf(fp_s2wifi_cfg,
"# SERIAL. Baud Rate. \
110/300/600/1200/2400/4800/9600/14400/19200/38400/57600/\n");
// # 115200/230400.
    fprintf(fp_s2wifi_cfg,
"# 115200/230400.\n");
// vbaudrate=115200
    fprintf(fp_s2wifi_cfg,
            "vbaudrate=%s\n",
            vbaudrate);
// # SERIAL. Data Size. 5/6/7/8
    fprintf(fp_s2wifi_cfg,
"# SERIAL. Data Size. 5/6/7/8\n");
// vdatasize=8
    fprintf(fp_s2wifi_cfg,
            "vdatasize=%s\n",
            vdatasize);
// # SERIAL. Parity. 0:None/1:Odd/2:Even/3:Mark/4:Space.
    fprintf(fp_s2wifi_cfg,
"# SERIAL. Parity. 0:None/1:Odd/2:Even/3:Mark/4:Space.\n");
// vparity=0
    fprintf(fp_s2wifi_cfg,
            "vparity=%s\n",
            vparity);
// # SERIAL. Stop Bits. 1/2.
    fprintf(fp_s2wifi_cfg,
"# SERIAL. Stop Bits. 1/2.\n");
// vstop=1
    fprintf(fp_s2wifi_cfg,
            "vstop=%s\n",
            vstop);
// # SERIAL. Flow Control. 0:None/1:Hardware.
    fprintf(fp_s2wifi_cfg,
"# SERIAL. Flow Control. 0:None/1:Hardware.\n");
// vflow=0
    fprintf(fp_s2wifi_cfg,
            "vflow=%s\n",
            vflow);
// # !End!
    fprintf(fp_s2wifi_cfg,
            "# !End!\n");

    fclose(fp_s2wifi_cfg);

#if 0
	fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "r+");
	if(fp_s2wifi_cfg == NULL)
	{
		return(false);
	}
	ret = fread(pu8_para_buf,1,sizeof(pu8_para_buf),fp_s2wifi_cfg);
	if(ret <= 0)
	{
		printf("read_config s2wifi:fread from read_config error\n");
		return(false);
	}	
	else
	{
		printf("read_config s2wifi:fread from ramdisk read_config ok\n");
		fn_mtd_write(M_CFG_BLOCK_S2WIFI_CFG ,pu8_para_buf,strlen(pu8_para_buf));
		printf("read_config s2wifi:write to mtd \n");
	}
	fclose(fp_s2wifi_cfg);	
#endif

    return(true);
}

/**
 * 登入用户名获取
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t login_username_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vusername;

    return(ret);
}

/**
 * 登入用户名设置
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t login_username_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vusername, 0, sizeof(vusername));
    strncpy(vusername, str, len);

    ret = login_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 登入密码获取
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t login_password_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vpassword;

    return(ret);
}

/**
 * 登入密码设置
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t login_password_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vpassword, 0, sizeof(vpassword));
    strncpy(vpassword, str, len);

    ret = login_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 登入状态获取
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t login_islogin_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vislogin;

    return(ret);
}

/**
 * 登入状态设置
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t login_islogin_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vislogin, 0, sizeof(vislogin));
    strncpy(vislogin, str, len);

    ret = login_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取无线网络SSID
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_ssid_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vssid;

    return(ret);
}

/**
 * 设置无线网络SSID
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_ssid_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vssid, 0, sizeof(vssid));
    strncpy(vssid, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取无线网络加密方式
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_encrytype_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vencrytype;

    return(ret);
}

/**
 * 设置无线网络加密方式
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_encrytype_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vencrytype, 0, sizeof(vencrytype));
    strncpy(vencrytype, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取无线网络密码
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_wifipassword_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vwifipassword;

    return(ret);
}

/**
 * 设置无线网络加密密码
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_wifipassword_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vwifipassword, 0, sizeof(vwifipassword));
    strncpy(vwifipassword, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取无线网络IP获取方式
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_staticip_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vstaticip;

    return(ret);
}

/**
 * 设置无线网络IP获取方式
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_staticip_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vstaticip, 0, sizeof(vstaticip));
    strncpy(vstaticip, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取无线网络无静态IP地址
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_wlansip_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vwlansip;

    return(ret);
}

/**
 * 设置无线网络静态IP地址
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_wlansip_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vwlansip, 0, sizeof(vwlansip));
    strncpy(vwlansip, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取子网掩码
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_wlanmip_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vwlanmip;

    return(ret);
}

/**
 * 设置无线网络子网掩码
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_wlanmip_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vwlanmip, 0, sizeof(vwlanmip));
    strncpy(vwlanmip, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取无线网络网关地址
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_wlangip_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vwlangip;

    return(ret);
}

/**
 * 设置无线网络网关地址
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_wlangip_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vwlangip, 0, sizeof(vwlangip));
    strncpy(vwlangip, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取无线网络DNS服务器地址
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_wlandip_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vwlandip;

    return(ret);
}

/**
 * 设置无线网络DNS服务器地址
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wlan_wlandip_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vwlandip, 0, sizeof(vwlandip));
    strncpy(vwlandip, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取WIFI port 本地端口号
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wifi_port_socketlp_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vsocketlp;

    return(ret);
}

/**
 * 设置WIFI port 本地端口号
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wifi_port_socketlp_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vsocketlp, 0, sizeof(vsocketlp));
    strncpy(vsocketlp, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取WIFI port 远程端口号
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wifi_port_socketrp_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vsocketrp;

    return(ret);
}

/**
 * 设置WIFI port 远程端口号
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wifi_port_socketrp_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vsocketrp, 0, sizeof(vsocketrp));
    strncpy(vsocketrp, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取WIFI port 工作模式 Client or Server
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wifi_port_socketmode_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vsocketmode;

    return(ret);
}

/**
 * 设置WIFI port 工作模式 Client or Server
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wifi_port_socketmode_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vsocketmode, 0, sizeof(vsocketmode));
    strncpy(vsocketmode, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取WIFI port 通信协议
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wifi_port_socketprol_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vsocketprol;

    return(ret);
}

/**
 * 设置WIFI port 通信协议
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wifi_port_socketprol_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vsocketprol, 0, sizeof(vsocketprol));
    strncpy(vsocketprol, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取WIFI port 远程服务器IP地址
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t wifi_port_socketrip_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vsocketrip;

    return(ret);
}

/**
 * 设置WIFI port 远程服务器IP地址
 *
 * @author toby.zhang (2011-10-6)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t wifi_port_socketrip_set(char *str,  int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vsocketrip, 0, sizeof(vsocketrip));
    strncpy(vsocketrip, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 *  获取串口波特率
 *
 * @author Toby (2011-9-28)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t serial_port_baudrate_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vbaudrate;

    return(ret);
}

/**
 * 波特率设置
 *
 * @author toby.zhang (2011-10-5)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t serial_port_baudrate_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vbaudrate, 0, sizeof(vbaudrate));
    strncpy(vbaudrate, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取串口数据位大小
 *
 * @author Toby (2011-9-28)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t serial_port_datasize_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vdatasize;

    return(ret);
}

/**
 * 数据位设置
 *
 * @author toby.zhang (2011-10-5)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t serial_port_datasize_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vdatasize, 0, sizeof(vdatasize));
    strncpy(vdatasize, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取串口校验模式
 *
 * @author Toby (2011-9-28)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t serial_port_parity_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vparity;

    return(ret);
}

/**
 * 校验位设置
 *
 * @author toby.zhang (2011-10-5)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t serial_port_parity_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vparity, 0, sizeof(vparity));
    strncpy(vparity, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取串口停止位数值
 *
 * @author Toby (2011-9-28)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t serial_port_stop_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vstop;

    return(ret);
}

/**
 * 停止位设置
 *
 * @author toby.zhang (2011-10-5)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t serial_port_stop_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vstop, 0, sizeof(vstop));
    strncpy(vstop, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * 获取串口流控状态
 *
 * @author Toby (2011-9-28)
 *
 * @param val 执行存储值地址
 *
 * @return bool_t 返回读取是否成功, true:表示成功/false:表示失败
 */
bool_t serial_port_flow_get(char **val)
{
    bool_t ret;

    ret = read_config();
    *val = vflow;

    return(ret);
}

/**
 * 流控状态设置
 *
 * @author toby.zhang (2011-10-5)
 *
 * @param str 字符串变量
 * @param len 字符串长度
 *
 * @return bool_t 写入是否成功, true:表示成功/false:表示失败
 */
bool_t serial_port_flow_set(char *str, int len)
{
    bool_t ret;

    ret = read_config();
    if(ret == false)
    {
        return(false);
    }

    memset(vflow, 0, sizeof(vflow));
    strncpy(vflow, str, len);

    ret = s2wifi_cfg_save();
    if(ret == false)
    {
        return(false);
    }

    return(true);
}

/**
 * login恢复出厂设置
 *
 * @author Toby (2011-10-10)
 *
 * @return bool_t 保存是否成功, true:成功/false:失败
 *
 */
bool_t login_file_factor_load(void)
{
    FILE *fp_login_cfg;
    unsigned char pu8_para_buf[1024];

    fp_login_cfg = fopen(LOGIN_CFG_FILE, "w+b");
    if(fp_login_cfg == NULL)
    {
        return(false);
    }

    /* 写入文件 */
    snprintf(pu8_para_buf,sizeof(pu8_para_buf),"%s","\
# S2WIFI Login file. Build by toby@2011.09.14\n\
# NOTE: There are no spaces between characters and characters.\n\
# !Begin!\n\
# LOGIN. Username, Max: 10 characters.\n\
vusername=admin\n\
# LOGIN. Password, Max: 10 characters.\n\
vpassword=1234567890\n\
# 0:NotLogin/1:Login. default is 0\n\
vislogin=0\n\
# !End!\n\
");
    fprintf(fp_login_cfg,pu8_para_buf);
    fclose(fp_login_cfg);
#if 0	
	fn_mtd_write(M_CFG_BLOCK_LOGIN_CFG ,pu8_para_buf,strlen(pu8_para_buf));
#endif 


    return(true);
}

/**
 * s2wifi恢复出厂设置
 *
 * @author Toby (2011-10-10)
 *
 * @return bool_t 保存是否成功, true:成功/false:失败
 *
 */
bool_t s2wifi_file_factor_load(void)
{
    FILE *fp_s2wifi_cfg;
    unsigned char pu8_para_buf[1024];

    fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "w+b");
    if(fp_s2wifi_cfg == NULL)
    {
        return(false);
    }

    /* 写入文件 */
    snprintf(pu8_para_buf,sizeof(pu8_para_buf),"%s","\
# S2WIFI Config file. Build by toby@2011.09.14\n\
# NOTE: There are no spaces between characters and characters.\n\
# !Begin!\n\
# WLAN. SSID, Max: 32 characters. e.g: TEST-15.\n\
vssid=TEST-15\n\
# WLAN. Encryption type. 0:None/1:WEP-open/2:WEP-share/3:WPA or WPA2.\n\
vencrytype=3\n\
# WLAN. WIFI password. Max: 63 characters. WPA or WPA2 Min: 8 characters.\n\
vwifipassword=zhangxiaojie\n\
# WLAN. Address type. 0:DHCP/1:StaticIP.\n\
vstaticip=1\n\
# WLAN. Static IP address. e.g:192.168.1.80.\n\
vwlansip=192.168.1.80\n\
# WLAN. Subnet Mask. e.g:255.255.255.0.\n\
vwlanmip=255.255.255.0\n\
# WLAN. Default Gateway. e.g:192.168.1.1.\n\
vwlangip=192.168.1.1\n\
# WLAN. DNS Server. e.g:192.168.1.1.\n\
vwlandip=192.168.1.1\n\
# WIFI. Local Socket Port Number. (1, 65535).\n\
vsocketlp=1234\n\
# WIFI. Remote Socket Port Number. (1, 65535).\n\
vsocketrp=5689\n\
# WIFI. Socket Mode. 0:Server/1:Client.\n\
vsocketmode=0\n\
# WIFI. Socket Protocol. 0:TCP/1:UDP.\n\
vsocketprol=0\n\
# WIFI. Remote Server IP. e.g:192.168.1.100.\n\
vsocketrip=192.168.1.100\n\
# SERIAL. Baud Rate. 110/300/600/1200/2400/4800/9600/14400/19200/38400/57600/\n\
# 115200/230400.\n\
vbaudrate=115200\n\
# SERIAL. Data Size. 5/6/7/8\n\
vdatasize=8\n\
# SERIAL. Parity. 0:None/1:Odd/2:Even/3:Mark/4:Space.\n\
vparity=0\n\
# SERIAL. Stop Bits. 1/2.\n\
vstop=1\n\
# SERIAL. Flow Control. 0:None/1:Hardware.\n\
vflow=0\n\
# !End!\n\
");
    fprintf(fp_s2wifi_cfg,pu8_para_buf);

    fclose(fp_s2wifi_cfg);

#if 0
	fn_mtd_write(M_CFG_BLOCK_S2WIFI_CFG ,pu8_para_buf,strlen(pu8_para_buf));
#endif

    return(true);
}

/**
 * AT指令参数保存,保存在AT_PARAM_FILE文件中
 * static bool_t g_bDisableEcho = false; // Add by toby@2011.09.27
 * static bool_t g_bDisableAtq = true;  // Add by toby@2011.09.28
 *
 * 0: g_bDisableEcho = false && g_bDisableAtq = true
 * 1: g_bDisableEcho = false && g_bDisableAtq = false
 * 2: g_bDisableEcho = true && g_bDisableAtq = true
 * 3: g_bDisableEcho = true && g_bDisableAtq = false
 *
 * @author Toby (2011-10-10)
 *
 * @return bool_t true:保存成功/false:保存失败
 */
bool_t at_param_conf_factor_load(int val)
{
    FILE *fp;

    fp = fopen(AT_PARAM_FILE, "w+b");
    if(fp == NULL)
    {
        return(false);
    }

    /*
     * 写入文件
     */
    switch(val)
    {
    case 0:
        fprintf(fp, "0\n");
        break;

    case 1:
        fprintf(fp, "1\n");
        break;

    case 2:
        fprintf(fp, "2\n");
        break;

    case 3:
        fprintf(fp, "3\n");
        break;

    default:
        fprintf(fp, "0\n");
        break;
    }

    fclose(fp);
    return(true);
}

/**
 * 读取AT指令参数,保存在AT_PARAM_FILE文件中
 * static bool_t g_bDisableEcho = false; // Add by toby@2011.09.27
 * static bool_t g_bDisableAtq = true;  // Add by toby@2011.09.28
 *
 * 0: g_bDisableEcho = false && g_bDisableAtq = true
 * 1: g_bDisableEcho = false && g_bDisableAtq = false
 * 2: g_bDisableEcho = true && g_bDisableAtq = true
 * 3: g_bDisableEcho = true && g_bDisableAtq = false
 *
 * @author Toby (2011-10-10)
 *
 * @return int 返回0/1/2/3，否者出错，返回-1.
 */
int at_param_conf_read(void)
{
    FILE *fp;
    char buf[4];
    int ret = 0;

    fp = fopen(AT_PARAM_FILE, "rb");
    if(fp == NULL)
    {
        return(-1);
    }

    fseek(fp, 0, SEEK_SET);
    fgets(buf, 2, fp);

    switch(buf[0])
    {
    case '0':
        ret = 0;
        break;

    case '1':
        ret = 1;
        break;

    case '2':
        ret = 2;
        break;
    case '3':
        ret = 3;
        break;
    default:
        ret = -1;
        break;
    }

    fclose(fp);
    return(ret);
}

/**
 * wifi_mode恢复出厂设置
 *
 * @author Toby (2011-10-10)
 *
 * @return bool_t 保存是否成功, true:成功/false:失败
 *
 */
bool_t wifi_mode_file_factor_load(void)
{
    FILE *fp;

    fp = fopen(WIFI_MODE_FILE, "w+b");
    if(fp == NULL)
    {
        return(false);
    }

    /*
     * 写入文件
     */
    fprintf(fp, "wifi_mode=AP\n");

    fclose(fp);
    return(true);
}

/**
 * 恢复出厂设置
 *
 * @author Toby (2011-10-10)
 *
 * @return bool_t 保存是否成功, true:成功/false:失败
 *
 */
bool_t all_param_factor_load(void)
{
    if(login_file_factor_load() == false)
    {
        return(false);
    }

    if(s2wifi_file_factor_load() == false)
    {
        return(false);
    }

    if(wifi_mode_file_factor_load() == false)
    {
        return(false);
    }

    if(at_param_conf_factor_load(0) == false)
    {
        return(false);
    }

    return(true);
}

/**
 * Close the Doxygen group.
 * @}
 */

