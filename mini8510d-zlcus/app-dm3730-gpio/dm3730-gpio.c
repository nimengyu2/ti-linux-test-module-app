#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 

#define DEVICE_NAME "dm3730-gpio" //Éè±¸Ãû(/dev/led) 

#define MOTOR_MAGIC 'g'
#define SET_GPIO42_DIR                  _IOW(MOTOR_MAGIC, 2,int)
#define SET_GPIO42_VALUE		_IOW(MOTOR_MAGIC, 3,int) 
#define GET_GPIO42_VALUE		_IOW(MOTOR_MAGIC, 4,int) 


int main(int argc, char **argv) 
{ 
        int val; 
        int cmd; 
        int fd; 
	unsigned long result;
        if ((sscanf(argv[2], "%d", &val) != 1) || (sscanf(argv[1],"%d", &cmd) != 1) || (argc != 3))
        {
           fprintf(stderr, "Usage: dm3730-gpio cmd val\n"); 
           exit(1); 
        }

        fd = open("/dev/dm3730-gpio", 0); 
        if (fd < 0) 
		{ 
           perror("open device dm3730-gpio error"); 
           exit(1); 
        } 
        printf("cmd=%d,val=%d\n",cmd,val); 
        
	switch (cmd)
   	{	
		case 0:  
		if(val == 0)
		{
			ioctl(fd, SET_GPIO42_DIR, 0); 
			printf("set gpio output dir\n");
		}
		else
		{
			ioctl(fd, SET_GPIO42_DIR, 1); 
			printf("set gpio input dir\n");
		}
		break;
		case 1:  
		if(val == 0)
		{
			ioctl(fd, SET_GPIO42_VALUE, 0); 
			printf("set gpio output low\n");
		}
		else
		{
			ioctl(fd, SET_GPIO42_VALUE, 1); 
			printf("set gpio input high\n");
		} 
		break;
		case 2:   
		ioctl(fd, GET_GPIO42_VALUE,&result); 
		printf("get gpio value is %d\n",(int)result);
		break;	
		default:
		printf("cmd error\n");
		break;
	}	
        close(fd); 
        return 0;
} 
