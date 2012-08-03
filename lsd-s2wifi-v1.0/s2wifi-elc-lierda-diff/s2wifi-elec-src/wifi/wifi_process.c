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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netdb.h> 
#include <sys/utsname.h> 
#include <netinet/in.h> 
#include <linux/types.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/ioctl.h> 
#include <netinet/in.h> 
#include <net/if.h> 
#include <net/if_arp.h> 
#include <arpa/inet.h> 
#include <errno.h> 
#include "wifi_process.h"
#include "config.h"
#include "debug.h"
#include "shm.h"
#include "create_wpa_cfg.h"
#include "wifi_mode.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "wifi_sta_ap.h"
#include "ringbuf.h"
#include "udp.h"
#include "led.h"

unsigned char pu8_wifi_to_uart_data[M_RING_BUF_SIZE];
tRingBufObject st_wifi_to_uart_data;

unsigned char pu8_uart_to_wifi_data[M_RING_BUF_SIZE];
tRingBufObject st_uart_to_wifi_data;

unsigned char pu8_wifi_read_buf[1024];
unsigned char pu8_wifi_write_buf[1024];
unsigned int  u32_wifi_time_ms_cnt = 0;
int  s32_wifi_read_cnt;

char wifi_cmd_string[255];

// wifi 模式改变标志位 如果这个标志位置1则重新设定wifi
int wifi_mode_change_flag = 0;

int wifi_mode_init_flag = 1;

// wifi模式     0 表示客户端   1 表示AP
int wifi_mode;

// wifi连接上路由标志位
int wifi_connected_route_flag = 0;

// 该标志永远 在已经连接上路由的时候,
// 此时开始连接pc的时候使用,一般情况下
// 只是用一次
int wifi_start_connect_to_pc_flag = 0;

// wifi 网络状态标志位
int wifi_net_state = M_WIFI_NET_STATE_NOLINK_ROUTE;

// wifi连接上pc端标志
int wifi_connected_pc_stat_flag = 0;

// wifi连接进程pid
pid_t wifi_connect_route_pid;


int am1808_uart_recv_data_flag = 0;
int am1808_uart_send_data_flag = 0;

unsigned char g_pu8_local_mac_addr[6];

//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  负责wifi的连接功能或者AP模式
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------

int fn_wifi_uart_data_ringbuf_init(void)
{
    RingBufInit(&st_wifi_to_uart_data, pu8_wifi_to_uart_data,
                               sizeof(pu8_wifi_to_uart_data));
    RingBufInit(&st_uart_to_wifi_data, pu8_uart_to_wifi_data,
                               sizeof(pu8_uart_to_wifi_data));
}

//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  负责wifi的连接功能或者AP模式
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_wifi_deal_connect_route_and_ap_mode(void)
{
    // 客户端模式,wifi去连接路由器
    if(wifi_mode == 0)
    {
        // 设定wifi为client
        fn_wifi_deal_connect_route();
    }
    // AP模式 wifi做路由器
    else if(wifi_mode == 1)
    {
        // 设定wifi为ap模式
        fn_wifi_deal_connect_as_ap();
    }
    // 配置完成之后就等待
    while(1)
    {
        sleep(1);
    }
}

// 注意,该宏定义载stdio.h中已经定义了
//#define BUFSIZ 1024
int s32_wifi_net_measure_cnt = 0;
int s32_wifi_net_running_stat = 0;
int s32_wifi_net_inet_addr_stat = 0;

