#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

// Ö÷º¯Êý
int main(int argc, char **argv) 
{ 
	int fd;
	//int fd1;
	int ret;
	unsigned char u8_wr_buf[256];
	unsigned long block;
	unsigned int u32_wr_cnt = 0;
	int count = 0;
	
	fd = fopen("/aaaa.txt","w+");
	if (fd < 0) 
	{ 
           	printf("open device error\n"); 
          	return 0;
        }
	else
	{
		printf("open file ok\n");
	} 
	

	while(1)
	{
		snprintf(u8_wr_buf,sizeof(u8_wr_buf),"write data = %04d\n",count);
		ret = fwrite(u8_wr_buf,1,strlen(u8_wr_buf),fd);
		if(ret != strlen(u8_wr_buf))
		{
			printf("ERROR write\n");
		}
		else
		{
			
			printf("%s",u8_wr_buf);
		}
		count++;
		//fflush(fd);
		sleep(1);
	}
	
	close(fd);
		
    return 0;
} 

