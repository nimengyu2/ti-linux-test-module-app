#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include "am1808_gpio.h"

int fd_am1808_gpio_key;

int fn_am1808_gpio_init(void)
{
	fd_am1808_gpio_key = open("/dev/am1808-all-gpio", 0); 
        if (fd_am1808_gpio_key < 0) 
	{ 
           perror("open device am1808-all-gpio error"); 
           //exit(1); 
	    return -1;
        } 
	return 0;
}

int fn_get_am1808_gpio_value(int gpio_num)
{
	unsigned long result = 0xff;	
	ioctl(fd_am1808_gpio_key,GET_GPIO_VALUE_BASE+gpio_num,&result);
	if(result == 0xff)
		return -1;
	else
		return result;
}


int fn_set_am1808_gpio_value(int gpio_num,int value)
{
	ioctl(fd_am1808_gpio_key,SET_GPIO_VALUE_BASE+gpio_num,value);
	return 0;
}

int fn_set_am1808_gpio_direct(int gpio_num,int direct)
{
	ioctl(fd_am1808_gpio_key,SET_GPIO_DIRECT+gpio_num,direct);
	return 0;
}

int fn_am1808_gpio_exit(void)
{
        if (fd_am1808_gpio_key != 0) 
	{ 
           close(fd_am1808_gpio_key);
           return 0;
        } 
	return -1;
}