//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  该函数用于接受和处理从串口接受的数据
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_wifi_deal_uart_data_to_net(void)
{
    int ringbuf_free;
    int write_num;
    int wifi_data_num;
#if 0
    #if (PIPE_DEBUG >= 1)
    // 管道读写测试，这里是两个管道，供读写
    if(u32_wifi_time_ms_cnt > 2000)
    {
        u32_wifi_time_ms_cnt = 0;

        fn_wifi_pipe_write("wifi write \n",strlen("wifi write \n"));
    }
    else
    {

        u32_wifi_time_ms_cnt += 100;
    }
    s32_wifi_read_cnt = fn_wifi_pipe_read(
                pu8_wifi_read_buf,sizeof(pu8_wifi_read_buf));

    if(s32_wifi_read_cnt > 0)
    {
        debug_out("wifi receive str is:\n%s",pu8_wifi_read_buf);
    }
    #endif

    #if (SHM_DEBUG >= 2)
    // 共享内存写测试
    //fn_wifi_shm_test();
    #endif
#endif

// 测试uart转wifi功能
#if 1
    // 读取uart传递过来的数据,并进行相应的处理
    s32_wifi_read_cnt = fn_wifi_pipe_read(
                pu8_wifi_read_buf,sizeof(pu8_wifi_read_buf));
    if(s32_wifi_read_cnt > 0)
    {
        am1808_uart_recv_data_flag = 1;
 #if (UART_DEBUG >= 3)
        debug_out("[uart->wifi] in wifi_process str is: %s\n",pu8_wifi_read_buf);
        debug_out("[uart->wifi] in wifi_process hex is: ");
        int i;
        for(i = 0;i<s32_wifi_read_cnt ;i++)
        {
            debug_out("0x%02x ",pu8_wifi_read_buf[i]);
        }
        debug_out("\n\n");
        #endif

        #if 1
        // 设定为AP模式,设定条件 只有收到+++ 三个字节,必须是三个字节,多一个也不行
        // 并且当前的目录处于服务器模式的时候才会切换到客户端模式的
        if((pu8_wifi_read_buf[0] == '+') && (pu8_wifi_read_buf[1] == '+') && (pu8_wifi_read_buf[2] == '+') && (s32_wifi_read_cnt == 3)&&(wifi_mode == 0))
        {
            snprintf(wifi_cmd_string,sizeof(wifi_cmd_string),"kill %d",wifi_connect_route_pid);
            #if (UART_DEBUG >= 3)
            debug_out("change to ap mode,kill the wifi process ,do cmd %s",wifi_cmd_string);
            #endif
            system(wifi_cmd_string);

            wifi_mode = 1;
            wifi_mode_change_flag = 1;
        }
        // 设定为客户端模式  设定条件 只有收到--- 三个字节,必须是三个字节,多一个也不行
        // 并且当前的目录处于客户端模式的时候才会切换到AP模式的
        else if((pu8_wifi_read_buf[0] == '-') && (pu8_wifi_read_buf[1] == '-') && (pu8_wifi_read_buf[2] == '-') && (s32_wifi_read_cnt == 3)&&(wifi_mode == 1))
        {
            snprintf(wifi_cmd_string,sizeof(wifi_cmd_string),"kill %d",wifi_connect_route_pid);
            #if (UART_DEBUG >= 3)
            debug_out("change to client mode,kill the wifi process ,do cmd %s",wifi_cmd_string);
            #endif
            system(wifi_cmd_string);

            wifi_mode = 0;
            wifi_mode_change_flag = 1;
        }
        else
        {
            ringbuf_free = RingBufFree(&st_uart_to_wifi_data);
            if(ringbuf_free < s32_wifi_read_cnt)
            {
                write_num  = ringbuf_free;
            }
            else
            {
                write_num = s32_wifi_read_cnt;
            }
            RingBufWrite(&st_uart_to_wifi_data,pu8_wifi_read_buf,write_num);
        }
        #endif
    }
#endif

    // 如果有wifi-net发送过来的数据,则通过pipe传给串口
    wifi_data_num = RingBufUsed(&st_wifi_to_uart_data);
    if(wifi_data_num > 0)
    {
 	am1808_uart_send_data_flag = 1;
 	RingBufRead(&st_wifi_to_uart_data,pu8_wifi_write_buf,wifi_data_num);
        fn_wifi_pipe_write(pu8_wifi_write_buf,wifi_data_num);
    }



}


