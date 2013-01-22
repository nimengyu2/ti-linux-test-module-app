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

#include "tcp_client.h"
#include "config.h"


int tcp_client_socket_fd;
// tcp client的地址
struct sockaddr_in tcp_client_addr;
// tcp client 的读集合
fd_set tcp_client_rset;
// tcp client 的写集合
fd_set tcp_client_wset;
// tcp client的时间间隔
struct timeval tcp_client_tval;


#define TCP_CLIENT_DEBUG  4

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
int tcp_client_init (void)
{
    int res;
    int flags;
    tcp_client_tval.tv_sec = 0;
    tcp_client_tval.tv_usec = 0;

    // 创建socket描述符
    if ((tcp_client_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        // 创建socket失败
        #if (TCP_CLIENT_DEBUG >= 1)
        {
            printf("socket create error\n");
        }
        #endif
        return -1;
    }
    else
    {
        // 成功创建socket
        #if (TCP_CLIENT_DEBUG >= 3)
        {
            printf("socket create success,tcp_client_socket_fd = %d\n",
                    tcp_client_socket_fd);
        }
        #endif
    }

    tcp_client_addr.sin_family=AF_INET;
    // 服务器tcp端口和IP地址设定
    tcp_client_addr.sin_port=htons(u8_socket_serverport);
    tcp_client_addr.sin_addr.s_addr = inet_addr(pu8_socket_serverip);
    // 注意，此处如果连接的IP地址没有在网络中，则会出现错误
    // Error in connection() 113 - No route to host
    // Segmentation fault
    // 如果这样是不对的，按理说，应该是如果连接不上就应该一直连接
    //tcp_client_addr.sin_port=htons(5000);
    //tcp_client_addr.sin_addr.s_addr = inet_addr("192.168.1.76");
    bzero(&(tcp_client_addr.sin_zero),8);

    // 设置为非阻塞,首先获取flag,设定noblock,然后设定flag
    flags = fcntl(tcp_client_socket_fd,F_GETFL,0);
    fcntl(tcp_client_socket_fd,F_SETFL,flags|O_NONBLOCK);


    printf("Prepare connectting to remote IP=%s,remote port=%d\n",pu8_socket_serverip,u8_socket_serverport);
    // 注意:刚才这里conncet的参数写成了 tcp_client_addr.sin_addr
    // 此时产生错误socket connect  error
    // Error in connection() -1093411644 - Unknown error 3201555652
    // Segmentation fault
    // socket connect  error
    // Error in connection() 97 - Address family not supported by protocol
    // Segmentation fault
    res = connect(tcp_client_socket_fd, (struct sockaddr *)&tcp_client_addr,
                     sizeof(struct sockaddr)) ;
    if (res < 0)
    {
        // 如果返回则表示错误,直接返回
        if(errno != EINPROGRESS)
        {
            #if (TCP_CLIENT_DEBUG >= 1)
            {
                printf("socket connect  error\n");
            }
            #endif
            return -1;
        }
    }
    else if(res == 0)
    {
        //如果server与client在同一主机上，有些环境socket设为非阻塞会返回 0
        return 0;
    }

    // 到这里表示正在连接
    #if (TCP_CLIENT_DEBUG >= 1)
    {
        printf("waitting for socket connect to server\n");
    }
    #endif
    return 0;
}


//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  连接上返回0,连接不上否则返回-1,
//            如果连接出错，比如连接超时，或者其他的什么的，返回-2
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int tcp_client_connecting (void)
{
    int res;
    socklen_t len;
    int error;
    static int info_cnt = 0;
#if 0
    // 如果没有连接上，则尝试多次连接
    res = connect(tcp_client_socket_fd, (struct sockaddr *)&tcp_client_addr,
                     sizeof(struct sockaddr)) ;
    if (res < 0)
    {
        // 如果返回则表示错误,直接返回
        if(errno != EINPROGRESS)
        {
            #if (TCP_CLIENT_DEBUG >= 1)
            {
                //printf("socket connect  error\n");
            }
            #endif
            //return -1;
        }
    }
    else if(res == 0)
    {
        //如果server与client在同一主机上，有些环境socket设为非阻塞会返回 0
        // 这里暂时注释掉，如果连接上了，也用select获取连接信息
        //return 0;
    }
#endif
    FD_ZERO(&tcp_client_rset);
    FD_SET(tcp_client_socket_fd,&tcp_client_rset);
    tcp_client_wset = tcp_client_rset;
    if((res = select(tcp_client_socket_fd+1, NULL,
            &tcp_client_wset, NULL,&tcp_client_tval)) <= 0)
    {
        #if (TCP_CLIENT_DEBUG >= 3)
        {
            info_cnt++;
            if(info_cnt >= 150)
            {
                info_cnt = 0;
                //printf("no link ,is connecting  to server\n");
            }
        }
        #endif
        return -1;
    }
    else
    {
        len = sizeof(error);
        getsockopt(tcp_client_socket_fd, SOL_SOCKET, SO_ERROR, &error, &len);
        if (error)
        {
            #if (TCP_CLIENT_DEBUG >= 3)
            {
                // 注意：这句话会引起段错误，现在还不清楚这个是什么原因引起的，具体有空再分析
                // Error in connection() 113 - No route to host
                // Segmentation fault
                //printf("Error in connection() %d - %s\n", error, strerror(error));
                //printf("Error in connection() %s",strerror(error));
            }
            #endif
            return -2;
        }
        else if(error == 0)
        {
            #if (TCP_CLIENT_DEBUG >= 3)
            {
                printf("connect ok\n");
            }
            #endif
            // 连接成功
            return 0;
        }
    }
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
int tcp_client_read_data (unsigned char *buf,int len)
{
    int n;
    int recvbytes;
    if(tcp_client_socket_fd < 0)
    {
        #if (TCP_CLIENT_DEBUG >= 1)
        {
            printf("tcp_client_socket_fd below zero\n");
        }
        #endif
        // 读取错误
        return -2;
    }
    FD_ZERO(&tcp_client_rset);
    FD_SET(tcp_client_socket_fd,&tcp_client_rset);
    tcp_client_wset = tcp_client_rset;
    n = select(tcp_client_socket_fd+1,
                &tcp_client_rset,NULL, NULL,&tcp_client_tval);
    // 这里表示没有接受到数据
    if( n <= 0 )//rset没有使用过，不用重新置为sockfd
    {
        //perror("receive time out or connect error");
        //close(sockfd);
        //return -1;
        #if (TCP_CLIENT_DEBUG >= 3)
        {
            //printf("tcp_client no data to read\n");
        }
        #endif
        // 这里表示没有数据
        return -1;
    }
    else if(n > 0)
    {
        if ((recvbytes=recv(tcp_client_socket_fd, buf, len, 0)) ==-1)
        {
            #if (TCP_CLIENT_DEBUG >= 1)
            {
                printf("tcp_server read data error\n");
            }
            #endif
            //close(tcp_client_socket_fd);
            // 读取错误
            // 如果读取错误，应该关闭连接，重新连接
            return -2;
        }
        else if(recvbytes == 0)
        {
            // 接收到关闭信号
            return 0;
        }
        else if(recvbytes > 0)
        {
            // 根据需要打印输出接收到的数据
            #if 1
            buf[recvbytes] = 0x00;
            //printf("receive num %d\n",recvbytes);
            //printf("%s\n",buf);
            #endif
            // 返回接受到的数据个数
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
int tcp_client_write (unsigned char* buf,int len)
{
    int n;
    if(tcp_client_socket_fd < 0)
    {
        #if (TCP_CLIENT_DEBUG >= 1)
        {
            printf("tcp_client_socket_fd below zero\n");
        }
        #endif
        // 读取错误
        return -2;
    }
    if ( (n = send(tcp_client_socket_fd, buf, len,0) ) ==-1 )
    {
        #if (TCP_CLIENT_DEBUG >= 1)
        {
            printf("tcp_client write data error\n");
        }
        #endif
        close(tcp_client_socket_fd);
        // 读取错误
        return -1;
    }
    return n;
}

int tcp_client_close ()
{
    if(tcp_client_socket_fd > 0)
    {
        //shutdown(tcp_client_socket_fd,SHUT_RDWR);
        close(tcp_client_socket_fd);
        #if (TCP_CLIENT_DEBUG >= 1)
        {
            printf("tcp_client close socket\n");
        }
        #endif
    }
}


