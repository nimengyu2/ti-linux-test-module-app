//-----------------------------------------------------------------------------
//
//                                 串口转wifi模块程序
//                                  源文件(*.c)
//
//
//                    版权所有(C)2005-2010 利尔达科技有限公司
//
//
// 文件名    : uart_wifi_main.c
// 作者    : nmy
// 生成日期  : 2011-09-21
//
// arm gcc   : arm-none-linux-gnueabi-gcc 4.5.3
//
// 版本记录  : V1.00 2010-09-11 15:30 nmy 创建第一版
//                   2011-09-23 11:46 nmy 增加fork函数
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include "debug.h"
#include "wifi_pipe.h"
#include "uart_pipe.h"
#include "uart_process.h"
#include "wifi_process.h"
#include "shm.h"
#include "signal.h"
#include <config_file_lib.h> // add by toby
#include <data_types.h> // add by toby

void fn_signal_deal(int signo)
{
    printf("PID=%d,receive signal is %d \n",getpid(),signo);
    // 删除管道通信
    fn_pipe_delete();
    // 删除共享内存通信
    fn_shm_delete();

    tcp_server_close();
    tcp_client_close();
    udp_close();

    usleep(100000);
    exit(1);
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  main
// 函数功能  :
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int main (void)
{
    pid_t pid;

    /*
     * 在所有程序执行前，先把网页登录文件的登录记录清除
     * Modify by toby@2011.10.13
     */
    char *pislogin;
    char islogin_buf[2] = {'0', '\0'};

    debug_out("this file compile at %s\n",__TIMESTAMP__);

    if(login_islogin_get(&pislogin) == false)
    {
        debug_out("Read islogin value failed\n");
    }

    if(pislogin[0] == '1')
    {
        if(login_islogin_set(islogin_buf, strlen(islogin_buf)) == false)
        {
            debug_out("Write islogin value failed\n");
        }
        else
        {
            debug_out("Write islogin = 0, Quit WEB login\n");
        }
    }

    // 创建uart和wifi进程通信要用的管道
    fn_pipe_create();

#if 1
    if(signal(SIGTERM,fn_signal_deal) == SIG_ERR)
    {
        perror("cant signal");
        return 0;
    }
#endif

    if(signal(SIGINT,fn_signal_deal) == SIG_ERR)
    {
        perror("cant signal");
        return 0;
    }


    // fork一个子进程用于处理处理收发
    if ((pid = fork()) < 0)
    {
        debug_out("fork error\n");
    }
    else if (pid == 0)   // child process
    {
        debug_out("run in child process\n");
        // 该进程是一个while(1)循环的
        uart_process();
        while(1);
    }
    else // father process
    {
        //sleep(2);
        debug_out("run in father process\n");
        // 父进程用于处理wifi
        wifi_process();
    }


    // 删除管道通信
    fn_pipe_delete();
    // 删除共享内存通信
    fn_shm_delete();
    return 0;
}