//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  该函数用于扫描网络是否连接的到路由器上
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_wifi_scan_net_connect_to_route(void)
{
    static int wifi_stable_cnt;
    // 用来检测net链接状态 大概1s扫描一次
    s32_wifi_net_measure_cnt += M_WIFI_PROCESS_USLEEP_CNT_MS;
    if(s32_wifi_net_measure_cnt >= 1000)
    {
        s32_wifi_net_measure_cnt = 0;
        // 获取ifconfig ra0 中是否有running运行状态

        s32_wifi_net_running_stat  = fn_get_net_running_stat();
        // 获取ifconfig ra0 中时候有IP地址,如果模块被卸载掉的时候
        // IP会自动清除
        s32_wifi_net_inet_addr_stat  = fn_get_net_inet_addr_stat();

        // 是否网络连接成功
        if((s32_wifi_net_running_stat  == 1) && (s32_wifi_net_inet_addr_stat == 1))
        {
            if(wifi_mode == 0)
            {
                // 如果之前是没有连接连接到路由器上的,则开始连接pc
                //if(wifi_connected_route_flag == 0)
                //{
                //  wifi_start_connect_to_pc_flag = 1;
                //}
                // 已经成功连接路由器
                //wifi_connected_route_flag  = 1;
                #if (WIFI_DEBUG >= 3)
                //debug_out("wifi net link ok now\n");
                #endif
                if(wifi_net_state == M_WIFI_NET_STATE_NOLINK_ROUTE )
                {
                    // 等待稳定后开始配置socket
                    wifi_stable_cnt++;
                    if(wifi_stable_cnt >= 3)
                    {
                        wifi_stable_cnt = 0;

			// for electrolux
			// vsocketmode   0:Server/1:Client.
                	// vsocketprol  Socket Protocol. 0:TCP/1:UDP.
                	// 为tcp协议 服务器
			s2wifi_cfg.vsocketmode = 1;  // client
			s2wifi_cfg.vsocketprol = 0;  // tcp
			#if (WIFI_DEBUG >= 3)
                        debug_out("First:s2wifi_cfg.vsocketrip = %s\n",s2wifi_cfg.vsocketrip);
                        #endif
			//memcpy(s2wifi_cfg.vsocketrip,"192.168.1.103",strlen("192.168.1.103"));
			//s2wifi_cfg.vsocketrp = 5000;	
			memcpy(s2wifi_cfg.vsocketrip,"222.214.216.227",strlen("222.214.216.227"));
			s2wifi_cfg.vsocketrp = 80;			

                        wifi_net_state = M_WIFI_NET_STATE_CONNECTED_ROUTE;
                        #if (WIFI_DEBUG >= 3)
                        debug_out("wifi start to config socket\n");
                        #endif
                    }
                }
            }
            #if (WIFI_DEBUG >= 3)
            //debug_out("wifi net  link now and wifi_connected_route_flag = %d\n",wifi_connected_route_flag);
            #endif
        }
        else
        {
            // 开机刚刚启动的时候,一旦running之后马上连接的时候还不行,此时会
            // 会不稳定,导致会断一次线
            static int no_link_cnt = 0;
            if(wifi_mode == 0)
            {
                no_link_cnt++;
                if(no_link_cnt >= 2)
                {
                    no_link_cnt  = 0;
                    if(wifi_net_state != M_WIFI_NET_STATE_NOLINK_ROUTE)
                    {
                        wifi_net_state = M_WIFI_NET_STATE_NOLINK_ROUTE;
                        tcp_client_close();
                        tcp_server_close();
                        #if (WIFI_DEBUG >= 3)
                        debug_out("wifi net unlink now ,so close exist socket fd \n");
                        #endif
                    }
                }

            }
            #if 0
            //static no_link_cnt = 0;
            // 检测到无连接计数
            //no_link_cnt++;
            if(wifi_mode == 0)
            {
                if(no_link_cnt  >= 2 )
                {
                    no_link_cnt  = 0;
                    // 路由器连接失败
                    //wifi_connected_route_flag  = 0;
                    wifi_net_state = M_WIFI_NET_STATE_NOLINK_ROUTE;
                    tcp_client_close();
                    tcp_server_close();
                }
            }
            #endif
            #if (WIFI_DEBUG >= 3)
            //debug_out("wifi net no link now and wifi_connected_route_flag = %d\n",wifi_connected_route_flag);
            //debug_out("wifi net no link now and s32_wifi_net_running_stat = %d\n",s32_wifi_net_running_stat);
            //debug_out("wifi net no link now and s32_wifi_net_inet_addr_stat= %d\n",s32_wifi_net_inet_addr_stat);
            #endif
        }
    }
}

unsigned char pu8_udp_write_buf[M_RING_BUF_SIZE];
unsigned char pu8_udp_read_buf[M_RING_BUF_SIZE];

unsigned char pu8_tcp_write_buf[M_RING_BUF_SIZE];
unsigned char pu8_tcp_write_buf_tmp[M_RING_BUF_SIZE];
unsigned char pu8_tcp_read_buf[M_RING_BUF_SIZE];
unsigned char pu8_tcp_read_buf_tmp[M_RING_BUF_SIZE];
int tcp_write(unsigned char *buf,int len)
{
    int ret;
    // vsocketmode   0:Server/1:Client.
    if(s2wifi_cfg.vsocketmode == 1)
    {
        ret = tcp_client_write(buf,len);
    }
    else if(s2wifi_cfg.vsocketmode == 0)
    {
        ret = tcp_server_write(buf,len);
    }
    return ret;
}


int tcp_read (unsigned char *buf,int len)
{
    int ret;
    // vsocketmode   0:Server/1:Client.
    if(s2wifi_cfg.vsocketmode == 1)
    {
        ret = tcp_client_read_data(buf,len);
    }
    else if(s2wifi_cfg.vsocketmode == 0)
    {
        ret = tcp_server_read_data(buf,len);
    }
    return ret;
}


