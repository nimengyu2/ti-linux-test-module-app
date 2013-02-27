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
	
	fd = open("/aaaa.txt",O_WRONLY | O_TRUNC | O_CREAT | O_SYNC);
	if (fd < 0) 
	{ 
           	printf("open device error\n"); 
          	return 0;
        }
	else
	{
		printf("open file ok\n");
	} 
	
	int i;
	for(i = 0;i<40;i++)
	{
		snprintf(u8_wr_buf,sizeof(u8_wr_buf),"write data = %04d\n",count);
		ret = write(fd,u8_wr_buf,strlen(u8_wr_buf));
		if(ret != strlen(u8_wr_buf))
		{
			printf("ERROR write\n");
		}
		else
		{
			
			printf("%s",u8_wr_buf);
		}
		count++;
		sleep(1);
	}
	
	close(fd);
		
    return 0;
} 

