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


extern unsigned char u8_sdcard_exist;
extern unsigned char u8_sdcard_updateini_exist;

#define M_S2WIFI_CONFIG_CFG_PATH "/media/mmcblk0p1/update.ini"

int fn_update(void);


#endif
