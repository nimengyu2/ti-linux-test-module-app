#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>

char  SIM_DlyHMSM (unsigned char hours, unsigned char minutes, unsigned char seconds, unsigned short milli)
{
 unsigned int ticks;
 unsigned short loops;
 fd_set rfds;
 int fd = 1;
 struct timeval tv;
 
 FD_ZERO (&rfds);
 FD_SET (fd, &rfds);
 tv.tv_sec = seconds;               //modified by empty 2011/06/20
 tv.tv_usec = milli * 1000 - 233;
 select (0, NULL, NULL, NULL, &tv);
 
 return -4;
}

int fn_delay(int delay)
{
	int i;
	for(i = 0; i < delay;i++)
	{
		
	}
}
 
//void  h_TimerMsProc(void *pdata)
int main(void)
{
	unsigned int interval_sec = 0;
	unsigned int interval_usec = 0;
	unsigned int interval_diff = 0;
	struct timeval tv;
	struct timespec slptm;
 	slptm.tv_sec = 0;
	slptm.tv_nsec = 10000000 - 87000;      //1 us = 1000 ns; 1 ms = 1000 us = 1000000 ns; 10 ms = 10000000 ns  

	while (1)
	{
		gettimeofday(&tv, 0);
		//fn_delay(10000);
		if(tv.tv_sec > interval_sec)
		{
			interval_diff = 1000000 + tv.tv_usec - interval_usec;
		}
		else
		{
			interval_diff = tv.tv_usec - interval_usec;
		}
		printf("tv=%d\n",interval_diff);

		interval_sec = tv.tv_sec;
		interval_usec = tv.tv_usec;

		// —” ±10ms
		//SIM_DlyHMSM(0, 0, 0, 10);
		//nanosleep(&slptm, &slptm);
		usleep(10000-89);
	}
}

