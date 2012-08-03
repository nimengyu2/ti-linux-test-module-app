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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "shm.h"
#include "config.h"
#include "debug.h"

// 指向s2wifi的配置结构体
struct s2wifi_config *s2wifi_config_uart;

// 指向s2wifi的配置结构体
struct s2wifi_config *s2wifi_config_wifi;

int fn_uart_shm_init(void)
{
    int shmid;
    void *shared_memory = (void *)0;
    #if (SHM_DEBUG >= 1)
    debug_out("uart:shm init\n");
    #endif
    shmid = shmget((key_t)(M_S2WIFI_CONFIG_KEY),
            sizeof(struct s2wifi_config), 0666 | IPC_CREAT);
    if(shmid == -1)
    {
    #if (SHM_DEBUG >= 1)
    debug_out("uart:shmget failed\n");
    #endif
    return -1;
    }
    #if (SHM_DEBUG >= 3)
    debug_out("uart:before nattch\n");
    system("ipcs -m");
    #endif
    shared_memory = shmat(shmid, (void *)0, 0);
     if(shared_memory == (void *)-1)
     {
            #if (SHM_DEBUG >= 1)
        debug_out("uart:shmmat failed\n");
        #endif
        return -1;
     }
    #if (SHM_DEBUG >= 1)
    debug_out("uart:Memory attached at %X\n", (int)shared_memory);
    #endif

    s2wifi_config_uart = (struct s2wifi_config *)shared_memory;

    #if (SHM_DEBUG >= 3)
    debug_out("uart:After nattch\n");
    system("ipcs -m");
    #endif
}


int fn_uart_shm_test(void)
{
    #if (SHM_DEBUG >= 2)
    if(s2wifi_config_uart->config_change == 1)
    {
        s2wifi_config_uart->config_change = 0;
        debug_out("uart read shm\n");
        //debug_out("s2wifi_config_uart->vsocketlp = %d\n",s2wifi_config_uart->vsocketlp);
        fn_printf_config_information(*s2wifi_config_uart);
    }
    #endif
}


int fn_wifi_shm_init(void)
{
    int shmid;
    void *shared_memory = (void *)0;
    #if (SHM_DEBUG >= 1)
    debug_out("wifi:shm init\n");
    #endif
    shmid = shmget((key_t)(M_S2WIFI_CONFIG_KEY),
                sizeof(struct s2wifi_config), 0666 | IPC_CREAT);
    if(shmid == -1)
    {
        #if (SHM_DEBUG >= 1)
        debug_out("wifi:shmget failed\n");
        #endif
        return -1;
    }
    #if (SHM_DEBUG >= 3)
    debug_out("wifi:before nattch\n");
    system("ipcs -m");
    #endif
    shared_memory = shmat(shmid, (void *)0, 0);
    if(shared_memory == (void *)-1)
    {
        #if (SHM_DEBUG >= 1)
        debug_out("wifi:shmmat failed\n");
        #endif
        return -1;
    }
    #if (SHM_DEBUG >= 1)
    debug_out("wifi:Memory attached at %X\n", (int)shared_memory);
    #endif

    s2wifi_config_wifi = (struct s2wifi_config *)shared_memory;

    #if (SHM_DEBUG >= 3)
    debug_out("wifi:After nattch\n");
    system("ipcs -m");
    #endif
    return 0;
}


int fn_wifi_shm_test(void)
{
    static int s32_test_cnt = 0;
    #if (SHM_DEBUG >= 2)
    s32_test_cnt++;
    if(s32_test_cnt > 10)
    {
        s32_test_cnt = 0;
        debug_out("wifi write shm \n");
        s2wifi_config_wifi->vsocketlp ++;
        s2wifi_config_wifi->config_change = 1;
    }
    #endif
}

int fn_shm_delete(void)
{
    shmdt(s2wifi_config_wifi);
    shmdt(s2wifi_config_uart);
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

