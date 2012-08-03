#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include "ecap1-pwm1.h"
 

#define DEVICE_NAME "pwm2" //设备名(/dev/pwm) 

#define MOTOR_MAGIC 'E'
#define SET_PWM2_ON	_IOW(MOTOR_MAGIC, 2,int)
#define SET_PWM2_OFF 	_IOW(MOTOR_MAGIC, 3,int)

struct ecap
{
	unsigned long freq;
	unsigned long duty;	// 0 - 1000
	unsigned long tick_hz;  // 时钟	
	unsigned long period_ticks;  // 周期数
	unsigned long duty_ticks; // 占空比数
	//void __iomem	*mmio_base;
};

struct ecap ecap2_config;


// PWM 输出开启,并设定PWM的值
int fn_pwm_output_on(unsigned long u32_freq)
{ 
	int fd; 	
	fd = open("/dev/pwm2", 0); 
        if (fd < 0) 
	{ 
           perror("open device error"); 
          return 0;
        } 
       ecap2_config.freq = u32_freq;
       ecap2_config.duty = 50;
       ioctl(fd, SET_PWM2_ON,(unsigned long*)(&ecap2_config)); 
       close(fd); 
}

// PWM输出关闭
int fn_pwm_output_off(void)
{ 
	int fd; 	
	fd = open("/dev/pwm2", 0); 
        if (fd < 0) 
	{ 
           perror("open device error"); 
           return 0;
        } 
       ioctl(fd, SET_PWM2_OFF,(unsigned long*)(&ecap2_config)); 
       close(fd); 
}


// 主函数
int main(int argc, char **argv) 
{ 
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
    return 0;
} 

