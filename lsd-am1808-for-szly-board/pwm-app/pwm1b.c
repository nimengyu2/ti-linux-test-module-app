#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include "pwm1b.h"
 
#define MOTOR_MAGIC 'P'
#define SET_PWM_ON 					_IOW(MOTOR_MAGIC, 4,int)
#define SET_PWM_OFF 				_IOW(MOTOR_MAGIC, 5,int)
#define SET_PWM_HIGH_LEVEL_DIV 		_IOW(MOTOR_MAGIC, 6,int)

// PWM 输出开启,并设定PWM的值
int fn_pwm_output_on(unsigned long u32_freq)
{ 
	int fd; 	
	fd = open("/dev/pwm", 0); 
        if (fd < 0) 
	{ 
           perror("open device leds error"); 
          return 0;
        } 
       ioctl(fd, SET_PWM_ON,u32_freq); 
       close(fd); 
}

// PWM 输出开启,并设定PWM的值
int fn_pwm_output_change_div(unsigned long u32_div)
{ 
	int fd; 	
	fd = open("/dev/pwm", 0); 
        if (fd < 0) 
	{ 
           perror("open device leds error"); 
          return 0;
        } 
       ioctl(fd, SET_PWM_HIGH_LEVEL_DIV,u32_div); 
       close(fd); 
}

// PWM输出关闭
int fn_pwm_output_off(void)
{ 
	int fd; 	
	fd = open("/dev/pwm", 0); 
        if (fd < 0) 
	{ 
           perror("open device leds error"); 
           return 0;
        } 
       ioctl(fd, SET_PWM_OFF,0); 
       close(fd); 
}


// 主函数
int main(int argc, char **argv) 
{ 
        int data; 
        int cmd; 
        if ((sscanf(argv[1], "%d", &cmd) != 1) || (sscanf(argv[2],"%d", &data) != 1) || (argc != 3))
        {
           fprintf(stderr, "Usage: pwm no num\n"); 
           exit(1); 
        }
	if(cmd == 1)
	{
		fn_pwm_output_on(data);
	}
	if(cmd == 0)
	{
		fn_pwm_output_off();
	}
	if(cmd == 2)
	{
		fn_pwm_output_change_div(data);
	}
    return 0;
} 