int fn_led_task(void)
{
 static unsigned long led_mode_cnt = 0;
 static unsigned long led_run_cnt = 0;
 static unsigned long led_link_cnt = 0;
 led_mode_cnt  += M_WIFI_PROCESS_USLEEP_CNT_MS;
 led_run_cnt  += M_WIFI_PROCESS_USLEEP_CNT_MS;
 led_link_cnt  += M_WIFI_PROCESS_USLEEP_CNT_MS;

 // 模式指示灯   开机时指示一下
 if(led_mode_cnt == 1000)
 {
  //led_mode_cnt  = 0;
  // wifi作为STA模式  STA模式下灭
  if(wifi_mode == 0)
  {
   fn_led_set(LED_MODE,0);
  }
   // wifi作为AP模式  AP 模式下亮
  else if(wifi_mode == 1)
  {
   fn_led_set(LED_MODE,1);
  }
 }

 // 运行指示灯  2s周期闪烁
 if(led_run_cnt <= 500)
 {
  fn_led_set(LED_RUN,0);
 }
 else if(led_run_cnt <= 1000)
 {
  fn_led_set(LED_RUN,1);
 }
 else
 {
  led_run_cnt  = 0;
 }

 // 链接指示灯
 if(led_link_cnt >= 1000)
 {
  led_link_cnt = 0;
  if(wifi_net_state >=  M_WIFI_NET_STATE_CONNECTED_ROUTE)
  {
   // 链接上路由器 设定为亮
   fn_led_set(LED_LINK,1);
  }
  else
  {
   // 没有链接上设定为灭
   fn_led_set(LED_LINK,0);
  }
 }


 static int uart_recv_led_cnt = 0;
 if(uart_recv_led_cnt  <= 0)
 {
  if(am1808_uart_recv_data_flag == 1)
  {
   am1808_uart_recv_data_flag = 0;
   uart_recv_led_cnt  = M_LED_UART_ON_CNT + M_LED_UART_OFF_CNT ;
  }
 }
 else if(uart_recv_led_cnt <= M_LED_UART_ON_CNT)
 {
  // led关闭
  fn_led_set(LED_RXD,0);
  uart_recv_led_cnt--;
 }
 else if(uart_recv_led_cnt <= M_LED_UART_ON_CNT + M_LED_UART_OFF_CNT )
 {
  // led开启
  fn_led_set(LED_RXD,1);
  uart_recv_led_cnt--;
 }
 else
 {
  // led关闭
  fn_led_set(LED_RXD,0);
  uart_recv_led_cnt = 0;
 }

 static int uart_send_led_cnt = 0;
 if(uart_send_led_cnt  <= 0)
 {
  if(am1808_uart_send_data_flag == 1)
  {
   am1808_uart_send_data_flag = 0;
   uart_send_led_cnt  = M_LED_UART_ON_CNT + M_LED_UART_OFF_CNT ;
  }
 }
 else if(uart_send_led_cnt <= M_LED_UART_ON_CNT)
 {
  // led关闭
  fn_led_set(LED_TXD,0);
  uart_send_led_cnt--;
 }
 else if(uart_send_led_cnt <= M_LED_UART_ON_CNT + M_LED_UART_OFF_CNT )
 {
  // led开启
  fn_led_set(LED_TXD,1);
  uart_send_led_cnt--;
 }
 else
 {
  // led关闭
  fn_led_set(LED_TXD,0);
  uart_send_led_cnt = 0;
 }

}
//获取地址
//返回IP地址字符串
//返回：0=成功，-1=失败
int getlocalip(char* outip)
{
	int i=0;
	int sockfd;
	struct ifconf ifconf;
	char buf[512];
	struct ifreq *ifreq;
	char* ip;
	
	//初始化ifconf
	ifconf.ifc_len = 512;
	ifconf.ifc_buf = buf;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		return -1;
	}
	ioctl(sockfd, SIOCGIFCONF, &ifconf);    //获取所有接口信息
	close(sockfd);

	//接下来一个一个的获取IP地址
	ifreq = (struct ifreq*)buf;
	for(i=(ifconf.ifc_len/sizeof(struct ifreq));i>0;i--)
	{
		ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
		if(strcmp(ifreq->ifr_name,"ra0")!=0)
		{
			printf("Eth name is %s, IP is %s \n",ifreq->ifr_name,ip);			
			ifreq++;
			continue;
		}
		if(strcmp(ip,"127.0.0.1")==0)  //排除127.0.0.1，继续下一个
		{
			ifreq++;
			continue;
		}
		strcpy(outip,ip);
		printf("Local Eth name is %s, IP is ",ifreq->ifr_name,ip);	
		return 0;
	}
	return -1;
}


// For electrolux
void fn_get_local_ip_addr(void)
{
	char ip[20];
	int ipaddr[4];
	if(getlocalip(ip) == 0)
	{
		printf("local IP is %s\n",ip);
		sscanf(ip,"%d.%d.%d.%d",&ipaddr[0],&ipaddr[1],&ipaddr[2],&ipaddr[3]);
		printf("IP is %d.%d.%d.%d\n",ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3]);
	}
	else
	{
		printf("can not get local IP address\n");
	}
}

#define   ETH_NAME "ra0" 

