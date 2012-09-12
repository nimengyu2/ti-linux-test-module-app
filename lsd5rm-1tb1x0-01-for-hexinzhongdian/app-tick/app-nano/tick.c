#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sched.h>
#include <time.h>
#define MILLION 1000000L
#define GETTIME_DELAY 66*1000   // 66us

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
		slptm.tv_nsec = 10000000 - GETTIME_DELAY;      //1 us = 1000 ns; 1 ms = 1000 us = 1000000 ns; 10 ms = 10000000 ns  

		if (gettimeofday(&tstart, NULL) == -1) {
			fprintf(stderr, "Failed to get start time\n");
			return 1;
		}
		#if 1
		if (nanosleep(&slptm, &slptm) == -1) {
			printf("Failed to nanosleep");
			return 1;
		}
		#endif
		//usleep(10000-66);

		if (gettimeofday(&tend, NULL) == -1) {
			fprintf(stderr, "Failed to get end time\n");
			return 1;
		}
	
		tdif = MILLION * (tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec);
		if(cnt != 0)
		{
			printf("nanosleep() time is %ld us\n", tdif);
		}
		cnt++; 
	}
	return 0;
}


