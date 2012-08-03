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
#include "config.h"
#include "debug.h"
#include "shm.h"

struct s2wifi_config s2wifi_cfg;

int fn_get_parameter(char *buf)
{
    int ret;
    sscanf(buf,"vssid=%s",s2wifi_cfg.vssid);
    sscanf(buf,"vencrytype=%d",&(s2wifi_cfg.vencrytype));
    sscanf(buf,"vwifipassword=%s",s2wifi_cfg.vwifipassword);
    sscanf(buf,"vstaticip=%d",&(s2wifi_cfg.vstaticip));
    sscanf(buf,"vwlansip=%s",s2wifi_cfg.vwlansip);
    sscanf(buf,"vwlanmip=%s",s2wifi_cfg.vwlanmip);
    sscanf(buf,"vwlangip=%s",s2wifi_cfg.vwlangip);
    sscanf(buf,"vwlandip=%s",s2wifi_cfg.vwlandip);
    sscanf(buf,"vsocketlp=%d",&(s2wifi_cfg.vsocketlp));
    sscanf(buf,"vsocketrp=%d",&(s2wifi_cfg.vsocketrp));
    sscanf(buf,"vsocketmode=%d",&(s2wifi_cfg.vsocketmode));
    sscanf(buf,"vsocketprol=%d",&(s2wifi_cfg.vsocketprol));
    sscanf(buf,"vsocketrip=%s",s2wifi_cfg.vsocketrip);
    sscanf(buf,"vbaudrate=%d",&(s2wifi_cfg.vbaudrate));
    sscanf(buf,"vdatasize=%d",&(s2wifi_cfg.vdatasize));
    sscanf(buf,"vparity=%d",&(s2wifi_cfg.vparity));
    sscanf(buf,"vstop=%d",&(s2wifi_cfg.vstop));
    sscanf(buf,"vflow=%d",&(s2wifi_cfg.vflow));
}


int fn_printf_config_information(struct s2wifi_config cfg)
{
    int ret;
    #if (CONFIG_DEBUG >= 1)
    debug_out("\nnow printf all config information start\n");
    debug_out("vssid=%s\n",cfg.vssid);
    debug_out("vencrytype=%d\n",(cfg.vencrytype));
    debug_out("vwifipassword=%s\n",cfg.vwifipassword);
    debug_out("vstaticip=%d\n",cfg.vstaticip);
    debug_out("vwlansip=%s\n",cfg.vwlansip);
    debug_out("vwlanmip=%s\n",cfg.vwlanmip);
    debug_out("vwlangip=%s\n",cfg.vwlangip);
    debug_out("vwlandip=%s\n",cfg.vwlandip);
    debug_out("vsocketlp=%d\n",(cfg.vsocketlp));
    debug_out("vsocketrp=%d\n",(cfg.vsocketrp));
    debug_out("vsocketmode=%d\n",(cfg.vsocketmode));
    debug_out("vsocketprol=%d\n",(cfg.vsocketprol));
    debug_out("vsocketrip=%s\n",cfg.vsocketrip);
    debug_out("vbaudrate=%d\n",(cfg.vbaudrate));
    debug_out("vdatasize=%d\n",(cfg.vdatasize));
    debug_out("vparity=%d\n",(cfg.vparity));
    debug_out("vstop=%d\n",(cfg.vstop));
    debug_out("vflow=%d\n",(cfg.vflow));
    debug_out("now printf all config information end\n");
    #endif
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
int fn_get_config (struct s2wifi_config *pcfg)
{
    memset((char*)(&s2wifi_cfg),0,sizeof(s2wifi_cfg));

    FILE *fp;
    fp = fopen(M_S2WIFI_CONFIG_CFG_PATH,"rw");
    if(fp == NULL)
    {
        #if (CONFIG_DEBUG >= 1)
        debug_out("fopen M_FILE_S2WIFI_CONFIG_PATH failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (CONFIG_DEBUG >= 3)
        debug_out("fopen M_FILE_S2WIFI_CONFIG_PATH sucess\n");
        #endif
    }

    char buf[255];
    int ret;
    char *ret_buf;
    int get_para_right_cnt = 0;
    while(1)
    {
        ret_buf = fgets(buf,sizeof(buf),fp);
        if(ret_buf == NULL)
        {
            break;
        }
        ret = fn_get_parameter(buf);
        if(ret != -1)
        {
            get_para_right_cnt++;
        }
    }
    #if (CONFIG_DEBUG >= 3)
    debug_out("get_para_right_cnt = %d\n",get_para_right_cnt);
    #endif

    fclose(fp);

    if(pcfg != NULL)
    {
        memcpy((char *)pcfg,(char*)(&s2wifi_cfg),sizeof(s2wifi_cfg));
        pcfg->config_change = 1;
    }
    else
    {
        #if (CONFIG_DEBUG >= 3)
        debug_out("pcfg is NULL\n");
        #endif

    }
    return 0;
}
