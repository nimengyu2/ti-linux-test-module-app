
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>






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

int msleep (unsigned long milisec, int temp)
{
     struct timespec req = {0}, rem = {0};
     time_t sec = (int)(milisec / 1000);
     milisec = milisec - (sec * 1000);
     req.tv_sec = sec;            /*√Î*/
     req.tv_nsec = milisec * 1000000L;    /*ƒ…√Î*/
     while (nanosleep (&req, &req) == -1 && errno == EINTR) {
         //printf ("≤‚ ‘-%d±ª–≈∫≈÷–∂œ, £”‡ ±º‰Œ™: %ld√Î%ldƒ…√Î\n", temp, req.tv_sec, req.tv_nsec);
         continue;
     }
     return (1);
}


 
//void  h_TimerMsProc(void *pdata)
int main(void)
{
	unsigned int interval_sec = 0;
	unsigned int interval_usec = 0;
	unsigned int interval_diff = 0;

	struct timeval tv;
 
 
	while (1)
	{
		gettimeofday(&tv, 0);
		//printf("tv...%d\r\n", (tv.tv_sec % 10) * 1000000 + tv.tv_usec - interval);
		//interval = (tv.tv_sec % 10) * 1000000 + tv.tv_usec;

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
		msleep(10,1);
	}
}

