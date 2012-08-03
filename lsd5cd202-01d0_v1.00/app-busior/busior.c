#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 
#define DEVICE_NAME "busior" //Éè±¸Ãû(/dev/lcd) 

#define MOTOR_MAGIC 'r'
#define GET_BUS_IO_BTN_UP		_IOW(MOTOR_MAGIC, 0,int)
#define GET_BUS_IO_BTN_DOWN		_IOW(MOTOR_MAGIC, 1,int)
#define GET_BUS_IO_BTN_LEFT		_IOW(MOTOR_MAGIC, 2,int)
#define GET_BUS_IO_BTN_RIGHT	_IOW(MOTOR_MAGIC, 3,int)
#define GET_BUS_IO_BTN_ENTER	_IOW(MOTOR_MAGIC, 4,int)
#define GET_BUS_IO_BTN_CANCEL	_IOW(MOTOR_MAGIC, 5,int)

#define GET_BUS_IO_YX0			_IOW(MOTOR_MAGIC, 6,int)
#define GET_BUS_IO_YX1			_IOW(MOTOR_MAGIC, 7,int)
#define GET_BUS_IO_POWER_DOWN	_IOW(MOTOR_MAGIC, 8,int)
#define GET_BUS_IO_POWER_UP		_IOW(MOTOR_MAGIC, 9,int)

#define GET_BUS_IO_STATE0		_IOW(MOTOR_MAGIC, 10,int)
#define GET_BUS_IO_STATE1		_IOW(MOTOR_MAGIC, 11,int)
#define GET_BUS_IO_STATE2		_IOW(MOTOR_MAGIC, 12,int)
#define GET_BUS_IO_STATE3		_IOW(MOTOR_MAGIC, 13,int)
#define GET_BUS_IO_STATE4		_IOW(MOTOR_MAGIC, 14,int)
#define GET_BUS_IO_BTN_OPEN		_IOW(MOTOR_MAGIC, 15,int)
#define GET_BUS_IO_BTN_PROG		_IOW(MOTOR_MAGIC, 16,int)

#define M_BUS_IO_BTN_UP			0
#define M_BUS_IO_BTN_DOWN		1
#define M_BUS_IO_BTN_LEFT		2
#define M_BUS_IO_BTN_RIGHT		3
#define M_BUS_IO_BTN_ENTER		4
#define M_BUS_IO_BTN_CANCEL		5
#define M_BUS_IO_YX0			6
#define M_BUS_IO_YX1			7
#define M_BUS_IO_POWER_DOWN		8
#define M_BUS_IO_POWER_UP		9
#define M_BUS_IO_STATE0			10
#define M_BUS_IO_STATE1			11
#define M_BUS_IO_STATE2			12
#define M_BUS_IO_STATE3			13
#define M_BUS_IO_STATE4			14
#define M_BUS_IO_BTN_OPEN		15
#define M_BUS_IO_BTN_PROG		16


int result;
int main(int argc, char **argv) 
{ 
        int pin; 
        int cmd; 
        int fd; 
		
		unsigned long result;
        if ((sscanf(argv[1], "%d", &pin) != 1) || (argc != 2))
        {
           fprintf(stderr, "Usage: busior pin\n"); 
           exit(1); 
        }

        fd = open("/dev/busior", 0); 
        if (fd < 0) 
		{ 
           perror("open device busior error"); 
           exit(1); 
        } 
        //printf("pin=%d\n",pin); 
        
		//result = 100;

		switch (pin)
		{
			case M_BUS_IO_BTN_UP:
			ioctl(fd, GET_BUS_IO_BTN_UP,&result); 
			break;
			case M_BUS_IO_BTN_DOWN:
			ioctl(fd, GET_BUS_IO_BTN_DOWN,&result); 
			break;
			case M_BUS_IO_BTN_LEFT:
			ioctl(fd,GET_BUS_IO_BTN_LEFT,&result); 
			break;
			case M_BUS_IO_BTN_RIGHT:
			ioctl(fd,GET_BUS_IO_BTN_RIGHT,&result); 
			break;
			case M_BUS_IO_BTN_ENTER:
			ioctl(fd, GET_BUS_IO_BTN_ENTER,&result); 
			break;
			case M_BUS_IO_BTN_CANCEL:
			ioctl(fd, GET_BUS_IO_BTN_CANCEL,&result); 
			break;

			case M_BUS_IO_YX0:
			ioctl(fd,GET_BUS_IO_YX0 ,&result); 
			break;
			case M_BUS_IO_YX1:
			ioctl(fd,GET_BUS_IO_YX1 ,&result); 
			break;
			case M_BUS_IO_POWER_DOWN:
			ioctl(fd, GET_BUS_IO_POWER_DOWN,&result); 
			break;
			case M_BUS_IO_POWER_UP:
			ioctl(fd,GET_BUS_IO_POWER_UP ,&result); 
			break;

			case M_BUS_IO_STATE0:
			ioctl(fd, GET_BUS_IO_STATE0,&result); 
			break;
			case M_BUS_IO_STATE1:
			ioctl(fd,GET_BUS_IO_STATE1,&result); 
			break;
			case M_BUS_IO_STATE2:
			ioctl(fd,GET_BUS_IO_STATE2 ,&result); 
			break;
			case M_BUS_IO_STATE3:
			ioctl(fd,GET_BUS_IO_STATE3 ,&result); 
			break;
			case M_BUS_IO_STATE4:
			ioctl(fd, GET_BUS_IO_STATE4,&result); 
			break;
			case M_BUS_IO_BTN_OPEN:
			ioctl(fd,GET_BUS_IO_BTN_OPEN ,&result); 
			break;
			case M_BUS_IO_BTN_PROG:
			ioctl(fd, GET_BUS_IO_BTN_PROG,&result); 
			break;

			break;
			default:
			break;
		}  
		printf("result=%d\n",(unsigned int)result);
		//usleep(10000);
		     
        close(fd); 
        return 0;
} 
