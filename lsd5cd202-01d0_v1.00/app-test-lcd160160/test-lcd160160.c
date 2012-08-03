#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 
#define DEVICE_NAME "buslcd" //Éè±¸Ãû(/dev/lcd) 

#define MOTOR_MAGIC 's'
#define LCD_WRITE_CMD	_IOW(MOTOR_MAGIC, 0,int)
#define LCD_WRITE_DATA	_IOW(MOTOR_MAGIC, 1,int)


int main(int argc, char **argv) 
{ 
        int pin; 
        int cmd; 
        int fd; 
		int i;
		unsigned long result;
        
        fd = open("/dev/buslcd", 0); 
        if (fd < 0) 
		{ 
           perror("open device am1808-lcd160160 error"); 
           exit(1); 
        } 
		//ioctl(LCD_INIT,0);
		usleep(1000);
		printf("start output\n");
        while(1)
		{		
			ioctl(fd,LCD_WRITE_CMD,0xF0);
			//printf("LCD_WRITE_CMD 1\n");
			usleep(30);
			ioctl(fd,LCD_WRITE_DATA,0x0F);
			//printf("LCD_WRITE_CMD 2\n");
			usleep(30);
		}
        close(fd); 
        return 0;
} 