int  fn_get_local_mac_addr(void) 
{ 
	int   sock; 
	struct   sockaddr_in   sin; 
	struct   sockaddr   sa; 
	struct   ifreq   ifr; 
	unsigned   char   mac[6]; 

	sock = socket(AF_INET,   SOCK_DGRAM,   0); 
	if   (sock   ==   -1) 
	{ 
		printf( "socket error "); 
		return  -1; 
	} 

	strncpy(ifr.ifr_name,ETH_NAME,IFNAMSIZ); 
	ifr.ifr_name[IFNAMSIZ - 1] = 0; 

	memset(mac,0,sizeof(mac)); 
	if(ioctl(sock,SIOCGIFHWADDR,&ifr) < 0) 
	{ 
		perror( "ioctl "); 
		return -1; 
	} 

	memcpy(&sa,&ifr.ifr_addr,sizeof(sin)); 
	memcpy(mac,sa.sa_data,sizeof(mac)); 
	g_pu8_local_mac_addr[0] = mac[0];
	g_pu8_local_mac_addr[1] = mac[1];
	g_pu8_local_mac_addr[2] = mac[2];
	g_pu8_local_mac_addr[3] = mac[3];
	g_pu8_local_mac_addr[4] = mac[4];
	g_pu8_local_mac_addr[5] = mac[5];
	printf("%s mac:%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n ",ETH_NAME,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]); 
	return   0; 
} 

