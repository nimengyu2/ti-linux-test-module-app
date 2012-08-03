#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 
#define DEVICE_NAME "busiow" //Éè±¸Ãû(/dev/lcd) 

#define MOTOR_MAGIC 'w'
#define SET_BUS_IO_W_NPLC_RST	_IOW(MOTOR_MAGIC, 0,int)
#define SET_BUS_IO_W_PLC_SET	_IOW(MOTOR_MAGIC, 1,int)
#define SET_BUS_IO_W_NM_PCTRL	_IOW(MOTOR_MAGIC, 2,int)
#define SET_BUS_IO_W_M_IGT		_IOW(MOTOR_MAGIC, 3,int)
#define SET_BUS_IO_W_NM_RST		_IOW(MOTOR_MAGIC, 4,int)
#define SET_BUS_IO_W_ALARM_LED	_IOW(MOTOR_MAGIC, 5,int)
#define SET_BUS_IO_W_BATC		_IOW(MOTOR_MAGIC, 6,int)
#define SET_BUS_IO_W_ESAM_RESET	_IOW(MOTOR_MAGIC, 7,int)

#define M_BUS_IO_W_NPLC_RST 	0
#define M_BUS_IO_W_PLC_SET 		1
#define M_BUS_IO_W_NM_PCTRL 	2
#define M_BUS_IO_W_M_IGT 		3
#define M_BUS_IO_W_NM_RST 		4
#define M_BUS_IO_W_ALARM_LED 	5
#define M_BUS_IO_W_BATC 		6
#define M_BUS_IO_W_ESAM_RESET 	7


int main(int argc, char **argv) 
{ 
        int pin; 
        int cmd; 
        int fd; 
		unsigned long result;
      
        fd = open("/dev/busiow", 0); 
        if (fd < 0) 
		{ 
           perror("open device busiow error"); 
           exit(1); 
        } 
        while(1)
		{
			ioctl(fd, SET_BUS_IO_W_ALARM_LED,0);
			usleep(10);
			ioctl(fd, SET_BUS_IO_W_ALARM_LED,1); 
		}
				
				
        close(fd); 
        return 0;
} 
