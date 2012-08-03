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


#ifndef __WIFI_PROCESS_H__
#define __WIFI_PROCESS_H__

#define M_WIFI_NET_STATE_NOLINK_ROUTE       0
#define M_WIFI_NET_STATE_CONNECTED_ROUTE    1
#define M_WIFI_NET_STATE_CLIENT_CONECTING   3
#define M_WIFI_NET_STATE_CLIENT_CONECTED    4
#define M_WIFI_NET_STATE_SERVER_ACCEPT      5
#define M_WIFI_NET_STATE_SERVER_CONECTED    6
#define M_WIFI_NET_STATE_UDP                7

#define M_WIFI_PROCESS_USLEEP_CNT_MS  10

#define M_RING_BUF_SIZE 1024

extern int wifi_process (void);



#endif
