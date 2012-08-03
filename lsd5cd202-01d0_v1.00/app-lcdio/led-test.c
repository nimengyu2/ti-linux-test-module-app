#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 


#define MOTOR_MAGIC 'm'
#define LCD_INIT 	_IOW(MOTOR_MAGIC, 2,int)
#define LCD_WRITE_CMD	_IOW(MOTOR_MAGIC, 3,int)
#define LCD_WRITE_DATA  _IOW(MOTOR_MAGIC, 4,int)


int main(int argc, char **argv) 
{ 
        int on; 
        int led_no; 
        int fd; 
        if ((sscanf(argv[1], "%d", &led_no) != 1) || (sscanf(argv[2],"%d", &on) != 1))
        {
           fprintf(stderr, "Usage: leds led_no 0|1\n"); 
           exit(1); 
        }
	
	printf("led_on = %02x,on=%02x\n",led_on,on);
        fd = open("/dev/am1808-lcd160160", 0); 
        if (fd < 0) 
	{ 
           perror("open device leds error"); 
           exit(1); 
        }

        ioctl(fd, LCD_WRITE_CMD,on); 
      
        close(fd); 
        return 0;
} 
