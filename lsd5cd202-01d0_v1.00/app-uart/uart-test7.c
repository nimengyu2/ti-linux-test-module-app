#include <stdio.h> 
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include "uart0_api.h"

int main(int argc, char **argv)
{ 
	char buff[512]; 
	int result;
	int s32_read_num;
	int s32_write_num;
	unsigned long u32_cnt = 0;
	int i;
	result  = fn_init_uart0(9600,8,1,'E');
	if(result < 0)
	{
		printf("Open uart fail!\n");
		return 0;
	}
	else
	{
		printf("Open uart ok!\n");
	}
	printf("Pls send data to me\n"); 
	while (1) //循环读取数据 
	{    
		for(i = 0; i < 50; i++)
			buff[i] = 0xaa;
		s32_write_num = fn_write_data_block_uart0(buff,i); 
		if(s32_write_num < 0) 
			printf("error");
		usleep(500000);			
	} 
} 


