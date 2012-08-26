#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include "am1808_gpio.h"
#include "debug_console.h"

int fd_am1808_gpio;

int fn_am1808_gpio_init(void)
{
	fd_am1808_gpio = open(DEVICE_NAME, 0); 
        if (fd_am1808_gpio < 0) 
	{ 
           perror("open device am335x-gpio error"); 
           //exit(1); 
	    return -1;
        } 
	return 0;
}

int fn_get_am1808_gpio_value(int gpio_num)
{
	unsigned long result = 0xff;	
	ioctl(fd_am1808_gpio,GET_GPIO_VALUE_BASE+gpio_num,&result);
	if(result == 0xff)
		return -1;
	else
		return result;
}


int fn_set_am1808_gpio_value(int gpio_num,int value)
{
	ioctl(fd_am1808_gpio,SET_GPIO_VALUE_BASE+gpio_num,value);
	return 0;
}

int fn_set_am1808_gpio_direct(int gpio_num,int direct)
{
	ioctl(fd_am1808_gpio,SET_GPIO_DIRECT+gpio_num,direct);
	return 0;
}

int fn_am1808_gpio_exit(void)
{
        if (fd_am1808_gpio != 0) 
	{ 
           close(fd_am1808_gpio);
           return 0;
        } 
	return -1;
}


