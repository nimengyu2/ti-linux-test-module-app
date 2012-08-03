//-----------------------------------------------------------------------------
//
//                                 timer中断程序
//                                  源文件(*.c)
//
//
//                    版权所有(C)2005-2010 利尔达科技有限公司
//
//
// 文件名    : template.c
// 作者      : nmy
// 生成日期  : 2011-09-21
//
// arm gcc   : arm-none-linux-gnueabi-gcc 4.5.3
//
// 版本记录  : V1.00  创建第一版   2010-09-11 15:30
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "wifi_process.h"
#include "config.h"
#include "debug.h"
#include "shm.h"
#include "create_wpa_cfg.h"
#include "wifi_mode.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "wifi_sta_ap.h"

#define M_WIFI_AP_STA_SETTING_DO_MUCH  1

//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  负责连接wifi到路由器
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_wifi_deal_connect_route(void)
{
    int ret;
    char wifi_cmd_string[255];

    // 创建文件wpa_supplicant
    ret = system("mkdir -p /var/run/wpa_supplicant ");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: mkdir -p /var/run/wpa_supplicant  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: mkdir -p /var/run/wpa_supplicant  ok\n");
        #endif
    }
    printf("\n\n");


    // 卸载rt3070 sta模式模块
    #if(M_WIFI_AP_STA_SETTING_DO_MUCH > 0)
    system("pidof wpa_supplicant | xargs kill");
    system("pidof udhcpd | xargs kill");
    system("ifconfig ra0 down");
    system("rmmod rtnet3070ap");
    system("rmmod rt3070ap");
    system("rmmod rtutil3070ap");
    system("rmmod rt3070sta");
    printf("\n\n");
    #endif

    // 加载rt3070 sta模式模块
    ret = system("modprobe rt3070sta");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: modprobe rt3070sta-wep  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d:%s]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__,__FILE__);
        debug_out("cmd: modprobe rt3070sta-wep  ok\n");
        #endif
    }
    printf("\n\n");

#if 0
    // 关闭eth0
    ret = system("ifconfig eth0 down");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: ifconfig eth0 down  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: ifconfig eth0 down  ok\n");
        #endif
    }
    printf("\n\n");
#endif

#if 0
    // 设置ra0 up
    ret = system("ifconfig ra0 up");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: ifconfig ra0 up  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: ifconfig ra0 up  ok\n");
        #endif
    }
    printf("\n\n");
#endif

    // 配置wpa
    ret = system("wpa_supplicant -B -ira0 -c /etc/wpa_supplicant.conf -Dwext ");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: wpa_supplicant -B -ira0 -c /etc/wpa_supplicant.conf -Dwext  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: wpa_supplicant -B -ira0 -c /etc/wpa_supplicant.conf -Dwext  ok\n");
        #endif
    }
    printf("\n\n");

    // 根据ip地址获取模式选择是dhcp还是静态ip地址
    if(s2wifi_cfg.vstaticip == 1)
    {
        snprintf(wifi_cmd_string,sizeof(wifi_cmd_string),"ifconfig ra0 %s",s2wifi_cfg.vwlansip);
    }
    else
    {
        snprintf(wifi_cmd_string,sizeof(wifi_cmd_string),"udhcpc -i ra0");
    }
    ret = system(wifi_cmd_string);
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: %s  failed\n",wifi_cmd_string);
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: %s  ok\n",wifi_cmd_string);
        #endif
    }
    printf("\n\n");

    // 设定默认的网关地址
    snprintf(wifi_cmd_string,sizeof(wifi_cmd_string),"route add default gw %s",s2wifi_cfg.vwlangip);
    ret = system(wifi_cmd_string);
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: %s  failed\n",wifi_cmd_string);
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: %s  ok\n",wifi_cmd_string);
        #endif
    }
    printf("\n\n");

