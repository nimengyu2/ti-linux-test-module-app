
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include   <stdio.h> 
#include   <linux/rtc.h> 
#include   <sys/ioctl.h> 
#include   <sys/time.h> 
#include   <sys/types.h> 
#include   <fcntl.h> 
#include   <unistd.h> 
#include   <errno.h> 





int main(int argc,char *argv[]) 
{
	unsigned long i;
	unsigned long data = 0;
	int retval = 0;
	unsigned long count = 0;
	int fd = open("/dev/rtc0",O_RDONLY);
	ioctl(fd,RTC_IRQP_SET,4);
	ioctl(fd,RTC_PIE_ON,0);
	for(i = 0;i < 100;i++)
	{
		read(fd,&data,sizeof(unsigned long));
		printf("timer count=%d\n",count++);
	}
	ioctl(fd,RTC_PIE_OFF,0);
	close(fd);
	return 0;
}



