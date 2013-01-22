//-----------------------------------------------------------------------------
//
//
//                                  源文件(*.h)
//
//
//                    版权所有(C)2005-2010 利尔达科技有限公司
//
//
// 文件名    : tcp_client.h
// 作者      : nmy
// 生成日期  : 2010-09-11
//
// arm gcc   : arm-none-linux-gnueabi-gcc 4.5.3
//
// 版本记录  : V1.00  创建第一版   2010-09-11 15:30
//
//-----------------------------------------------------------------------------


#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

extern int tcp_client_init (void);
extern int tcp_client_connecting (void);
extern int tcp_client_read_data (unsigned char *buf,int len);
extern int tcp_client_write (unsigned char* buf,int len);
extern int tcp_client_close (void);



#endif
