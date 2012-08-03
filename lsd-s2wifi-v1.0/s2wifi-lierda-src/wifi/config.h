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

struct s2wifi_config
{
    char vssid[32+1];// 需要连接的路由器的SSID
    int vencrytype;// 加密格式
    char vwifipassword[63+1]; // wifi密码
    int vstaticip;// 选择静态IP或者动态获取
    char vwlansip[15+1];// wlan 静态IP地址
    char vwlanmip[15+1];// wlan 子网掩码
    char vwlangip[15+1];// wlan 网关
    char vwlandip[15+1];// wlan dns服务器
    int vsocketlp;// 本地socket端口号
    int vsocketrp;// 远程socket端口号
    int vsocketmode;// socket模式 客户端或者服务器
    int vsocketprol;// socket协议 tcp或者udp
    char vsocketrip[15+1];// 远程要连接的socket ip地址
    int vbaudrate;// 波特率
    int vdatasize;// 数据位大小
    int vparity;// 奇偶校验位
    int vstop;// 停止位
    int vflow;  // 流控制

    // 下面是不是属于配置内容中的
    // 用于联合调试用的，如果config_change=1 表示发生过改变
    // 在被读取改变的时候被清零
    int config_change;
    // 如果处于AP模式,则是1,客户端模式则是0
    int wifi_mode;

    int wifi_process_init_ok;
};

extern struct s2wifi_config s2wifi_cfg;
//extern int fn_get_parameter(char *buf);
extern int fn_printf_config_information(struct s2wifi_config cfg);
extern int fn_get_config (struct s2wifi_config *pcfg);

#define M_S2WIFI_CONFIG_KEY 0x12340000
#define M_S2WIFI_CONFIG_KEY_STR "0x12340000"

#define M_S2WIFI_CONFIG_CFG_PATH "/var/www/cgi-bin/s2wifi.cfg"
#define M_S2WIFI_LOGIN_CFG_PATH "/var/www/cgi-bin/login.cfg"

#endif
