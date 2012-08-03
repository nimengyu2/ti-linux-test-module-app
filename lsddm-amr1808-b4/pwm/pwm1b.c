#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 


#define MOTOR_MAGIC 'P'
#define SET_PWM_PIN_HIGH 	_IOW(MOTOR_MAGIC, 2,int)
#define SET_PWM_PIN_LOW 	_IOW(MOTOR_MAGIC, 3,int)
#define SET_PWM_ON 			_IOW(MOTOR_MAGIC, 4,int)
#define SET_PWM_OFF 		_IOW(MOTOR_MAGIC, 5,int)
#define SET_PWM_SLOW_ON 	_IOW(MOTOR_MAGIC, 6,int)


int main(int argc, char **argv) 
{ 
        int on; 
        int led_no; 
        int fd; 
        if ((sscanf(argv[1], "%d", &led_no) != 1) || (sscanf(argv[2],"%d", &on) != 1) || (argc != 3))
        {
           fprintf(stderr, "Usage: pwm no num\n"); 
           exit(1); 
        }

        fd = open("/dev/pwm", 0); 
        if (fd < 0) 
		{ 
           perror("open device leds error"); 
           exit(1); 
        } 
        printf("led_no=%d,on=%d\n",led_no,on); 
        if(led_no == 2)
        {
           ioctl(fd, SET_PWM_PIN_HIGH,on); 
        }
        else if(led_no == 3)
        {
          ioctl(fd, SET_PWM_PIN_LOW,on);  
        }
        else if(led_no == 4)
        {
           ioctl(fd, SET_PWM_ON,on); 
        }
        else if(led_no == 5)
        {
           ioctl(fd, SET_PWM_OFF,on); 
        }
		else if(led_no == 6)
        {
           ioctl(fd, SET_PWM_SLOW_ON,on); 
        }
        while(1);
        close(fd); 
        return 0;
} 
