//----------------------------------------------------------------------------
//
//
//                                  源文件(*.h)
//
//
//                    版权所有(C)2005-2010 利尔达科技有限公司
//
//
// 文件名    : template.h
// 作者      : nmy
// 生成日期  : 2010-09-11
//
// arm gcc   : arm-none-linux-gnueabi-gcc 4.5.3
//
// 版本记录  : V1.00  创建第一版   2010-09-11 15:30
//
//-----------------------------------------------------------------------------


#ifndef __CONFIG_H__
#define __CONFIG_H__


extern unsigned char u8_config_com_number;
extern unsigned int u16_config_com_baudrate;
extern unsigned char u8_config_com_databit;
extern char u8_config_com_paritybit;
extern unsigned char u8_config_com_stopbit;
extern char pu8_socket_localip[15];
extern char pu8_socket_serverip[15];
extern unsigned int u8_socket_serverport;

extern unsigned char u8_sdcard_exist;
extern unsigned char u8_sdcard_configini_exist;


extern int fn_get_sdcard_exist(void);
extern int fn_get_sdcard_configini_exist(void);
extern int fn_get_config (void);
extern int fn_printf_config_information(void);

#define M_S2WIFI_CONFIG_KEY 0x12340000
#define M_S2WIFI_CONFIG_KEY_STR "0x12340000"

#define M_S2WIFI_CONFIG_CFG_PATH "/media/mmcblk0p1/config.ini"


#endif
