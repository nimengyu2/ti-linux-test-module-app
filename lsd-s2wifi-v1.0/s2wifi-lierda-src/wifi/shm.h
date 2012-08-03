//---------------------------------------------------------------------------
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


#ifndef __SHM_H__
#define __SHM_H__

// 指向s2wifi的配置结构体
extern struct s2wifi_config *s2wifi_config_uart;

// 指向s2wifi的配置结构体
extern struct s2wifi_config *s2wifi_config_wifi;

extern int fn_uart_shm_init(void);
extern int fn_uart_shm_test(void);
extern int fn_wifi_shm_init(void);
extern int fn_wifi_shm_test(void);
extern int fn_shm_delete(void);


#endif
