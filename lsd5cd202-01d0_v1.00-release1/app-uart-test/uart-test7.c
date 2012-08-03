#include <stdio.h> 
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <string.h>
#include "uart0_api.h"

int main(int argc, char **argv)
{ 
	char buff[512]; 
	char send_buff[512];
	int result;
	int s32_read_num;
	int s32_write_num;
	int i;
	int baudrate,databit,stopbit;
	unsigned long u32_cnt = 0;
	if(argc < 6)
	{
		printf("<exe> ttySUx baudrate databit stopbit N/O/E\n");
		return 0;
	}
	sscanf(argv[2],"%d", &baudrate);
	sscanf(argv[3],"%d", &databit);
	sscanf(argv[4],"%d", &stopbit);
	printf("<exe>=%s ttySUx=%s baudrate=%d databit=%d stopbit=%d N/O/E=%s\n",
			argv[0],argv[1],baudrate,databit,stopbit,argv[5]);
	result  = fn_init_uart0(argv[1],baudrate,databit,stopbit,(argv[5])[0]);
	if(result < 0)
	{
		printf("Open uart fail!\n");
		return 0;
	}
	else
	{
		printf("Open uart ok!\n");
	}
	//printf("Pls send data to me\n"); 
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
				#if 1
				buff[s32_read_num+1] = '\0'; 
				printf("%s recv string: %s \n",argv[1],buff);  
				//s32_write_num = fn_write_data_block_uart0(buff,s32_read_num); 
				//if(s32_write_num < 0) 
				//printf("error");
				#endif  
				#if 0 
				printf("/dev/ttySU0 recv:\n");
				for(i = 0;i < s32_read_num;i++)
				{
					printf("0x%02x ",buff[i]);
				}
				printf("\n");
				#endif
			}
		}
		usleep(1000*10);
		u32_cnt++;
		if((u32_cnt%300) == 0)
		{
			sprintf(send_buff,"%s send cnt=%d\n",argv[1],u32_cnt);
			printf("%s send string: %s",argv[1],send_buff);
			s32_write_num = fn_write_data_block_uart0(send_buff,strlen(send_buff)); 
			if(s32_write_num < 0) 
				printf("error");
			
		}			
		
	} 
} 