#if 1
    // 测试ping网关地址
    snprintf(wifi_cmd_string,sizeof(wifi_cmd_string),"ping %s -c 8 &",s2wifi_cfg.vwlangip);
    ret = system(wifi_cmd_string);
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: %s  failed\n",wifi_cmd_string);
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: %s  ok\n",wifi_cmd_string);
        #endif
    }
#endif

    return 0;
}


//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  负责设置wifi为AP
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_wifi_deal_connect_as_ap(void)
{
    int ret;
    // 卸载rt3070 sta模式模块
#if(M_WIFI_AP_STA_SETTING_DO_MUCH > 0)
    system("ifconfig eth0 down");
    system("pidof wpa_supplicant | xargs kill");
    system("pidof udhcpd | xargs kill");
    system("ifconfig ra0 down");
    system("rmmod rtnet3070ap");
    system("rmmod rt3070ap");
    system("rmmod rtutil3070ap");
    system("rmmod rt3070sta");
    printf("\n\n");
#endif

    // 加载rtutil3070ap模式模块
    ret = system("modprobe rtutil3070ap");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: modprobe rtutil3070ap  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d:%s]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__,__FILE__);
        debug_out("cmd: modprobe rtutil3070ap  ok\n");
        #endif
    }
    printf("\n\n");

    // 加载rt3070ap模式模块
    ret = system("modprobe rt3070ap");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: modprobe rt3070ap  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d:%s]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__,__FILE__);
        debug_out("cmd: modprobe rt3070ap  ok\n");
        #endif
    }
    printf("\n\n");

    // 加载rtnet3070ap模式模块
    ret = system("modprobe rtnet3070ap");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: modprobe rtnet3070ap  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d:%s]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__,__FILE__);
        debug_out("cmd: modprobe rtnet3070ap ok\n");
        #endif
    }
    printf("\n\n");

    // 设置ra0 up
    ret = system("ifconfig ra0 up");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: ifconfig ra0 up  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__);

        debug_out("cmd: ifconfig ra0 up  ok\n");
        #endif
    }
    printf("\n\n");

    // 设置ra0  ifconfig ra0 192.168.1.1
    ret = system("ifconfig ra0 192.168.1.1");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: ifconfig ra0 192.168.1.1  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: ifconfig ra0 192.168.1.1  ok\n");
        #endif
    }
    printf("\n\n");

    // udhcpd
    ret = system("udhcpd");
    if(ret == -1)
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 1)
        //printf("[%s:%s:%d]  cmd fail\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: udhcpd  failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (WIFI_CONNECT_ROUTER_DEBUG >= 2)
        //printf("[%s:%s:%d]  cmd ok\n",__FILE__,__FUNCTION__,__LINE__);
        debug_out("cmd: udhcpd  ok\n");
        #endif
    }
    printf("\n\n");

    return 0;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  running正常链接1,断开返回-1
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_get_net_running_stat(void)
{
    char    buffer[BUFSIZ];
    FILE    *read_fp;
    int        chars_read;
    int        ret;

    memset( buffer, 0, BUFSIZ );
    read_fp = popen("ifconfig ra0 | grep RUNNING", "r");
    if ( read_fp != NULL )
    {
        chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp);
        if (chars_read > 0)
        {
            ret = 1;
        }
        else
        {
            ret = -1;
        }
        pclose(read_fp);
    }
    else
    {
        ret = -1;
    }

    return ret;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  如果正确获取到IP地址或者设定好IP地址的
//             时候就返回1,否则返回-1
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_get_net_inet_addr_stat(void)
{
    char    buffer[BUFSIZ];
    FILE    *read_fp;
    int        chars_read;
    int        ret;

    memset( buffer, 0, BUFSIZ );
    read_fp = popen("ifconfig ra0 | grep inet", "r");
    if ( read_fp != NULL )
    {
        chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp);
        if (chars_read > 0)
        {
            ret = 1;
        }
        else
        {
            ret = -1;
        }
        pclose(read_fp);
    }
    else
    {
        ret = -1;
    }

    return ret;
}

