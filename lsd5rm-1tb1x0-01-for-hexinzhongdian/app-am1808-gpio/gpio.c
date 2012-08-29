#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include <stdio.h>

 
#define DEVICE_NAME "gpio" //Éè±¸Ãû(/dev/led) 

#define MOTOR_MAGIC 'L'
#define SET_LED _IOW(MOTOR_MAGIC, 0,int)

int main(int argc, char **argv) 
{ 
        int pin; 
        int cmd; 
        int fd; 
	unsigned long result;
	if(argc != 3)
	{
		printf("ERROR:argc != 2\n");		
		fprintf(stderr, "Usage: gpio pin level\n"); 
		fprintf(stderr, "Usage: pin 1 ~ 14\n"); 
		fprintf(stderr, "Usage: level=1 high level level=0 low level\n"); 
           	exit(1); 
	}
        if ((sscanf(argv[2], "%d", &cmd) != 1) || (sscanf(argv[1],"%d", &pin) != 1))
        {
		printf("ERROR:get cmd pin \n");		
		fprintf(stderr, "Usage: gpio pin level\n"); 
		fprintf(stderr, "Usage: pin 1 ~ 14\n"); 
		fprintf(stderr, "Usage: level=1 high level level=0 low level\n"); 
           	exit(1);  
        }

        fd = open("/dev/gpio", 0); 
        if (fd < 0) 
	{ 
           perror("open device gpio error"); 
           exit(1); 
        } 
        printf("pin=%d,level=%d\n",pin,cmd); 
        
	if((pin >= 1) && (pin <=14))
	{
		if((cmd == 1) || (cmd == 0))
		{
			ioctl(fd, SET_LED + pin -1,cmd); 
		}
		else
		{
			printf("ERROR:cmd != 1 0, cmd=%d\n",cmd);		
			fprintf(stderr, "Usage: gpio pin level\n"); 
			fprintf(stderr, "Usage: pin 1 ~ 14\n"); 
			fprintf(stderr, "Usage: level=1 high level level=0 low level\n"); 
		   	exit(1); 
		}		
		
	}
	else
	{	
		printf("ERROR:pin exit,pin=%d\n",pin);
		fprintf(stderr, "Usage: gpio pin level\n"); 
		fprintf(stderr, "Usage: pin 1 ~ 14\n"); 
		fprintf(stderr, "Usage: level=1 high level level=0 low level\n"); 
           	exit(1); 
	}
        close(fd); 
        return 0;
} 
