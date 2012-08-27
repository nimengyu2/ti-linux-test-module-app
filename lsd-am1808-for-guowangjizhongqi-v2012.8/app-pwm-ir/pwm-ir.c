#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 

#define DEVICE_NAME "all-pwm" //设备名(/dev/pwm) 

#define MOTOR_MAGIC 'P'
#define PWM1_IOCTL_SET	 _IOW(MOTOR_MAGIC, 2,int)
#define PWM1_IOCTL_STOP	 _IOW(MOTOR_MAGIC, 3,int)

struct pwm_config
{
        unsigned long  pwm_num;//通道号
	unsigned long  pwm_ctrl;//模式
	unsigned long  pwm_div;//分频比
	unsigned long  pwm_period;//周期  计数值
	unsigned long  pwm_data;//数据  占空比
} ;

int fd_pwm;

int pwm_open(void)
{
	int fd; 	
	fd_pwm = open("/dev/all-pwm", 0); 
        if (fd_pwm < 0) 
	{ 
           perror("open device error"); 
          return 0;
        } 
	return 0;
}

void pwm_close(void)
{
	close(fd_pwm);
}

void pwm_stop(int num)
{
    struct pwm_config *p;
    struct pwm_config pwm;
    unsigned long freq;
    pwm.pwm_num=num;
    p = &pwm;
    freq = (unsigned long)p;
    ioctl(fd_pwm, PWM1_IOCTL_STOP, freq);
}

void pwm_set(unsigned long num,unsigned long mode,unsigned long div,unsigned long period,unsigned long data)
{
    struct pwm_config *p;
    struct pwm_config pwm;
    unsigned long freq;
    pwm_stop(num);
    pwm.pwm_num=num;
    pwm.pwm_ctrl =mode;
    pwm.pwm_div =div;
    pwm.pwm_period =period;
    pwm.pwm_data =(period*data)/100;
    p = &pwm;
    freq = (unsigned long)p;
    ioctl(fd_pwm, PWM1_IOCTL_SET, freq);
}


// 主函数
int main(int argc, char **argv) 
{ 
      int data; 
        int cmd; 
	int num;
	unsigned long freq;

	if ((sscanf(argv[1], "%d", &cmd) != 1))
        {
           fprintf(stderr, "Usage: pwm-ir on/off\n"); 
           exit(1); 
        }
	printf("freq=%d,cmd=%d\n",38000,cmd);
        pwm_open();
	if(cmd == 1)
	{
		printf("pwm on\n");
		pwm_set(0,0,0,150000000/38000,50);
	}
	else
	{
		printf("pwm off\n");
		pwm_stop(num);
	}
	pwm_close();
	
        return 0;
} 

