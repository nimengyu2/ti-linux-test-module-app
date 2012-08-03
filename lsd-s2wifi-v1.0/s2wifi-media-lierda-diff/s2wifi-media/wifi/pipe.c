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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include "wifi_pipe.h"
#include "uart_pipe.h"


// uart发送数据到wifi的管道文件描述符
int s2w_pipe_fd[2];

// wifi发送数据到uart的管道文件描述符
int w2s_pipe_fd[2];

#define BUFSZ PIPE_BUF

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_s2w_pipe_init
// 函数功能  :
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_pipe_create(void)
{
    if(pipe(s2w_pipe_fd) < 0)
    {
        debug_out("pipe create s2w_pipe_fd fail\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        debug_out("pipe create s2w_pipe_fd ok\n");
        // 刚才不小心这个没有注释掉，导致w2s的fd获取失败
        // 以及uart端收取不到数据
        //return 0;
    }

    if(pipe(w2s_pipe_fd) < 0)
    {
        debug_out("pipe create w2s_pipe_fd fail\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        debug_out("pipe create w2s_pipe_fd ok\n");
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_pipe_delete
// 函数功能  :
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_pipe_delete(void)
{
    close(s2w_pipe_fd[0]);
    close(s2w_pipe_fd[1]);
    close(w2s_pipe_fd[0]);
    close(w2s_pipe_fd[1]);
    debug_out("pipe delete all fd\n");
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_pipe_write
// 函数功能  :
//
// 输入参数  :
//
// 返回参数  :
//
//-----------------------------------------------------------------------------
int fn_pipe_write(int fd,char *buf,int len)
{
    int cnt;
    if((fd >= 0) && (buf != NULL))
    {
        cnt = write(fd,buf,len);
        return cnt;
    }
    else
    {
        debug_out("fn_pipe_write fd or buf error\n");
        exit(EXIT_SUCCESS);
    }
}

//-----------------------------------------------------------------------------
//
// 函数名称  :      fn_pipe_write
// 函数功能  :      向某一个管道写数据
//
// 输入参数  :      int fd 要写的管道的描述符
//              char *buf 缓冲区
//              int size 读取的大小
//
// 返回参数  :
//
//-----------------------------------------------------------------------------
int fn_pipe_read_noblock(int fd,char *buf,int size)
{
    int cnt;
    if(size > BUFSZ)
    {
        size = BUFSZ;
    }
    if((fd >= 0) && (buf != NULL) )
    {
        cnt = read(fd,buf,size);
        return cnt;
    }
    else
    {
        debug_out("fn_pipe_write fd or buf or size error\n");
        exit(EXIT_SUCCESS);
    }
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_pipe_write
// 函数功能  :
//
// 输入参数  :
//
// 返回参数  :
//
//-----------------------------------------------------------------------------
int fn_set_fd_noblock(int fd)
{
    int flag;
    if (flag = fcntl(fd, F_GETFL, 0) <0)
        perror("get flag");
    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) < 0)
        perror("set flag");
    return 0;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_pipe_write
// 函数功能  :
//
// 输入参数  :
//
// 返回参数  :
//
//-----------------------------------------------------------------------------
int fn_uart_pipe_init(void)
{
    // 关闭串口到wifi的 串口的读入端
    close(s2w_pipe_fd[0]);
    // 关闭wifi到串口的 串口的写入端
    close(w2s_pipe_fd[1]);

    // 在wifi到串口时，设定串口读取为无阻塞
    fn_set_fd_noblock(w2s_pipe_fd[0]);
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_pipe_write
// 函数功能  :
//
// 输入参数  :
//
// 返回参数  :
//
//-----------------------------------------------------------------------------
int fn_wifi_pipe_init(void)
{
    // 关闭串口到wifi的 串口的写入端
    close(s2w_pipe_fd[1]);
    // 关闭wifi到串口的 串口的读入端
    close(w2s_pipe_fd[0]);

    // 在串口到wifi时，设定wifi读取为无阻塞
    fn_set_fd_noblock(s2w_pipe_fd[0]);
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_pipe_write
// 函数功能  :
//
// 输入参数  :
//
// 返回参数  :
//
//-----------------------------------------------------------------------------
int fn_uart_pipe_write(char *buf,int len)
{
    int cnt;
    cnt = fn_pipe_write(s2w_pipe_fd[1],buf,len);
    return cnt;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_pipe_write
// 函数功能  :
//
// 输入参数  :
//
// 返回参数  :
//
//-----------------------------------------------------------------------------
int fn_uart_pipe_read(char *buf,int size)
{
    int cnt = 0;
    cnt = fn_pipe_read_noblock(w2s_pipe_fd[0],buf,size);
    return cnt;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_pipe_write
// 函数功能  :
//
// 输入参数  :
//
// 返回参数  :
//
//-----------------------------------------------------------------------------
int fn_wifi_pipe_write(char *buf,int len)
{
    int cnt;
    cnt = fn_pipe_write(w2s_pipe_fd[1],buf,len);
    return cnt;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_pipe_write
// 函数功能  :
//
// 输入参数  :
//
// 返回参数  :
//
//-----------------------------------------------------------------------------
int fn_wifi_pipe_read(char *buf,int size)
{
    int cnt = 0;
    cnt = fn_pipe_read_noblock(s2w_pipe_fd[0],buf,size);
    return cnt;
}

