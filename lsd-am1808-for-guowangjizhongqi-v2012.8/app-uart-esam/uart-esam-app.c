#include <stdio.h> 
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include "uart0_api.h"
#include "am1808_gpio.h"


#define ESAM_BUF_SIZE  30
unsigned char g_pu8_write_buf[ESAM_BUF_SIZE];
unsigned char g_pu8_read_buf[ESAM_BUF_SIZE];
unsigned long g_u32_write_cnt = 0;
unsigned long g_u32_read_cnt = 0;

int fn_do_esam_write_read_once(void)
{
	int i;
	unsigned char pu8_read_tmp[ESAM_BUF_SIZE];	
	unsigned long u32_read_tmp_cnt = 0;
	if(g_u32_write_cnt != 0)
	{
		g_u32_write_cnt = fn_write_data_block_uart0(g_pu8_write_buf,g_u32_write_cnt);
	}
	u32_read_tmp_cnt = fn_read_data_block_uart0(pu8_read_tmp,100);
	if(u32_read_tmp_cnt < 0) 
	{
		perror("read error"); 
		return -1;
	}
	else
	{
		if(g_u32_write_cnt != 0)
		{
			printf("have send data len=%d,data:\n",g_u32_write_cnt);
			for(i = 0; i < g_u32_write_cnt; i++)
			{
				printf("0x%02x ",g_pu8_write_buf[i]);
			}	
			printf("\n"); 
				

			printf("Row Recieve data,len=%d,data:\n",u32_read_tmp_cnt);  
			for(i = 0; i < u32_read_tmp_cnt; i++)
			{
				printf("0x%02x ",pu8_read_tmp[i]);
			}
			printf("\n"); 
		}

		for(i = 0; i < g_u32_write_cnt; i++)
		{
			if(pu8_read_tmp[i] != g_pu8_write_buf[i])
			{
				printf("recv data error,because no recv send data\n");
				return -1;
			}
		}
		for(i = g_u32_write_cnt; i < u32_read_tmp_cnt; i++)
		{
			g_pu8_read_buf[i-g_u32_write_cnt] = pu8_read_tmp[i];
		}
		g_u32_read_cnt = u32_read_tmp_cnt - g_u32_write_cnt;

		printf("True Recieve data,len=%d,data:\n",g_u32_read_cnt);  
		for(i = 0; i < g_u32_read_cnt; i++)
		{
			printf("0x%02x ",g_pu8_read_buf[i]);
		}
		printf("\n"); 		
	}	
	printf("\n"); 
}

int fd_am1808_gpio;

int main(int argc, char **argv)
{ 
	char buff[512]; 
	int result;
	int s32_read_num;
	int s32_write_num;
	int i;
	unsigned long u32_cnt = 0;
	// 9600*372
	system("/test/pwm-esam 0");
	system("/test/pwm-esam 1");
	result  = fn_init_uart0(9600,8,2,'E');
	if(result < 0)
	{
		printf("Open uart fail!\n");
		return 0;
	}
	else
	{
		printf("Open uart ok!\n");
	} 

	//fd_am1808_gpio = open("/dev/busiow", 0); 
	//if (fd_am1808_gpio < 0) 
	//{ 
	//perror("open device error"); 
	//exit(1); 
	//} 

	fn_am1808_gpio_init();

    	//ioctl(fd_am1808_gpio,SET_BUS_IO_W_ESAM_RESET,0); 
	fn_set_am1808_gpio_value(SAM_RST,0);
	usleep(200*1000);
	//ioctl(fd_am1808_gpio,SET_BUS_IO_W_ESAM_RESET,1); 
	fn_set_am1808_gpio_value(SAM_RST,1);
	usleep(200*1000);
	//close(fd_am1808_gpio); 
	fn_am1808_gpio_exit();


	// 等待直到读取复位ATR
	printf("\n\nStart read ESAM ATR,pls reset esam\n");
	g_u32_write_cnt = 0;
	fn_do_esam_write_read_once();

#if 0
	// PPS
	printf("Start do PPS\n");  	
	g_pu8_write_buf[0] = 0xFF;
  	g_pu8_write_buf[1] = 0x10;
  	g_pu8_write_buf[2] = 0x94;
  	g_pu8_write_buf[3] = 0x7B;
	g_u32_write_cnt = 4;
	fn_do_esam_write_read_once();

	// 57600*64
	system("/etc/init.d/pru-smartcard-clock stop");
	system("/etc/init.d/pru-smartcard-clock start2");
	result  = fn_init_uart0(57600,8,2,'E');
	if(result < 0)
	{
		printf("Open uart fail!\n");
		return 0;
	}
	else
	{
		printf("Open uart ok!\n");
	} 
	printf("\n");
#endif
//while(1)
{
	// 获取随机数	
	printf("Start get chanllage \n");
	g_pu8_write_buf[0] = 0x00;
  	g_pu8_write_buf[1] = 0x84;
  	g_pu8_write_buf[2] = 0x00;
  	g_pu8_write_buf[3] = 0x00;
  	g_pu8_write_buf[4] = 0x08;
	g_u32_write_cnt = 5;
	fn_do_esam_write_read_once();
	sleep(1);
	
// 公钥验证	
	printf("Start check key \n");
	g_pu8_write_buf[0] = 0x80;
  	g_pu8_write_buf[1] = 0x42;
  	g_pu8_write_buf[2] = 0x00;
  	g_pu8_write_buf[3] = 0x01;
  	g_pu8_write_buf[4] = 0x8C;
	g_u32_write_cnt = 5;
	fn_do_esam_write_read_once();

	sleep(1);
}	
	while(1);

} 


