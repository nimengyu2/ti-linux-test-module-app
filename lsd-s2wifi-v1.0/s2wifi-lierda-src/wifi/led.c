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
#include <sys/ioctl.h> 
#include <led.h>
#include <debug.h>

#define MOTOR_MAGIC 'L'
#define SET_LED              _IOW(MOTOR_MAGIC, 1,int)

//-----------------------------------------------------------------------------
//
// 函数名称  :   
// 函数功能  :  
//
// 输入参数  :  char* led_name   指向led名字
//			int value   1 表示led亮  0 表示灭
//
// 返回参数  :  如果执行成功返回0,否则返回-1
// 
//-----------------------------------------------------------------------------
int fn_led_set (char* led_name,int value)
{
	int fd;	
	fd = open(led_name, 0);    
	if (fd < 0) 
	{ 
		#if(LED_DEBUG >= 3)
		debug_out("led:open dev %s failed \n",led_name);
		#endif		
		return -1;
        } 
	if(value == 0)
	{
		ioctl(fd, SET_LED,1); 
	}
	else
	{
		ioctl(fd, SET_LED,0); 
	}
	close(fd); 
	return 0;
}
