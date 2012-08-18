#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <mtd/mtd-user.h>
unsigned char u8_rd_buf[4096];
// Ö÷º¯Êý
int main(int argc, char **argv) 
{ 
	int fd;
	int ret;
	//unsigned char u8_wr_buf[256] = "1234567890";
	unsigned long block;
	//unsigned int u32_wr_cnt = 0;
	
	//int regcount;

	if ((sscanf(argv[1], "%d", &block) != 1))
        {
           fprintf(stderr, "read-mtd <block>\n"); 
           exit(1); 
        }

	printf("block=%d\n",block);
	
	fd = fopen("/dev/mtd2","r+");
	if (fd < 0) 
	{ 
           perror("open device error"); 
          return 0;
        } 
	
	block = block * 0x10000;

	ret = fseek(fd, block, SEEK_SET); 
	if(ret != 0)
	{
		printf("fseek error\n");
	}
	else
	{
		printf("fseek ok\n");
	}

#if 0
	ret = fwrite(u8_wr_buf,1,u32_wr_cnt,fd);
	if(ret != 64)
	{
		printf("fwrite error\n");
	}
	else
	{
		printf("fwrite ok\n");
	}
#endif
#if 1
	ret = fread(u8_rd_buf,1,4096,fd);
	if(ret != sizeof(u8_rd_buf))
	{
		printf("fread error\n");
	}
	else
	{
		printf("fread ok\n");
		printf("recv message: %s \r\n\n",u8_rd_buf);
	}
#endif
#if 0
	ret = fwrite(u8_wr_buf,1,u32_wr_cnt,fd);
	if(ret != u32_wr_cnt)
	{
		printf("fwrite error\n");
	}
	else
	{
		printf("fwrite ok\n");
	}
#endif
	fclose(fd);
		
    return 0;
} 

