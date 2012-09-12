
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

// 2 4 8 16 32 64 128 256 512 1024
#define TIMER 128
#define COUNT 50
#define MILLION 1000000L

struct timeval tend, tstart;
unsigned long count;
long   tdif;


int fn_delay(int delay)
{
	int i;
	for(i = 0; i < delay;i++)
	{
		
	}
}


int main(int argc,char *argv[]) 
{
	unsigned long i;
	unsigned long data = 0;
	int retval = 0;
	int fd = open("/dev/rtc0",O_RDONLY);
	ioctl(fd,RTC_IRQP_SET,TIMER);
	ioctl(fd,RTC_PIE_ON,0);
	while(1)
	{
		read(fd,&data,sizeof(unsigned long));
		if((count%COUNT) == 0)
		{
			if (gettimeofday(&tend, NULL) == -1) {
				fprintf(stderr, "Failed to get start time\n");
				//return 1;
			}
			if(count != 0)
			{
				tdif = MILLION * (tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec);
				printf(" time is %ld us\n", tdif/COUNT);	  // 求平均值
			}
			tstart = tend;
		
		}
		count++;
		fn_delay(20000);   // 模拟应用程序时间
	}
	ioctl(fd,RTC_PIE_OFF,0);
	close(fd);
	return 0;
}



