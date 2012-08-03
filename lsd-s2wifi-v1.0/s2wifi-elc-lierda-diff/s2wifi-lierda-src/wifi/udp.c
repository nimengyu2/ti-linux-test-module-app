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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include "wifi_process.h"
#include "config.h"
#include "debug.h"
#include "shm.h"
#include "create_wpa_cfg.h"
#include "wifi_mode.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "udp.h"
#include <netdb.h>

struct sockaddr_in udp_addr_local;
struct sockaddr_in udp_addr_remote;
int udp_socket;


//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  初始化socket，错误返回-1,成功返回0
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int udp_init (void)
{
    int ret;
    int port=9000;

    // 初始化socket
    bzero(&udp_addr_local,sizeof(udp_addr_local));
    udp_addr_local.sin_family = AF_INET;
    udp_addr_local.sin_addr.s_addr = htonl(INADDR_ANY);
    udp_addr_local.sin_port = htons(s2wifi_cfg.vsocketlp);

    bzero(&udp_addr_remote,sizeof(udp_addr_remote));
    udp_addr_remote.sin_family = AF_INET;
    udp_addr_remote.sin_addr.s_addr = inet_addr(s2wifi_cfg.vsocketrip);
    udp_addr_remote.sin_port = htons(s2wifi_cfg.vsocketrp);

    udp_socket = socket(AF_INET,SOCK_DGRAM,0);
    if(udp_socket == -1)
    {
        debug_out("socket call failed");
        return -1;
    }
    else
    {
        #if (UDP_DEBUG >= 3)
        debug_out("socket create ok,udp_socket is %d \n",udp_socket);
        #endif
    }

    ret = bind(udp_socket,(struct sockaddr *)&udp_addr_local,
                    sizeof(udp_addr_local));
    if(ret == -1)
    {
        debug_out("bind call failed");
        return -1;
    }
    else
    {
        #if (UDP_DEBUG >= 3)
        debug_out("bind ok\n");
        #endif
    }

    long save_file_flags;
    save_file_flags = fcntl(udp_socket,F_GETFL);
    //printf("file flags are %ld \n",save_file_flags);
    save_file_flags |= O_NONBLOCK;
    if(fcntl(udp_socket,F_SETFL,save_file_flags) == -1)
    {
        debug_out("trying to set input socket to non-blocking");
        return -1;
    }
    else
    {
        #if (UDP_DEBUG >= 3)
        debug_out("set udp socket nonblock ok\n");
        #endif
    }

    #if (UDP_DEBUG >= 3)
    debug_out("set udp socket ok,local port is %d,remote port is %d,remote ip is %s\n",s2wifi_cfg.vsocketlp,s2wifi_cfg.vsocketrp,s2wifi_cfg.vsocketrip);
    #endif
    return 0;
}


//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  从socket读取数据，如果读取失败,返回-2,没有数据，则返回-1
//             读取0个字节，返回0, 读取多个字节，返回读取的字节个数
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int udp_read_data (unsigned char *buf,int len)
{
    int ret;
    ssize_t recv_rc;
    int sin_len = sizeof(udp_addr_local);
    recv_rc = recvfrom(udp_socket,(char *)buf,len,0,
                        (struct sockaddr*)&udp_addr_local,&sin_len);
    if((recv_rc == -1) && (errno != EAGAIN))
    {
        #if (UDP_DEBUG >= 3)
        debug_out("recv from failed \n");
        #endif
        return -1;
    }
    // 注意，之前发现udp一直接受不到数据，是因为这里的errno始终没有被
    // 清0,始终都是EAGAIN，这个和之前在一个函数中的用法是有区别的
    // 可能是因为这个errno会被恢复的缘故，具体还不清楚，要仔细研究一下
    else if((recv_rc == 0) /*| (errno == EAGAIN)*/)
    {
        errno = 0;
        #if (UDP_DEBUG >= 3)
        //debug_out("recv no data \n");
        #endif
        return 0;
    }
    // have receive data
    else if(recv_rc > 0)
    {
        errno = 0;
        ret = recv_rc;
        #if (UDP_DEBUG >= 3)
        buf[recv_rc] = 0;
        //printf("udp:response from server:%s \n",buf);
        #endif
        return ret;
    }
    // 如果这里返回-1,则会默认返回值为1
    // 这个问题很严重,终端一直出错
    // 纠结了一个早上,以后一定要注意带返回函数一定要加默认返回值
    return -1;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  如果写失败，返回-1。如果写成功则返回写的数据个数
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int udp_write (unsigned char* buf,int len)
{
    int ret;
    ret = sendto(udp_socket,buf,len,
                        0,(struct sockaddr *)&udp_addr_remote,
                        sizeof(udp_addr_remote));
    if(ret == -1)
    {
        //perror("sendto call failed");
        //exit(errno);
        #if (UDP_DEBUG >= 3)
        debug_out("udp:send to data failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (UDP_DEBUG >= 3)
        debug_out("udp:send data ok\n");
        #endif
        return ret;
    }
}

int udp_close (void)
{
    if(udp_socket > 0)
    {
        close(udp_socket);
        #if (UDP_DEBUG >= 1)
        debug_out("close udp socket\n");
        #endif
    }
}



