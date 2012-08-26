#include <stdio.h> 
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <string.h>
#include "uart0_api.h"
#include "debug_console.h"
#include "test_gpio.h"
#include "test_eth.h"
#include "test_usb_sata.h"


int main(int argc, char **argv)
{ 
	unsigned long u32_cnt = 0;
	fn_test_uart_log_console_init();
	fn_test_uart_log_console_write("/------------------------------------------------/\r\n");
	fn_test_uart_log_console_write("lierda board: lsd-smd-am1808 test log console\r\n");
	fn_test_uart_log_console_write("/------------------------------------------------/\r\n");

	fn_am1808_gpio_init();

	//sleep(1);
	fn_test_am1808_gpio();
	fn_test_am1808_eth();
	//fn_test_am1808_usb20();
	//fn_test_am1808_usb11();
	fn_test_am1808_mmc();
	fn_test_am1808_nand();
	//sleep(6);
	sleep(1);
	fn_test_am1808_usb(1);
	fn_test_am1808_usb(2);
	//fn_test_am1808_sata();
	//while(1);
	return 0;

	while (1) // 循环读取数据
	{    
		fn_test_uart_log_console_read();
		usleep(1000*10);
#if 0		
		u32_cnt++;
		if((u32_cnt%100) == 0)
		{
			fn_test_uart_log_console_write("time out \r\n");	
		}
#endif
		
	} 
	return 0;
} 


