#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
//#include <linux/compiler.h>
#include <mtd/mtd-user.h>

// Ö÷º¯Êý
int main(int argc, char **argv) 
{ 
	int fd;
	//int fd1;
	int ret;
	unsigned char u8_wr_buf[256] = "1234567890";
	unsigned long block;
	unsigned int u32_wr_cnt = 0;

	if ((sscanf(argv[1], "%d", &block) != 1))
        {
           fprintf(stderr, "write-mtd <block> <string>\n"); 
           exit(1); 
        }

	if ((sscanf(argv[2], "%s", u8_wr_buf) != 1))
        {
           fprintf(stderr, "write-mtd <block> <string>\n"); 
           exit(1); 
        }

	u32_wr_cnt = strlen(u8_wr_buf);
	printf("block=%d,write string =%s,str len=%d\n",
		block,u8_wr_buf,u32_wr_cnt);
	
	fd = fopen("/dev/mtd4","r+");
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

#if 1
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
#if 0
	ret = fread(u8_rd_buf,1,64,fd);
	if(ret != 64)
	{
		printf("fread error\n");
	}
	else
	{
		printf("fread ok\n");
		printf("recv message: %s\n",u8_rd_buf);
	}
#endif
	close(fd);
		
    return 0;
} 

