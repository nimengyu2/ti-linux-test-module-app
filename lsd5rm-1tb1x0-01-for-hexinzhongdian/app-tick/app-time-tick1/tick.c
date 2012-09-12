
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include<signal.h>

#define MILLION 1000000L
#define GETTIME_DELAY 66*1000   // 66us
#define COUNT 200

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

void signalrm(int sig)
{
	signal(SIGALRM,signalrm);
	
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
	//fn_delay(10000);   // 模拟应用程序时间
}
 
//void  h_TimerMsProc(void *pdata)
int main(void)
{
	struct itimerval   itv,oldtv;
	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 10000;
 	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = 10000;
 
	signal(SIGALRM,signalrm);
	setitimer(ITIMER_REAL,&itv,&oldtv);  // 定时器
	while(1)
	{
		sleep(1);
	}
	return 0;
}

