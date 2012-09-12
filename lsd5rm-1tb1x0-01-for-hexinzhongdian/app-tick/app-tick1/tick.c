
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>




int main(int argc,char *argv[]) 
{
	unsigned int interval = 0;
	struct timeval tv;

	while (1)
	{
		gettimeofday(&tv, 0);

		printf("\n\r tick : %d", (tv.tv_sec) * 1000000 + tv.tv_usec - interval);

		interval = (tv.tv_sec) * 1000000 + tv.tv_usec;

		usleep(10 * 1000);
	}

	return 0;
}



