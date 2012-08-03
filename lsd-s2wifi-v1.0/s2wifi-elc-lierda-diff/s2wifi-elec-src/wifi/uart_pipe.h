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


#ifndef __UART_PIPE_H__
#define __UART_PIPE_H__

// uart 进程 初始化pipe
extern int fn_uart_pipe_init(void);
// uart 进程 写数据pipe
extern int fn_uart_pipe_write(char *buf,int len);
// uart 进程读取pipe，无阻塞读取，如果没有数据返回0
extern int fn_uart_pipe_read(char *buf,int size);


#endif
