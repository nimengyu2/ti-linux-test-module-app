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

// tcp server的监听fd
int tcp_server_socket_listen_fd;
int tcp_server_socket_fd;
// tcp server的地址
struct sockaddr_in tcp_server_addr;
// tcp server 的读集合
fd_set tcp_server_rset;
// tcp server 的写集合
fd_set tcp_server_wset;
// tcp server的时间间隔
struct timeval tcp_server_tval;
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
int tcp_server_init (void)
{
    tcp_server_tval.tv_sec = 0;
    tcp_server_tval.tv_usec = 0;

    // 创建socket描述符
    if((tcp_server_socket_listen_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("socket create error\n");
        }
        #endif
        return -1;
    }
    else
    {
        #if (TCP_SERVER_DEBUG >= 3)
        {
            debug_out("socket create success,tcp_server_socket_listen_fd = %d\n",
                    tcp_server_socket_listen_fd);
        }
        #endif
    }

    bzero(&tcp_server_addr,sizeof(tcp_server_addr));
    tcp_server_addr.sin_family = AF_INET;
    tcp_server_addr.sin_port = htons(s2wifi_cfg.vsocketlp);
    tcp_server_addr.sin_addr.s_addr = INADDR_ANY;

    int tr=1;

    // kill "Address already in use" error message
    if (setsockopt(tcp_server_socket_listen_fd,SOL_SOCKET,
                    SO_REUSEADDR,&tr,sizeof(int)) == -1)
    {
        #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("set listen opt failed\n");
        }
        #endif
        return -1;
    }

    if(bind(tcp_server_socket_listen_fd, (struct sockaddr *)&tcp_server_addr, sizeof(tcp_server_addr)) == -1)
    {
        #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("socket bind error\n");
        }
        #endif
        return -1;
    }
    else
    {
        #if (TCP_SERVER_DEBUG >= 3)
        {
            debug_out("socket bind ok\n");
            debug_out("socket port is %d\n",s2wifi_cfg.vsocketlp);
            debug_out("local IP is \n");
            system("ifconfig ra0 | grep inet ");
        }
        #endif
    }

// 实际测试发现,不需要设定noblock就会直接跳过listen函数
#if 0
    // 设置为非阻塞,首先获取flag,设定noblock,然后设定flag
    flags = fcntl(slisten,F_GETFL,0);
    fcntl(slisten,F_SETFL,flags|O_NONBLOCK);
#endif

    if (listen(tcp_server_socket_listen_fd, 5) == -1)
    {
             #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("socket listen error\n");
        }
        #endif
        return -1;
     }
    else
    {
        #if (TCP_SERVER_DEBUG >= 3)
        {
            debug_out("socket listen ok,waitting for client connect to me\n");
        }
        #endif
    }

    tcp_server_socket_fd = 0;

    return 0;
}


