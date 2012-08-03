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
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "uart0_api.h"
#include "uart_process.h"
#include "config.h"
#include "debug.h"
#include "shm.h"
//#include "uart/s2wifi.h"

unsigned char pu8_uart_read_buf[1024];
unsigned char pu8_uart_write_buf[1024];
unsigned int  u32_uart_time_ms_cnt;
int  s32_uart_read_cnt;


// uart进程中uart部分初始化
int fn_uart_process_uart_init(void)
{
    int result;
    char parity;
    unsigned long u32_cnt = 0;

    if(s2wifi_config_uart->vparity == 0)
    {
        parity = 'N';
    }
    else if(s2wifi_config_uart->vparity == 1)
    {
        parity = 'O';
    }
    else if(s2wifi_config_uart->vparity == 2)
    {
        parity = 'E';
    }

    #if (UART_DEBUG >= 3)
    debug_out("baudrate is %d,datasize is %d,stop bit is %d,parity is %c\n",
                s2wifi_config_uart->vbaudrate,
                s2wifi_config_uart->vdatasize,
                s2wifi_config_uart->vstop,
                parity);
    #endif

    //result  = fn_init_uart0(9600,8,1,'N');
    result  = fn_init_uart0(s2wifi_config_uart->vbaudrate,
                            s2wifi_config_uart->vdatasize,
                            s2wifi_config_uart->vstop,
                            parity);
    if(result < 0)
    {
        #if (UART_DEBUG >= 1)
        debug_out("Open uart fail!\n");
        #endif
        return -1;
    }
    else
    {
        #if (UART_DEBUG >= 3)
        debug_out("Open uart ok!\n");
        debug_out("Pls send data to me\n");
        #endif
    }
}

extern int at_uart_process(void);
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
int uart_process (void)
{
    // 初始化管道
    fn_uart_pipe_init();
    // 初始化共享内存区域
    fn_uart_shm_init();

    // 必须要延时一会，否则如果串口参数没有正确配置
    // 则会导致串口初始化失败，而且串口通信会不正常
    //usleep(200*1000);
    //sleep(4);
    debug_out("uart process:waitting for wifi process init......\n");
    while((s2wifi_config_uart->wifi_process_init_ok) == 0);
    debug_out("uart process:wifi process init ok\n");

while(1)
{
    // AP 模式
    if(s2wifi_config_uart->wifi_mode == 1)
    {
        // 改函数内部也是一个while循环
        at_uart_process();
        debug_out("out of at_uart_process()\n");
        //fn_wifi_mode_write(0);
        s2wifi_config_uart->wifi_mode = 0;
        debug_out("change ap mode to client mode\n");
        fn_uart_pipe_write("---",3);
    }
    // 客户端模式
    else if(s2wifi_config_uart->wifi_mode == 0)
    {
        // 执行下面的代码
        // 初始化uart0  用于uart转wifi
        fn_uart_process_uart_init();
        while(1)
        {
            usleep(100000);
    #if 0
            #if (PIPE_DEBUG >= 1)
            // 管道读写测试，这里是两个管道，供读写

            if(u32_uart_time_ms_cnt > 2000)
            {
                u32_uart_time_ms_cnt = 0;
                fn_uart_pipe_write("uart write \n",strlen("uart write \n"));
            }

            else
            {
                u32_uart_time_ms_cnt += 100;
            }


            s32_uart_read_cnt = fn_uart_pipe_read(
                        pu8_uart_read_buf,sizeof(pu8_uart_read_buf));
            if(s32_uart_read_cnt > 0)
            {

                debug_out("uart receive str is:\n%s",pu8_uart_read_buf);
            }
            #endif


            #if (SHM_DEBUG >= 2)
            // 共享内存测读取测试
            fn_uart_shm_test();
            #endif

    #endif
            // 将从wifi接受的数据发送到uart上
            s32_uart_read_cnt = fn_uart_pipe_read(
                        pu8_uart_read_buf,sizeof(pu8_uart_read_buf));
            if(s32_uart_read_cnt > 0)
            {
                 pu8_uart_read_buf[s32_uart_read_cnt] = 0;
                 debug_out("[wifi->uart] in uart_process str is: %s\n",pu8_uart_read_buf);
				 debug_out("[wifi->uart] in uart_process hex is: ");
				 int u8_i;
				 for(u8_i = 0;u8_i<s32_uart_read_cnt ;u8_i++)
				 {
				    debug_out("0x%02x ",pu8_uart_read_buf[u8_i]);
				 }
				 debug_out("\n\n");

                 fn_write_data_block_uart0(pu8_uart_read_buf,
                                    s32_uart_read_cnt );
            }

    // 测试串口转wifi功能
    #if 1
            // 将串口接受到的数据通过wifi-net发送出去
            if(fn_poll_uart0_recv() == 0)
            {
                s32_uart_read_cnt = fn_read_data_block_uart0(pu8_uart_read_buf,
                                sizeof(pu8_uart_read_buf)-1);
                if(s32_uart_read_cnt< 0)
                {
                    #if (UART_DEBUG >= 2)
                    debug_out("read error");
                    #endif
                }
                else
                {
                    // 将uart接受到的数据发送给管道pipe
                    fn_uart_pipe_write(pu8_uart_read_buf,s32_uart_read_cnt);

                    #if (UART_DEBUG >= 2)
                    debug_out("[uart->wifi] in uart_process: len=%d\n",s32_uart_read_cnt);
                    pu8_uart_read_buf[s32_uart_read_cnt+1] = '\0';
                    //printf("\n%s\n", pu8_uart_read_buf);

                    // 回写
                    //s32_write_num = fn_write_data_block_uart0(buff,s32_read_num);
                    //if(s32_write_num < 0)
                    //printf("error");

                    #endif
                }
            }
    #endif

        }
    }
    else  // wifi 模式错误
    {
        debug_out("wifi mode error\n");
        while(1);
    }
}//while(1)



    return 0;
}
