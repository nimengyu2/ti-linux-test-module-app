#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 

// Ö÷º¯Êý
int main(int argc, char **argv) 
{ 
#if 0
        int data; 
        int cmd; 

        if ((sscanf(argv[1], "%d", &cmd) != 1))
        {
           fprintf(stderr, "Usage: pwm on/off\n"); 
           exit(1); 
        }
	if(cmd == 1)
	{
		fn_pwm_output_on(38000);
	}
	if(cmd == 0)
	{
		fn_pwm_output_off();
	}
#endif
	int fd;
	int ret;
	unsigned char u8_wr_buf[64] = "1234567890";
	unsigned char u8_rd_buf[64];

	fd = fopen("/dev/mtd7","r+");
	if (fd < 0) 
	{ 
           perror("open device error"); 
          return 0;
        } 

	ret = fwrite(u8_wr_buf,1,64,fd);
	if(ret != 64)
	{
		printf("fwrite error\n");
	}
	else
	{
		printf("fwrite ok\n");
	}

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
		
    return 0;
} 