//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  如果接受到client连接，则返回0,如果没有接受到连接，则返回-1
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int tcp_server_accept (void)
{
    int ret;
    socklen_t len;
    int res;
    if(tcp_server_socket_listen_fd < 0)
    {
        #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("tcp_server_socket_listen_fd below zero\n");
        }
        #endif
        return -1;
    }

    FD_ZERO(&tcp_server_rset);
    FD_SET(tcp_server_socket_listen_fd,&tcp_server_rset);
    tcp_server_wset = tcp_server_rset;
    if((res = select(tcp_server_socket_listen_fd+1,
                &tcp_server_rset,NULL, NULL,&tcp_server_tval) ) <= 0)
    {
        #if (TCP_SERVER_DEBUG >= 3)
        {
            //debug_out("no client connect\n");
        }
        #endif
        return -1;
        //close(sockfd);
    }
    else if(res > 0)
    {
        #if 0
        len = sizeof(error);
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
        if (error)
        {
            fprintf(stderr, "Error in connection() %d - %s\n", error, strerror(error));
            //return 1;
        }
        else
        {
            tcp_client_connect_ok_flag = 1;
            printf("connect ok\n");
        }
        #endif
        if(FD_ISSET(tcp_server_socket_listen_fd,&tcp_server_rset)) // new connection
        {
            len = sizeof(struct sockaddr);
            tcp_server_socket_fd = accept(tcp_server_socket_listen_fd,
                            (struct sockaddr*)&tcp_server_addr,&len);
            if(tcp_server_socket_fd == -1)
            {
                #if (TCP_SERVER_DEBUG >= 1)
                {
                    debug_out("select for accept error\n");
                }
                #endif
                return -1;
            }
            else
            {
                #if (TCP_SERVER_DEBUG >= 2)
                {
                    debug_out("accept connection ok\n");
                    debug_out("Your got a connection from %s.\n",inet_ntoa(tcp_server_addr.sin_addr));
                }
                #endif
                // 返回表示接受到连接成功
                return 0;
            }
        }
    }
    return -1;
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
int tcp_server_read_data (unsigned char *buf,int len)
{
    int n;
    int recvbytes;
    if(tcp_server_socket_fd < 0)
    {
        #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("tcp_server_socket_fd below zero\n");
        }
        #endif
        // 读取错误
        return -2;
    }
    FD_ZERO(&tcp_server_rset);
    FD_SET(tcp_server_socket_fd,&tcp_server_rset);
    tcp_server_wset = tcp_server_rset;
    n = select(tcp_server_socket_fd+1,&tcp_server_rset,NULL, NULL,&tcp_server_tval);
    if( n <= 0 )//rset没有使用过，不用重新置为sockfd
    {
        //perror("receive time out or connect error");
        //close(sockfd);
        //return -1;
        #if (TCP_SERVER_DEBUG >= 3)
        {
            //debug_out("tcp_server no data to read\n");
        }
        #endif
        // 读取错误
        return -1;
    }
    else if(n > 0)
    {
        if ((recvbytes=recv(tcp_server_socket_fd, buf, len, 0)) ==-1)
        {
            #if (TCP_SERVER_DEBUG >= 1)
            {
                debug_out("tcp_server read data error\n");
            }
            #endif
            close(tcp_server_socket_fd);
            // 读取错误
            return -1;
        }
        else if(recvbytes == 0)
        {
            // 接受到关闭连接信号
            return 0;
        }
        else if(recvbytes > 0)
        {
            // 接受到tcp连接数据
            #if 1
            buf[recvbytes] = 0x00;
            //printf("receive num %d\n",recvbytes);
            //printf("%s\n",buf);
            #endif
            return recvbytes;
        }
    }
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
int tcp_server_write (unsigned char* buf,int len)
{
    int n;
    if(tcp_server_socket_fd < 0)
    {
        #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("tcp_server_socket_fd below zero\n");
        }
        #endif
        // 读取错误
        return -2;
    }
    if ( (n = send(tcp_server_socket_fd, buf, len,0) ) ==-1 )
    {
        #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("tcp_server write data error\n");
        }
        #endif
        close(tcp_server_socket_fd);
        // 读取错误
        return -1;
    }
    return n;
}


int tcp_server_close (void)
{
    if(tcp_server_socket_listen_fd> 0)
    {
        //shutdown(tcp_server_socket_listen_fd,SHUT_RDWR);
        close(tcp_server_socket_listen_fd);
        #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("tcp_server close socket tcp_server_socket_listen_fd\n");
        }
        #endif
    }
    if(tcp_server_socket_fd> 0)
    {
        ///shutdown(tcp_server_socket_fd,SHUT_RDWR);
        close(tcp_server_socket_fd);
        #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("tcp_server close socket tcp_server_socket_fd\n");
        }
        #endif
    }
}

int tcp_server_close_connectting (void)
{
    if(tcp_server_socket_fd> 0)
    {
        ///shutdown(tcp_server_socket_fd,SHUT_RDWR);
        close(tcp_server_socket_fd);
        #if (TCP_SERVER_DEBUG >= 1)
        {
            debug_out("tcp_server close socket tcp_server_socket_fd\n");
        }
        #endif
    }
}