// For electrolux
void fn_init_mac_addr(void)
{
	g_pu8_local_mac_addr[0] = 0xaa;
    	g_pu8_local_mac_addr[1] = 0xbb;
    	g_pu8_local_mac_addr[2] = 0xcc;
    	g_pu8_local_mac_addr[3] = 0xdd;
    	g_pu8_local_mac_addr[4] = 0xee;
    	g_pu8_local_mac_addr[5] = 0xff;
}
// For electrolux
unsigned char g_u8_connect_server_first_send_string = 1;
unsigned long g_u32_connect_server_first_time_out_cnt = 0;
void fn_connect_server_first_send_string(void)
{
	unsigned char u8_string[200];
	unsigned int u16_string_len = 0;
	if(g_u8_connect_server_first_send_string == 1)
	{
	     g_u8_connect_server_first_send_string = 0;
	     u16_string_len = snprintf(u8_string,sizeof(u8_string),
			"GET /i.php?mac=%02x%02x%02x%02x%02x%02x HTTP/1.1\r\nHost: elks.zeroplace.cn\r\n\r\n",
			g_pu8_local_mac_addr[0],g_pu8_local_mac_addr[1],g_pu8_local_mac_addr[2],
			g_pu8_local_mac_addr[3],g_pu8_local_mac_addr[4],g_pu8_local_mac_addr[5]);
	     tcp_client_write(u8_string,u16_string_len);
	     #if (WIFI_DEBUG >= 3)
	     debug_out("fn_connect_server_first_send_string:\n%s\nstring len=%d\n\n",u8_string,u16_string_len);
	     #endif
	}
    
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
int wifi_process (void)
{
    // ringbuf缓冲区剩余大小
    int ringbuf_free;
    int write_num;
    int wifi_num;
    int wifi_data_num;
    // tcp相关数据读写计数
    int tcp_read_num;
    int tcp_write_num;

    // udp相关数据读写计数
    int udp_read_num;
    int udp_write_num;

    int i,j;
    int mac_addr_no_match_flag = 0;

    // 初始化管道
    fn_wifi_pipe_init();
    // 初始化内存共享区域
    fn_wifi_shm_init();

    // For electrolux 初始化MAC地址  
    fn_init_mac_addr();

    // 从s2wifi.cfg获取配置结构体
    fn_get_config(s2wifi_config_wifi);
    //fn_printf_config_information(*s2wifi_config_wifi);

    // 读取wifi模式
    wifi_mode = fn_wifi_mode_read();
    if(wifi_mode == -1)
    {
        debug_out("wifi_mode error\n");
        wifi_mode = 1;
    }
    s2wifi_config_wifi->wifi_mode = wifi_mode;

    // 产生wpa配置文件,这个函数必须先获取配置信息到结构体s2wifi_cfg
    fn_create_wpa_cfg();

    // 初始化ringbuf缓冲区
    fn_wifi_uart_data_ringbuf_init();

    while(1)
    {
        usleep(M_WIFI_PROCESS_USLEEP_CNT_MS*1000);

        // wifi 模式改变或者 wifi初始化的时候
        if((wifi_mode_change_flag == 1) || (wifi_mode_init_flag == 1))
        {
            // wifi模式改变
            if((wifi_mode_change_flag == 1)  && (wifi_mode_init_flag == 0))
            {
                // wifi作为AP模式
                if(wifi_mode == 1)
                {
                    wifi_mode_change_flag = 0;
                    fn_wifi_mode_write(wifi_mode);
                    system("sync");
                    system("reboot");
                    while(1);
                }
                // wifi 作为客户端
                else if(wifi_mode == 0)
                {
                    // 此时继续执行下面的东东
                    wifi_mode_change_flag = 0;
                    fn_wifi_mode_write(wifi_mode);
                    system("sync");
                    system("reboot");
                    while(1);
                    #if 0
                    // 重新获取配置信息
                    // 从s2wifi.cfg获取配置结构体
                    fn_get_config(s2wifi_config_wifi);
                    //fn_printf_config_information(*s2wifi_config_wifi);

                    // 读取wifi模式
                    wifi_mode = fn_wifi_mode_read();
                    if(wifi_mode == -1)
                    {
                        debug_out("wifi_mode error\n");
                        wifi_mode = 1;
                    }
                    s2wifi_config_wifi->wifi_mode = wifi_mode;

                    // 产生wpa配置文件,这个函数必须先获取配置信息到结构体s2wifi_cfg
                    fn_create_wpa_cfg();
                    #endif

                }
            }
            // wifi 初始配置 仅在刚开始的时候执行一次
            else if(wifi_mode_init_flag == 1)
            {
                wifi_mode_init_flag = 0;
            }

            // 创建一个子进程用于连接无线网络
            // wifi_connect_route_pid中存储该进程ID号
            if((wifi_connect_route_pid = fork()) < 0)
            {
                debug_out("cmd process pid\n");
            }
            else if(wifi_connect_route_pid == 0)   // child process
            {
                //fn_wifi_deal_connect_route();
                //while(1)
                //{
                //  sleep(1);
                //  debug_out("test in wifi connect child \n");
                //}
                // wifi负责连接处理的函数,改函数内是while(1)循环
                fn_wifi_deal_connect_route_and_ap_mode();
                // 如果进程被杀掉,不会执行这句话的
                debug_out("test in wifi connect child be killed now \n");
            }
            else   //father process
            {
                // 父进程直接退出
            }
        }

        // wifi 程序中负责uart数据处理  uart->wifi  wifi->uart
        fn_wifi_deal_uart_data_to_net();

        // 扫描net和连接到路由器上  判断是否已经链接到路由器上
        fn_wifi_scan_net_connect_to_route();

 	// led 显示任务
 	fn_led_task();

        switch (wifi_net_state)
        {
            // 刚刚连接到路由器上
            case M_WIFI_NET_STATE_CONNECTED_ROUTE:
	    // 获取本机IP地址
	    fn_get_local_ip_addr();
	    // 获取本机MAC地址
	    fn_get_local_mac_addr();
            // wifi_mode = 0的时候为客户端  wifi_mode=1的时候为AP模式
            if(wifi_mode == 0)
            {
                #if (WIFI_DEBUG >= 3)
                debug_out("wifi_net_state = %d\n",wifi_net_state);
                #endif
                // vsocketmode   0:Server/1:Client.
                // vsocketprol  Socket Protocol. 0:TCP/1:UDP.
                // 为tcp协议 服务器
                if((s2wifi_cfg.vsocketmode == 0) && (s2wifi_cfg.vsocketprol == 0))
                {
                    tcp_server_init();
                    wifi_net_state = M_WIFI_NET_STATE_SERVER_ACCEPT;
                }
                // 为tcp协议 客户端
                else if((s2wifi_cfg.vsocketmode == 1) && (s2wifi_cfg.vsocketprol == 0))
                {
                    if(tcp_client_init() == -1)
		    {
			 wifi_net_state = M_WIFI_NET_STATE_NOLINK_ROUTE;
		    }
		    else
	            {
                         wifi_net_state = M_WIFI_NET_STATE_CLIENT_CONECTING ;
                    }
                }
                // 为udp协议
                else if(s2wifi_cfg.vsocketprol == 1)
                {
                    udp_init();
                    wifi_net_state = M_WIFI_NET_STATE_UDP ;
                }
            }
            break;
            case M_WIFI_NET_STATE_SERVER_ACCEPT:
            if((s2wifi_cfg.vsocketmode == 0) && (s2wifi_cfg.vsocketprol == 0))
            {
                // 表示连接上了
                if(tcp_server_accept() == 0)
                {
                    wifi_net_state = M_WIFI_NET_STATE_SERVER_CONECTED;
                    #if (WIFI_DEBUG >= 3)
                    debug_out("wifi_net_state = %d\n",wifi_net_state);
                    #endif
                }
            }
            else
            break;
            case M_WIFI_NET_STATE_CLIENT_CONECTING:
            if((s2wifi_cfg.vsocketmode == 1) && (s2wifi_cfg.vsocketprol == 0))
            {
                // 表示连接上了
                int connec_result;
                if((connec_result = tcp_client_connecting()) == 0)
                {
                    wifi_net_state = M_WIFI_NET_STATE_CLIENT_CONECTED;
                    #if (WIFI_DEBUG >= 3)
                    debug_out("wifi_net_state = %d\n",wifi_net_state);
		    debug_out("|r is 0x%02x \n",(unsigned char)('\r'));
		    debug_out("|n is 0x%02x \n",(unsigned char)('\n'));
                    #endif
                }
                else if(connec_result == -2)
                {
	       		static int connect_err_cnt = 0;
	      		// 下面几句话加进去之后   导致客户端链接不上了
	      		// connect_err_cnt += M_WIFI_PROCESS_USLEEP_CNT_MS;
	       		//if(connect_err_cnt >= 600)
	       		{
	       			// 接受到关闭信号，此时关闭tcp 客户端
              			tcp_client_close();

              			wifi_net_state = M_WIFI_NET_STATE_CONNECTED_ROUTE;
              			#if (TCP_CLIENT_DEBUG >= 1)
              			debug_out("connect occur error,tcp client init again\n");
              			#endif
       			}
                }
            }
            break;
            // 处于客户端已经连接的状态下
            case M_WIFI_NET_STATE_CLIENT_CONECTED:
	    
            tcp_read_num = tcp_client_read_data(pu8_tcp_read_buf,sizeof(pu8_tcp_read_buf));
            if(tcp_read_num  == 0)
            {
                // 如果是处于客户端的时候，接受到的关闭连接的信号的时候
                if(wifi_net_state == M_WIFI_NET_STATE_CLIENT_CONECTED)
                {
                    // 接受到关闭信号，此时关闭tcp 客户端
                    tcp_client_close();
                    // wifi返回到客户端连接状态
                    // 如果这里返回M_WIFI_NET_STATE_CLIENT_CONECTING，此时发现
                    // 如果服务器端断开连接，此时不能再正确连接，如果重新初始化
                    // tcp_client_init，此时能够立即连接上服务器
                    wifi_net_state = M_WIFI_NET_STATE_CONNECTED_ROUTE;
                    #if (TCP_CLIENT_DEBUG >= 1)
                    debug_out("receice close connection signal,turn to init connecting\n");
                    #endif
                }
            }
            else if(tcp_read_num > 0)
            {
                ringbuf_free = RingBufFree(&st_wifi_to_uart_data);
                if(ringbuf_free  < tcp_read_num)
                {
                    wifi_num  = ringbuf_free;
                }
                else
                {
                    wifi_num= tcp_read_num;
                }
                #if (WIFI_DEBUG >= 3)
                debug_out("[wifi->uart] in wifi_process: ringbuf_free = %d,wifi_num=%d,tcp_read_num=%d\n",ringbuf_free,wifi_num,tcp_read_num);
		debug_out("receive data is %s\n",pu8_tcp_read_buf);
                #endif
                // For electrolux
                //RingBufWrite(&st_wifi_to_uart_data,pu8_tcp_read_buf,wifi_num);
		//if(sscanf(pu8_tcp_read_buf,"true%s",pu8_tcp_read_buf_tmp) != -1)
		if(strstr(pu8_tcp_read_buf,"true") != NULL)		
		{
			#if (WIFI_DEBUG >= 3)
                	debug_out("receive string true\n");
                	#endif
			// 接受到关闭信号，此时关闭tcp 客户端
                        tcp_client_close();
			wifi_net_state = M_WIFI_NET_STATE_CONNECTED_ROUTE;
			
			// for electrolux
			// vsocketmode   0:Server/1:Client.
                	// vsocketprol  Socket Protocol. 0:TCP/1:UDP.
                	// 为tcp协议 服务器
			s2wifi_cfg.vsocketmode = 0;  // server
			s2wifi_cfg.vsocketprol = 0;  // tcp
			s2wifi_cfg.vsocketlp = 5000; // 本地端口
		}
		else
		{
			#if (WIFI_DEBUG >= 3)
                	debug_out("No receive string true\n");
                	#endif
		}
            }

            wifi_data_num = RingBufUsed(&st_uart_to_wifi_data);
            if(wifi_data_num > 0)
            {
                RingBufRead(&st_uart_to_wifi_data,pu8_tcp_write_buf,wifi_data_num);
                // For electrolux
		// 丢掉数据 
		//tcp_client_write(pu8_tcp_write_buf,wifi_data_num);
		wifi_data_num = 0;
            }
		
            fn_connect_server_first_send_string();

	    g_u32_connect_server_first_time_out_cnt += M_WIFI_PROCESS_USLEEP_CNT_MS;
	    if((g_u32_connect_server_first_time_out_cnt >= 3000) && (wifi_net_state == M_WIFI_NET_STATE_CLIENT_CONECTED))
	    {
		g_u32_connect_server_first_time_out_cnt = 0;
		g_u8_connect_server_first_send_string = 1;
	    }
	    
            break;
            // 处于服务器已经处于被连接状态下
            case M_WIFI_NET_STATE_SERVER_CONECTED:
            tcp_read_num = tcp_server_read_data(pu8_tcp_read_buf_tmp,sizeof(pu8_tcp_read_buf_tmp));
            if(tcp_read_num  == 0)
            {
                // 注意：刚才在服务器端的时候没有问题，而如果换成客户端的时候
                // 发现串口接受到数据，但是没有接受数据，原来是这边无论客户端
                // 和服务器都是切换到服务器等嗲状态，因此有问题
                // 如果处于tcp server状态的时候，此时接受到的关闭的信号的时候，
                // 就返回连接状态
                if(wifi_net_state == M_WIFI_NET_STATE_SERVER_CONECTED)
                {
                    // 接受到关闭信号
                    tcp_server_close_connectting();
                    // wifi返回到服务器监听状态了
                    wifi_net_state = M_WIFI_NET_STATE_SERVER_ACCEPT;
                }
            }
            else if(tcp_read_num > 0)
            {
                ringbuf_free = RingBufFree(&st_wifi_to_uart_data);
                if(ringbuf_free  < tcp_read_num)
                {
                    wifi_num  = ringbuf_free;
                }
                else
                {
                    wifi_num= tcp_read_num;
                }
                #if (WIFI_DEBUG >= 3)
                debug_out("[wifi->uart] in wifi_process: ringbuf_free = %d,wifi_num=%d,tcp_read_num=%d\n",ringbuf_free,wifi_num,tcp_read_num);
                #endif
		
		if(tcp_read_num > 6)
		{
			#if (WIFI_DEBUG >= 3)			
			debug_out("our MAC is%02x-%02x-%02x-%02x-%02x-%02x\n",
			g_pu8_local_mac_addr[0],g_pu8_local_mac_addr[1],g_pu8_local_mac_addr[2],
			g_pu8_local_mac_addr[3],g_pu8_local_mac_addr[4],g_pu8_local_mac_addr[5]);			
			debug_out("recv MAC is\n");
			#endif
			mac_addr_no_match_flag = 0;				
			for(i = 0; i < 6; i++)
			{
				#if (WIFI_DEBUG >= 3)
                		debug_out("%02x ",pu8_tcp_read_buf_tmp[i]);
               		 	#endif
				if(g_pu8_local_mac_addr[i] != pu8_tcp_read_buf_tmp[i])
				{
					mac_addr_no_match_flag = 1;
				}
			}
			debug_out("\n\n");
			for(i = 6; i < wifi_num; i++)
			{
				pu8_tcp_read_buf[i-6] = pu8_tcp_read_buf_tmp[i];
			}
			if(mac_addr_no_match_flag == 0)	
			{
				RingBufWrite(&st_wifi_to_uart_data,&pu8_tcp_read_buf,wifi_num-6);
				#if (WIFI_DEBUG >= 3)
                		debug_out("Mac is match,jump to server mode\n");
               		 	#endif
			}	
			else
			{
				#if (WIFI_DEBUG >= 3)
                		debug_out("Mac is no match,discard this data,wait string...\n");
               		 	#endif
			}	
			
		} 
                
            }

            wifi_data_num = RingBufUsed(&st_uart_to_wifi_data);
            if(wifi_data_num > 0)
            {
                RingBufRead(&st_uart_to_wifi_data,pu8_tcp_write_buf_tmp,wifi_data_num);
		for(i = 0; i < 6; i++)
		{
			pu8_tcp_write_buf[i] = g_pu8_local_mac_addr[i];
		}
		for(i = 6; i < 6 + wifi_data_num; i++)
		{
			pu8_tcp_write_buf[i] = pu8_tcp_write_buf_tmp[i-6];
		}
                tcp_server_write(pu8_tcp_write_buf,wifi_data_num + 6);
            }

            break;
            // udp正常通信情况下循环查询
            case M_WIFI_NET_STATE_UDP:
            // 从udp读取数据 读取到缓冲区pu8_udp_read_buf中
            udp_read_num = udp_read_data(pu8_udp_read_buf,sizeof(pu8_udp_read_buf));
            // 如果读取的个数大于0,则表示udp接受到数据了
            if(udp_read_num > 0)
            {
                // 查看ringbuf剩余空闲空间
                ringbuf_free = RingBufFree(&st_wifi_to_uart_data);
                // 如果ringbuf空间过小，则限定为ringbuf剩余空间大小
                if(ringbuf_free  < udp_read_num)
                {
                    wifi_num  = ringbuf_free;
                }
                else
                {
                    wifi_num= udp_read_num;
                }
                #if (WIFI_DEBUG >= 3)
                debug_out("[wifi->uart] in wifi_process: ringbuf_free = %d,wifi_num=%d,udp_read_num=%d\n",ringbuf_free,wifi_num,udp_read_num);
                #endif
                // 向ringbuf中写数据
                RingBufWrite(&st_wifi_to_uart_data,pu8_udp_read_buf,wifi_num);
            }

            wifi_data_num = RingBufUsed(&st_uart_to_wifi_data);
            if(wifi_data_num > 0)
            {
                RingBufRead(&st_uart_to_wifi_data,pu8_udp_write_buf,wifi_data_num);
                udp_write(pu8_udp_write_buf,wifi_data_num);
            }
            break;

            default:
            break;
        }


    }
    return 0;
}
