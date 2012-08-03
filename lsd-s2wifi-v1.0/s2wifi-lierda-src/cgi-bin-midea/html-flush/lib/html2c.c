/**
 * @file html2c.c
 * @brief 主要负责网页的状态刷新工作
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
 * @date 2011-9-16 11:22:32
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

/**
 * @note
 * 负责网页状态刷新 \n
 *
 * \b Change \b Logs: \n
 * 2011.09.16   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_html2c_cgi
 * @{
 */

#include "html2c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * html状态刷新
 *
 * @author Toby (2011-9-16)
 *
 * @return int 返回刷新是否成功, 1:表示成功/-1:表示失败
 */
int html_printf(void)
{
    int i;
    FILE *fp_login_cfg, *fp_s2wifi_cfg;
    // UNIX换行LF，不支持DOS换行CR/LF
    char file_login_cfg_buf[LOGIN_CFG_FILE_ROW][LOGIN_CFG_FILE_COLUMN+1];
    char file_s2wifi_cfg_buf[S2WIFI_CFG_FILE_ROW][S2WIFI_CFG_FILE_COLUMN+1];

    int user_islogin = 0;

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

    /*
     * 以只读方式打开login.cfg配置文件
     */
    fp_login_cfg = fopen(LOGIN_CFG_FILE, "rb");
    if(fp_login_cfg == NULL)
    {
        return(-1);
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
        return(-1);
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
     * CGI HTML 输出
     */
    // 请不要修改以下代码
printf("Content-type: text/html\n\n");
    printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
    printf("<html>\n");
    printf("<head>\n");
    printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" />\n");
    printf("<link href=\"../styles.css\" rel=\"stylesheet\" media=\"screen\">\n");
    printf("<title>\n");
    printf("Configure Page</title>\n");
    printf("<script type='text/javascript' language='JavaScript'>\n");
printf("var vloginstat = %s;\n", vislogin);
if(user_islogin == 1)
{
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
}
else
{
printf("var vssid = \"\";\n");
printf("var vencrytype = ;\n");
printf("var vwifipassword = \"\";\n");
printf("var vstaticip = ;\n");
printf("var vwlansip = \"\";\n");
printf("var vwlanmip = \"\";\n");
printf("var vwlangip = \"\";\n");
printf("var vwlandip = \"\";\n");
printf("\n");
printf("var vsocketlp = ;\n");
printf("var vsocketrp = ;\n");
printf("var vsocketmode = ;\n");
printf("var vsocketprol = ;\n");
printf("var vsocketrip = \"\";\n");
printf("\n");
printf("var vbaudrate = ;\n");
printf("var vdatasize = ;\n");
printf("var vparity = ;\n");
printf("var vstop = ;\n");
printf("var vflow = ;\n");
}
    printf("</script>\n");
    printf("<script type=\"text/javascript\" src=\"../cfg.js\">\n");
    printf("</script>\n");
    printf("<style type=\"text/css\">\n");
    printf("<!--.STYLE12 {font-size: 16pt}-->\n");
    printf("</style>\n");
    printf("</head>\n");
    printf("<body onLoad=\"SetFormDefaults();\">\n");
    printf("<div align=\"center\">\n");
    printf("<table width=\"778\" height=\"179\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" style=\"text-align: left; width: 778 ; height: 66px;\">\n");
    printf("<tbody>\n");
    printf("<tr>\n");
    printf("<td width=\"389\" style=\"vertical-align: top; text-align: left; width: 100%;\">\n");
    printf("<a href=\"http://www.midea.com\" target=\"_blank\">\n");
    printf("<span title=\"Go to midea website\">\n");
    printf("</samp>\n");
    printf("</a>\n");
    printf("<img src=\"../logo/logo.png\" width=\"278\" height=\"89\">\n");
    printf("</td>\n");
    printf("<td width=\"389\" colspan=10width=\"1016\" style=\"vertical-align: top; text-align: left; width: 100%;\">\n");
    printf("<table width=\"200\" height=\"70\" border=\"0\" align=\"right\">\n");
    printf("<tr>\n");
    printf("<td height=\"66\">\n");
    printf("<div align=\"right\" class=\"STYLE9\">\n");
    printf("<div align=\"right\">\n");
    printf("<h4 class=\"STYLE10 STYLE12\">\n");
    printf("Welcome to Midea </h4>\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</table>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</tbody>\n");
    printf("</table>\n");
    printf("<hr width=\"790\" size=\"5\" class=\"STYLE10\" />\n");
    printf("<table width=\"778\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\" style=\"text-align: left; width: 790;\">\n");
    printf("<tbody>\n");
    printf("<tr>\n");
    printf("<td width=\"778\">\n");
    printf("<h3 class=\"box\">\n");
    printf("Login</h3>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td>\n");
    printf("<form action=\"login.cgi\" method=\"get\" name=\"login\">\n");
    printf("<table cellpadding=\"0\" cellspacing=\"1\" width=\"100%\">\n");
    printf("<tbody>\n");
    printf("<tr>\n");
    printf("<td colspan=\"4\">\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td width=\"23%\" rowspan=\"3\">\n");
    printf("<div align=\"left\">\n");
    printf("<input type=\"submit\" class=\"button\" id=\"loginbtn\" onClick=\"do_login_apply();\" value=\"Login\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td width=\"23%\">\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td width=\"25%\" class=\"gr\">\n");
    printf("<div align=\"center\">\n");
    printf("<span class=\"logingr\" title=\"Login username, default is admin, should be less than 10 characters.\">\n");
    printf("Username:</span>\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("<td width=\"2%\">\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td width=\"27%\">\n");
    printf("<div align=\"left\">\n");
    printf("<input name=\"username\" type=\"text\" class=\"loginlist\" value=\"admin\" size=\"10\" maxlength=\"10\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td class=\"gr\">\n");
    printf("<div align=\"center\">\n");
    printf("<span class=\"logingr\" title=\"Login password, should be less than 10 characters.\">\n");
    printf("Password:</span>\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<div align=\"left\">\n");
    printf("<input name=\"password\" type=\"password\" class=\"loginlist\" size=\"10\" maxlength=\"10\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td colspan=\"5\" rowspan=\"1\" align=\"center\">\n");
    printf("<div align=\"center\">\n");
    printf("<br>\n");
    printf("<p>\n");
    printf("<a id=\"ploginstat\">\n");
if(user_islogin == 1)
{
    // do nothing
}
else
{
printf("Not Logged In\n");
}
    printf("</a>\n");
    printf("</p>\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</tbody>\n");
    printf("</table>\n");
    printf("</form>\n");
    printf("</tr>\n");
    printf("</tbody>\n");
    printf("</table>\n");
    printf("<br>\n");
    printf("<table style=\"text-align: left; width: 778;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n");
    printf("<tbody>\n");
    printf("<tr>\n");
    printf("<td>\n");
    printf("<h3 class=\"box\">\n");
    printf("WLAN Configure</h3>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td>\n");
    printf("<form action=\"wlan.cgi\" method=\"get\" name=\"wlan\">\n");
    printf("<table cellpadding=\"0\" cellspacing=\"1\" width=\"100%\">\n");
    printf("<tbody>\n");
    printf("<tr>\n");
    printf("<td width=\"23%\" height=\"26\" class=\"gr\">\n");
    printf("</td>\n");
    printf("<td colspan=\"2\" class=\"logingr\">\n");
    printf("<div align=\"center\">\n");
    printf("Current</div>\n");
    printf("</td>\n");
    printf("<td width=\"40%\" class=\"logingr\">\n");
    printf("<div align=\"center\">\n");
    printf("Updated</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"SSID, should be less than 32 characters. Example: TEST-15\">\n");
    printf("SSID:</span>\n");
    printf("</td>\n");
    printf("<td width=\"1%\">\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td width=\"36%\">\n");
    printf("<a id=\"pssid\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<input name=\"ssid\" class=\"wlan\" id=\"ssid\" value=\"TEST-15\" maxlength=\"32\">\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"Encryption type, None,WEP-open,WEP-share, WPA or WPA2.\">\n");
    printf("Encryption Type:</span>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"pencrytype\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<select name=\"encrytype\" size=\"1\" class=\"list\" id=\"encrytype\" onChange=\"SetWLANState();\">\n");
    printf("<option value=\"0\">\n");
    printf("None</option>\n");
    printf("<option value=\"1\">\n");
    printf("WEP-open</option>\n");
    printf("<option value=\"2\">\n");
    printf("WEP-share</option>\n");
    printf("<option value=\"3\">\n");
    printf("WPA or WPA2</option>\n");
    printf("</select>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"WIFI password, please check your router's encryption type and password. Example: 123456\">\n");
    printf("WIFI Password:</span>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"pwifipassword\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<input name=\"wifipassword\" type=\"password\" class=\"wlan\" id=\"wifipassword\" maxlength=\"63\">\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"If you select DHCP, you should make sure the router have opened DHCP server. Otherwise, you should enter static IP address, subnet mask, gateway, and DNS manually.\">\n");
    printf("Address Type:</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"pstaticip\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<div align=\"left\">\n");
    printf("<select name=\"staticip\" size=\"1\" class=\"list\" onChange=\"SetWLANState();\">\n");
    printf("<option value=\"0\">\n");
    printf("DHCP</option>\n");
    printf("<option value=\"1\">\n");
    printf("Static IP</option>\n");
    printf("</select>\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"You should select Static IP first. Example: 192.168.1.20\">\n");
    printf("Static IP Address:</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"pwlansip\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<div align=\"left\">\n");
    printf("<input name=\"wlansip\" class=\"ip\" maxlength=\"15\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"You should select Static IP first. Make sure the Subnet ID of S2WIFI module is the same as your router. Example: 255.255.255.0\">\n");
    printf("Subnet Mask:</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"pwlanmip\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<div align=\"left\">\n");
    printf("<input name=\"wlanmip\" class=\"ip\" maxlength=\"15\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"You should select Static IP first. Check the IP of your router. Example:192.168.1.1\">\n");
    printf("Default Gateway:</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"pwlangip\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<div align=\"left\">\n");
    printf("<input name=\"wlangip\" class=\"ip\" maxlength=\"15\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"You should select Static IP first. DNS always is the same as the IP of your router. Example:192.168.1.1\">\n");
    printf("DNS Server:</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"pwlandip\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<div align=\"left\">\n");
    printf("<input name=\"wlandip\" class=\"ip\" maxlength=\"15\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</tbody>\n");
    printf("</table>\n");
    printf("</form>\n");
    printf("<div align=\"center\">\n");
    printf("<table width=\"778\" border=\"0\">\n");
    printf("<tr>\n");
    printf("<td width=\"654\">\n");
    printf("<div align=\"right\">\n");
    printf("<input type=\"submit\" class=\"button\" id=\"wlanbtn\" onClick=\"do_wlan_apply();\" value=\"Save\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("<td width=\"69\">\n");
    printf("<div align=\"right\">\n");
    printf("<input type=\"submit\" class=\"button\" onClick=\"do_wlan_reset();\" value=\"Reset\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("<td width=\"41\">\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</table>\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</tbody>\n");
    printf("</table>\n");
    printf("<br>\n");
    printf("<table style=\"text-align: left; width: 778;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n");
    printf("<tbody>\n");
    printf("<tr>\n");
    printf("<td>\n");
    printf("<h3 class=\"box\">\n");
    printf("WIFI Port Configure</h3>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td>\n");
    printf("<form name=\"wifi\" method=\"get\" action=\"wifi.cgi\">\n");
    printf("<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"2\">\n");
    printf("<tbody>\n");
    printf("<tr>\n");
    printf("<td width=\"23%\" class=\"gr\">\n");
    printf("</td>\n");
    printf("<td colspan=\"2\" class=\"logingr\">\n");
    printf("<div align=\"center\">\n");
    printf("Current</div>\n");
    printf("</td>\n");
    printf("<td width=\"40%\" class=\"logingr\">\n");
    printf("<div align=\"center\">\n");
    printf("Updated</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"Local socket port number, this port number is required between 1 and 65535\">\n");
    printf("Local Socket Port:</span>\n");
    printf("</td>\n");
    printf("<td width=\"1%\">\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td width=\"36%\">\n");
    printf("<a id=\"psocketlp\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<input name=\"socketlp\" class=\"port\" size=\"6\" maxlength=\"5\">\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"Remote socket  port number, this port number is required between 1 and 65535.\">\n");
    printf("Remote Socket Port:</span>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"psocketrp\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<input name=\"socketrp\" class=\"port\" size=\"6\" maxlength=\"5\">\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"Socket work mode. if you select TCP protocol, you should choose Server or Client, otherwise, Ignore it.\">\n");
    printf("Socket Mode:</span>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"psocketmode\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<select name=\"socketmode\" size=\"1\" class=\"list\" onChange=\"SetWIFIState();\">\n");
    printf("<option value=\"0\">\n");
    printf("Server</option>\n");
    printf("<option value=\"1\">\n");
    printf("Client</option>\n");
    printf("</select>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"Socket protocol. You can choose TCP or UDP.\">\n");
    printf("Socket Protocol:</span>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"psocketprol\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<select name=\"socketprol\" size=\"1\" class=\"list\" onChange=\"SetWIFIState();\">\n");
    printf("<option value=\"0\">\n");
    printf("TCP</option>\n");
    printf("<option value=\"1\">\n");
    printf("UDP</option>\n");
    printf("</select>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("<span title=\"Romote server IP. If you select TCP&Client or UDP, you should enter remote server IP. Otherwise, S2WIFI module will not work.\">\n");
    printf("Remote Server IP:</span>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<a id=\"psocketrip\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<input name=\"socketrip\" class=\"ip\" maxlength=\"15\">\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td class=\"gr\">\n");
    printf("Bind MAC Address:</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("<td>\n");
    printf("<input name=\"bindmac\" type=\"text\" disabled=\"disabled\" class=\"mac\" value=\"FF-FF-FF-FF-FF-FF\" size=\"17\" maxlength=\"17\">\n");
    printf("<input name=\"isbindmac\" type=\"checkbox\" value=\"checkbox\" disabled=\"disabled\">\n");
    printf("Bind</td>\n");
    printf("</tr>\n");
    printf("</tbody>\n");
    printf("</table>\n");
    printf("</form>\n");
    printf("<div align=\"center\">\n");
    printf("<table width=\"778\" border=\"0\">\n");
    printf("<tr>\n");
    printf("<td width=\"654\">\n");
    printf("<div align=\"right\">\n");
    printf("<input type=\"submit\" class=\"button\" id=\"wifibtn\" onClick=\"do_wifi_apply();\" value=\"Save\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("<td width=\"69\">\n");
    printf("<div align=\"right\">\n");
    printf("<input type=\"submit\" class=\"button\" onClick=\"do_wifi_reset();\" value=\"Reset\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("<td width=\"41\">\n");
    printf("&nbsp;\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</table>\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</tbody>\n");
    printf("</table>\n");
    printf("<p align=\"center\">\n");
    printf("<span class=\"STYLE10\">\n");
    printf("<span title=\"Reset S2WIFI module. Reboot will take a few seconds, usually is 30s.\">\n");
    printf("<input type=\"submit\" class=\"btn\" onClick=\"do_module_reset();\" value=\"Finish\">\n");
    printf("</span>\n");
    printf("</p>\n");
    printf("<hr width=\"778\" size=\"4\" class=\"STYLE10\" />\n");
    printf("<table style=\"text-align: left; width: 778;\" border=\"0\" cellpadding=\"2\" cellspacing=\"2\">\n");
    printf("<tbody>\n");
    printf("<tr>\n");
    printf("<td>\n");
    printf("<div align=\"center\" class=\"STYLE7\">\n");
    printf("2011 (C) Copyright <a href=\"http://www.midea.com\" target=\"_blank\">\n");
    printf("www.midea.com</a>\n");
    printf(", All right reserved</div>\n");
    printf("<div align=\"center\" class=\"STYLE5\">\n");
    printf("</div>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</tbody>\n");
    printf("</table>\n");
    printf("</body>\n");
    printf("</html>\n");

    return(1);
}

/**
 * Close the Doxygen group.
 * @}
 */

