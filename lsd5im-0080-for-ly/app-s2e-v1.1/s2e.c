#include <stdio.h> 
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include "uart0_api.h"
#include "config.h"

#define S2E_DEBUG  1

unsigned char uart2eth[512];
unsigned char eth2uart[512];
unsigned char tcp_client_connect_flag = 0;

#define VERSION "V1.1"

#define NAND_UART2ETH_FILE   "/uart2eth.txt"
#define NAND_ETH2UART_FILE   "/eth2uart.txt"

#define SDCARD_UART2ETH_FILE   "/media/mmcblk0p1/uart2eth.txt"
#define SDCARD_ETH2UART_FILE   "/media/mmcblk0p1/eth2uart.txt"

int fn_file_write_to_end(char *pathname,unsigned char *data,int data_len)
{
	int fd;
	int write_cnt;
	fd = open(pathname,O_RDWR | O_CREAT | O_APPEND,S_IRUSR | S_IWUSR | S_IXUSR);
	if(fd == -1)
	{
		printf("open file %s error\n",pathname);
		return -1;
	}
	write_cnt = write(fd,data,data_len);
	if(write_cnt != data_len)
	{
		printf("write data error,need %d, acture write %d\n",data_len,write_cnt);
		close(fd);
		return write_cnt;
	}
	close(fd);
	return write_cnt;
}

int fn_file_trunc(char *pathname)
{
	int fd;
	int write_cnt;
	fd = open(pathname,O_RDWR | O_TRUNC);
	if(fd == -1)
	{
		printf("open file %s error\n",pathname);
		return -1;
	}
	close(fd);
	return 0;
}


int fn_file_read(char *pathname,unsigned char *data,int data_len)
{
	int fd;
	int read_cnt;
	fd = open(pathname,O_RDWR);
	if(fd == -1)
	{
		printf("open file %s error\n",pathname);
		return -1;
	}
	read_cnt = read(fd,data,data_len);
	if(read_cnt != data_len)
	{
		printf("read data error,need %d, acture write %d\n",data_len,read_cnt);
		close(fd);
		return read_cnt;
	}
	close(fd);
	return read_cnt;
}



int main(int argc, char **argv)
{ 
	int i;
	int ret,write_cnt,read_cnt;
	char pu8_uart_device_name[] = "/dev/ttyO1";
	char pu8_cmd_string[40];

	printf("s2e code version=%s\n",VERSION);

	fn_get_sdcard_exist();
	fn_get_sdcard_configini_exist();
	if(u8_sdcard_configini_exist == 1)
	{
		fn_get_config();
	}	
	fn_printf_config_information();

	pu8_uart_device_name[9] = 0x30 + u8_config_com_number;
	snprintf(pu8_cmd_string,sizeof(pu8_cmd_string),"ifconfig eth0 %s",pu8_socket_localip);
        system(pu8_cmd_string);

	//ret = fn_init_uart0(115200,8,1,'N');
	ret = fn_init_uart0(pu8_uart_device_name,u16_config_com_baudrate,u8_config_com_databit
				,u8_config_com_stopbit,u8_config_com_paritybit);
	if(ret < 0)
	{
		printf("Open uart fail!\n");
		return 0;
	}
	else
	{
		printf("Open uart ok!\n");
	}
	tcp_client_init();
	

	
	while (1) //循环读取数据 
	{    
		if(tcp_client_connect_flag == 0)
		{
			ret = tcp_client_connecting();
			if(ret == 0)  // 表示连接上了
			{
				tcp_client_connect_flag = 1;
				printf("tcp client connect ok\n");
			}
			else
			{
				//printf("connecting..\n");
			}
		}
		else
		{
			ret = tcp_client_read_data(eth2uart,sizeof(eth2uart));
			if(ret > 0)
			{
				if((eth2uart[0] == 'r') && (eth2uart[1] == 'e') && (eth2uart[2] == 'a') && (eth2uart[3] == 'd'))
				{
					printf("socket read data\n");
					if (u8_sdcard_exist == 0)
					{
						read_cnt = fn_file_read(NAND_UART2ETH_FILE,uart2eth,sizeof(uart2eth));
						fn_file_trunc(NAND_UART2ETH_FILE);
					}
					else
					{
						read_cnt = fn_file_read(SDCARD_UART2ETH_FILE,uart2eth,sizeof(uart2eth));
						fn_file_trunc(SDCARD_UART2ETH_FILE);
					}
					
					tcp_client_write(uart2eth,read_cnt);
					#ifdef S2E_DEBUG
					printf("\nETH request data,so send data str=%s\nhex:\n",uart2eth);
					for(i=0;i<read_cnt;i++)
					{
						printf("0x%02x ",uart2eth[i]);
					}
					printf("\n");
					#endif
				}
				else if((eth2uart[0] == 'w') && (eth2uart[1] == 'r') && (eth2uart[2] == 'i') && (eth2uart[3] == 't') && (eth2uart[4] == 'e'))
				{
					printf("socket write data\n");
					write_cnt = fn_write_data_block_uart0(&eth2uart[5],ret-5); 
					if (u8_sdcard_exist == 0)
					{
						fn_file_write_to_end(NAND_ETH2UART_FILE,&eth2uart[5],ret-5);
					}
					else
					{
						fn_file_write_to_end(SDCARD_ETH2UART_FILE,&eth2uart[5],ret-5);
					}
					#ifdef S2E_DEBUG
					printf("\nETH receive data for send to uart str=%s\nhex:\n",&eth2uart[5]);
					for(i=0;i<write_cnt;i++)
					{
						printf("0x%02x ",eth2uart[i]);
					}
					printf("\n");
					#endif
				}
				else
				{
					printf("eth:receive error data\n");
				}
			}
			else if(ret == 0)
			{
				tcp_client_connect_flag = 0;
				tcp_client_close();
				tcp_client_init();
			}
			
		}



		ret = fn_poll_uart0_recv();
		if(ret == 0)  // 有数据可读取
		{
			read_cnt = fn_read_data_block_uart0(uart2eth,sizeof(uart2eth));
			if (u8_sdcard_exist == 0)
			{
				ret = fn_file_write_to_end(NAND_UART2ETH_FILE,uart2eth,read_cnt);
			}
			else
			{
				ret = fn_file_write_to_end(SDCARD_UART2ETH_FILE,uart2eth,read_cnt);
			}
			
			#ifdef S2E_DEBUG
			printf("\nUART receive data str=%s\nhex:\n",uart2eth);
			for(i=0;i<read_cnt;i++)
			{
				printf("0x%02x ",uart2eth[i]);
			}
			printf("\n");
			#endif
		}
		usleep(100000);			
	} 
	tcp_client_close();
} 


