#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include "am1808_gpio.h"
#include "debug_console.h"
#include "test_eth.h"



int fn_test_am1808_eth(void)
{
    	int ret;
    	ret = system("ping 192.168.1.1 -c 1 -w 1");
    	if(ret != 0)
    	{
		fn_test_uart_log_console_write("test_eth failed\r\n");
		return -1;
    	}
	else
	{
		
	}
#if 0
	ret = system("ping 192.168.1.122 -c 2");
    	if(ret == -1)
    	{
		fn_test_uart_log_console_write("test_eth failed\r\n");
		return -1;
    	}
	else
	{
		
	}
#endif	
	//printf("fn_test_am1808_eth ret=%d\r\n",ret);
	fn_test_uart_log_console_write("test_eth ok\r\n");
	return 0;
}






