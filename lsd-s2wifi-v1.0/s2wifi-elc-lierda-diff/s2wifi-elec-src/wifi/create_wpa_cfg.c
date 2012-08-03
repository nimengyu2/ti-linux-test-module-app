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
#include "debug.h"
#include "config.h"
#include "create_wpa_cfg.h"


// wifi none 配置
int fn_create_encrytype_none_cfg(FILE *fp)
{
    fprintf(fp,"ctrl_interface=/var/run/wpa_supplicant\n");
    fprintf(fp,"network={ \n");
    fprintf(fp,"ssid=\"%s\"\n",s2wifi_cfg.vssid);
    fprintf(fp,"key_mgmt=NONE\n");
    fprintf(fp,"}\n");
}

// wifi wep open 配置
int fn_create_encrytype_wep_open_cfg(FILE *fp)
{
    fprintf(fp,"ctrl_interface=/var/run/wpa_supplicant\n");
    fprintf(fp,"network={ \n");
    fprintf(fp,"ssid=\"%s\"\n",s2wifi_cfg.vssid);
    fprintf(fp,"key_mgmt=NONE\n");
    fprintf(fp,"wep_key0=\"%s\"\n",s2wifi_cfg.vwifipassword);
    fprintf(fp,"wep_key1=0102030405\n");
    fprintf(fp,"wep_key2=\"1234567890123\"\n");
    fprintf(fp,"wep_tx_keyidx=0\n");
    fprintf(fp,"priority=5\n");
    fprintf(fp,"}\n");
}

// wifi wep share 配置
int fn_create_encrytype_wep_share_cfg(FILE *fp)
{
    fprintf(fp,"ctrl_interface=/var/run/wpa_supplicant\n");
    fprintf(fp,"network={ \n");
    fprintf(fp,"ssid=\"%s\"\n",s2wifi_cfg.vssid);
    fprintf(fp,"key_mgmt=NONE\n");
    fprintf(fp,"wep_key0=\"%s\"\n",s2wifi_cfg.vwifipassword);
    fprintf(fp,"wep_key1=0102030405\n");
    fprintf(fp,"wep_key2=\"1234567890123\" \n");
    fprintf(fp,"wep_tx_keyidx=0  \n");
    fprintf(fp,"priority=5\n");
    fprintf(fp,"auth_alg=SHARED\n");
    fprintf(fp,"}\n");
}


// wifi wpa 配置
int fn_create_encrytype_wpa_cfg(FILE *fp)
{
    fprintf(fp,"ctrl_interface=/var/run/wpa_supplicant\n");
    fprintf(fp,"network={ \n");
    fprintf(fp,"ssid=\"%s\"\n",s2wifi_cfg.vssid);
    fprintf(fp,"scan_ssid=1 \n");
    fprintf(fp,"key_mgmt=WPA-EAP WPA-PSK IEEE8021X NONE \n");
    fprintf(fp,"pairwise=TKIP CCMP\n");
    fprintf(fp,"group=CCMP TKIP WEP104 WEP40\n");
    fprintf(fp,"psk=\"%s\"\n",s2wifi_cfg.vwifipassword);
    fprintf(fp,"}\n");
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
int fn_create_wpa_cfg (void)
{
    FILE *fp;
    fp = fopen(M_WPA_CFG_PATH,"w+");
    if(fp == NULL)
    {
        #if (WPA_CFG_DEBUG >= 3)
        debug_out("open wpa cfg file fail\n");
        #endif
    }
    else
    {
        #if (WPA_CFG_DEBUG >= 3)
        debug_out("open wpa cfg file success\n");
        #endif
    }
    // Encryption type. 0:None/1:WEP-open/2:WEP-share/3:WPA or WPA2.
    switch (s2wifi_cfg.vencrytype)
    {
        case 0:
        fn_create_encrytype_none_cfg(fp);
        break;
        case 1:
        fn_create_encrytype_wep_open_cfg(fp);
        break;
        case 2:
        fn_create_encrytype_wep_share_cfg(fp);
        break;
        case 3:
        fn_create_encrytype_wpa_cfg(fp);
        break;
        default:
        break;
    }
    fclose(fp);
    return 0;
}
