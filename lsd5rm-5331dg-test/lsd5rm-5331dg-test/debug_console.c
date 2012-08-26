#include <stdio.h> 
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <string.h>
#include "uart0_api.h"
#include "debug_console.h"

void fn_test_uart_log_console_init(void)
{
#if 0
	int baudrate,databit,stopbit;
	int result;
	baudrate = 115200;
	databit = 8;
	stopbit = 1;
	//printf("Uart-test ttyS1 baudrate=%d databit=%d stopbit=%d N/O/E=%s\n",
	//		baudrate,databit,stopbit,"N");
	result  = fn_init_uart0("/dev/ttyS1",baudrate,databit,stopbit,'N');
	if(result < 0)
	{
		printf("Open uart fail!\r\n");
	}
	else
	{
		//printf("Open uart ok!\n");
	}
#endif
}

void fn_test_uart_log_console_write(char* str)
{
	int s32_write_num;	
	//sprintf(send_buff,"send cnt=%d\n",u32_cnt);
	#if 1
	printf("%s",str);
	#endif
	//s32_write_num = fn_write_data_block_uart0(str,strlen(str)); 
	//if(s32_write_num != strlen(str)) 
	//	printf("error");
}

void fn_test_uart_log_console_read(void)
{
	char buff[512]; 
	int s32_read_num;
	if(fn_poll_uart0_recv() == 0)
	{
		s32_read_num = fn_read_data_block_uart0(buff,sizeof(buff));
		if(s32_read_num < 0) 
		{
			perror("read error"); 
		}
		else
		{
			#if 1
			buff[s32_read_num+1] = '\0'; 
			printf("recv string: %s \r\n",buff);  
			//s32_write_num = fn_write_data_block_uart0(buff,s32_read_num); 
			//if(s32_write_num < 0) 
			//printf("error");
			#endif  
			#if 0 
			printf("Uart recv:\n");
			for(i = 0;i < s32_read_num;i++)
			{
				printf("0x%02x ",buff[i]);
			}
			printf("\n");
			#endif
		}
	}	
}

