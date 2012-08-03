#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 
#define MOTOR_MAGIC 'L'
#define SET_LED2 _IOW(MOTOR_MAGIC, 2,int)
#define SET_LED3 _IOW(MOTOR_MAGIC, 3,int)
#define SET_LED4 _IOW(MOTOR_MAGIC, 4,int)
#define SET_LED5 _IOW(MOTOR_MAGIC, 5,int)


int main(int argc, char **argv) 
{ 
        int on; 
        int led_no; 
        int fd; 
        if ((sscanf(argv[1], "%d", &led_no) != 1) || (sscanf(argv[2],"%d", &on) != 1) || (argc != 3)
            || (on < 0) || (on > 1) || (led_no < 2) || (led_no > 5))
        {
           fprintf(stderr, "Usage: leds led_no 0|1\n"); 
           exit(1); 
        }

        fd = open("/dev/leds", 0); 
        if (fd < 0) 
	{ 
           perror("open device leds error"); 
           exit(1); 
        } 
        printf("led_no=%d,on=%d\n",led_no,on); 
        if(led_no == 2)
        {
           ioctl(fd, SET_LED2,on); 
        }
        else if(led_no == 3)
        {
          ioctl(fd, SET_LED3,on);  
        }
        else if(led_no == 4)
        {
           ioctl(fd, SET_LED4,on); 
        }
        else if(led_no == 5)
        {
           ioctl(fd, SET_LED5,on); 
        }
      
        close(fd); 
        return 0;
} 
