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
		if(fn_poll_uart0_recv() == 0)
		{
			s32_read_num = fn_read_data_block_uart0(buff,100);
			if(s32_read_num < 0) 
			{
				perror("read error"); 
			}
			else
			{
				printf("\nLen %d\n",s32_read_num);  
				buff[s32_read_num+1] = '\0';    
				printf("\n%s\n", buff);  

				s32_write_num = fn_write_data_block_uart0(buff,s32_read_num); 
				if(s32_write_num < 0) 
				printf("error");
			
			}
		}
		usleep(1000*10);
		u32_cnt++;
		if((u32_cnt%200) == 0)
		{
			printf("u32_cnt = %d\n",u32_cnt);
		}			
		
	} 
} 


