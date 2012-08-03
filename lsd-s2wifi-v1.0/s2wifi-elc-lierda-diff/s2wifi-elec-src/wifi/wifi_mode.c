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
#include "wifi_mode.h"
#include "debug.h"

//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  如果处于AP模式,则返回1,客户端模式返回0
//          错误返回-1
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_wifi_mode_read(void)
{
    FILE *fp;
    fp = fopen(M_WIFI_MODE_PATH,"r");

    char buf[128];
    char wifi_mode[10];
    fgets(buf,sizeof(buf),fp);
    fclose(fp);
    sscanf(buf,"wifi_mode=%s",wifi_mode);
    int ret;
    ret = strcmp(wifi_mode,"AP");
    if(ret == 0)
    {
        #if ( WIFI_MODE_DEBUG >= 3)
        debug_out("wifi mode read is AP mode\n");
        #endif
        return 1;
    }
    ret = strcmp(wifi_mode,"STA");
    if(ret == 0)
    {
        #if ( WIFI_MODE_DEBUG >= 3)
        debug_out("wifi mode read is STA mode\n");
        #endif
        return 0;
    }
    return -1;
}


//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_wifi_mode_write (int wifi_mode)
{
        FILE *fp;
    fp = fopen(M_WIFI_MODE_PATH,"w+");
    if(wifi_mode == 0)
    {
        fprintf(fp,"wifi_mode=STA\n");
        #if ( WIFI_MODE_DEBUG >= 3)
        debug_out("wifi mode write STA mode\n");
        #endif
    }
    else if(wifi_mode == 1)
    {
        fprintf(fp,"wifi_mode=AP\n");
        #if ( WIFI_MODE_DEBUG >= 3)
        debug_out("wifi mode write AP mode\n");
        #endif
    }
    fclose(fp);
    return 0;
}
