#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 

#define DEVICE_NAME "timer" //设备名(/dev/pwm) 

#define MOTOR_MAGIC 'T'
#define	IOCTL_TIMERINIT  _IOW(MOTOR_MAGIC, 1,int)
#define IOCTL_TIMERSET	 _IOW(MOTOR_MAGIC, 2,int)
#define IOCTL_STATEGATE  _IOW(MOTOR_MAGIC, 3,int)
#define IOCTL_TIMEDEL 	 _IOW(MOTOR_MAGIC, 4,int)

#define PFSTEP_TIMER  1    // Timer2 TM12
#define STEP_TIMER    2    // Timer2 TM34
#define HEAD_TIMER    3    // Timer3 TM12
//#define SYS_TIMER     4
#define SYS_CLOCK     4    // Timer3 TM34


struct sep_timer
{
    int id;//TIMER号
    unsigned int data;//TIMER初始值
    int timer_flag;
};

struct sep_timer timer;



// 主函数
int main(int argc, char **argv) 
{ 
        long data; 
	long id;
        long cmd; 
	int fd; 
        if ((sscanf(argv[1], "%d", &id) != 1))
        {
           fprintf(stderr, "Usage: timer id cmd data\n"); 
	   fprintf(stderr, "Usage: id= 1 2 3 4\n"); 
           exit(1); 
        }
	 if ((sscanf(argv[2], "%d", &cmd) != 1))
        {
           fprintf(stderr, "Usage: timer id cmd data\n"); 
	   fprintf(stderr, "Usage: cmd= 1 2 3 4\n"); 
           exit(1); 
        }
	 if ((sscanf(argv[3], "%d", &data) != 1))
        {
           fprintf(stderr, "Usage: timer id cmd data\n"); 
	   fprintf(stderr, "Usage: data=interger\n"); 
           exit(1); 
        }
	printf("id=%d,cmd=%d,data=%d\n",id,cmd,data);

	fd = open("/dev/timer", 0); 
        if (fd < 0) 
	{ 
           perror("open device error"); 
          return 0;
        } 

	timer.id = id;
	
	if(cmd == 1) //  IOCTL_TIMERINIT
	{
		ioctl(fd, IOCTL_TIMERINIT,&timer); 
	}
	if(cmd == 2) // IOCTL_TIMERSET
	{
		timer.data = data;
		ioctl(fd, IOCTL_TIMERSET,&timer);
	}
	if(cmd == 3) // IOCTL_STATEGATE
	{
		ioctl(fd, IOCTL_STATEGATE,&timer);
		printf("timer.timer_flag=%d\n",timer.timer_flag);
	}
	if(cmd == 4)  // IOCTL_TIMEDEL
	{
		ioctl(fd, IOCTL_TIMEDEL,&timer);
	}
	close(fd); 
	return 0;
} 

