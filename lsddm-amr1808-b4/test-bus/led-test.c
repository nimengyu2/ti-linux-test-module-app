#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 
#define DEVICE_NAME "bus-cs4" //设备名(/dev/led) 

#define MOTOR_MAGIC 'P'
#define WRITE_DATA			_IOW(MOTOR_MAGIC, 2,int)
#define READ_DATA 			_IOW(MOTOR_MAGIC, 3,int)


int main(int argc, char **argv) 
{ 
        int on; 
        int led_no; 
        int fd; 
        if ((sscanf(argv[1], "%d", &led_no) != 1) || (sscanf(argv[2],"%d", &on) != 1) )
        {
           fprintf(stderr, "Usage: leds led_no 0|1\n"); 
           exit(1); 
        }

        fd = open("/dev/bus-cs4", 0); 
        if (fd < 0) 
	{ 
           perror("open device leds error"); 
           exit(1); 
        } 
        printf("led_no=%d,on=%d\n",led_no,on); 
      
        ioctl(fd, WRITE_DATA,on);     
        close(fd); 
        return 0;
} 
