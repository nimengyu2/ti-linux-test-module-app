//-----------------------------------------------------------------------------
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


#ifndef __MTD_OPS_H__
#define __MTD_OPS_H__



#define M_CFG_MTD_DEVICE_PATH   "/dev/mtd4"
#define M_CFG_BLOCK_WIFI_MODE_CONF  	1
#define M_CFG_BLOCK_S2WIFI_CFG  	2
#define M_CFG_BLOCK_LOGIN_CFG  		3

extern unsigned char g_pu8_flush_mtd_buf_wifi_mode_conf[64];
extern unsigned char g_pu8_flush_mtd_buf_login_cfg[1024];
extern unsigned char g_pu8_flush_mtd_buf_s2wifi_cfg[4096];

extern int fn_mtd_read(unsigned long  file,unsigned char* read_buf,unsigned int cnt);
extern int fn_mtd_write(unsigned long  file,unsigned char* write_buf,unsigned int cnt);
extern char fn_write_s2wifi_config_to_mtd(void);

extern void debug_out(const char *format, ...);
extern void debug_open(void);
extern void debug_close(void);



#endif
