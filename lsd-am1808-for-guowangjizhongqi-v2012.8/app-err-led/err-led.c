#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include "am1808_gpio.h" 


int main(int argc, char **argv) 
{ 
        fn_am1808_gpio_init();
	while(1)
	{
		fn_set_am1808_gpio_value(ERR_LED,1);
		usleep(300000);
		fn_set_am1808_gpio_value(ERR_LED,0);
		usleep(300000);
	}
        fn_am1808_gpio_exit();
        return 0;
} 
