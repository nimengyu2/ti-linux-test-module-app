#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sched.h>
#include <time.h>
#define MILLION 1000000L
#define GETTIME_DELAY 66*1000   // 66us
#define COUNT 100
struct timeval tv;

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
	tv.tv_usec = milli * 1000;
	select (0, NULL, NULL, NULL, &tv);

	return -4;
}

int main(void)
{
	int i;
	struct timespec slptm;
	long   tdif;
	struct timeval tend, tstart;
	unsigned int cnt = 0;

	while(1)
	{
		slptm.tv_sec = 0;
		slptm.tv_nsec = 10000000;      //1 us = 1000 ns; 1 ms = 1000 us = 1000000 ns; 10 ms = 10000000 ns  

		if (gettimeofday(&tstart, NULL) == -1) {
			fprintf(stderr, "Failed to get start time\n");
			return 1;
		}
	
		for(i = 0; i < COUNT; i++)
		{
			#if 0			
			if (nanosleep(&slptm, &slptm) == -1) {
			printf("Failed to nanosleep");
			return 1;
			}
			#endif
			// ÑÓÊ±10ms
			SIM_DlyHMSM(0, 0, 0, 8);
		}
		
		//usleep(10000-66);

		if (gettimeofday(&tend, NULL) == -1) {
			fprintf(stderr, "Failed to get end time\n");
			return 1;
		}
	
		tdif = MILLION * (tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec);
		if(cnt != 0)
		{
			printf("nanosleep() time is %ld us\n", tdif/COUNT);
		}
		cnt++; 
	}
	return 0;
}


