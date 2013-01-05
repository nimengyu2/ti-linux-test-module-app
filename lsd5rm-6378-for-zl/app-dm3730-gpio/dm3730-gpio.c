#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 
#define DEVICE_NAME "dm3730-gpio" //Éè±¸Ãû(/dev/led) 
#define MOTOR_MAGIC 'g'
#define GET_KEY10			_IOW(MOTOR_MAGIC, 1,int) 
#define GET_KEY9			_IOW(MOTOR_MAGIC, 2,int) 
#define GET_KEY8			_IOW(MOTOR_MAGIC, 3,int) 
#define GET_KEY7			_IOW(MOTOR_MAGIC, 4,int) 
#define GET_KEY6			_IOW(MOTOR_MAGIC, 5,int) 
#define GET_KEY5			_IOW(MOTOR_MAGIC, 6,int) 
#define GET_KEY4			_IOW(MOTOR_MAGIC, 7,int) 
#define GET_KEY3			_IOW(MOTOR_MAGIC, 8,int) 
#define GET_KEY2			_IOW(MOTOR_MAGIC, 9,int) 
#define GET_KEY1			_IOW(MOTOR_MAGIC, 10,int) 
#define GET_1V8_LED_INT			_IOW(MOTOR_MAGIC, 11,int) 
#define SET_BL_EN			_IOW(MOTOR_MAGIC, 12,int) 
#define SET_LED_EN			_IOW(MOTOR_MAGIC, 13,int) 
#define SET_BUZ_EN			_IOW(MOTOR_MAGIC, 14,int) 
#define SET_LED1			_IOW(MOTOR_MAGIC, 15,int) 
#define SET_LED2			_IOW(MOTOR_MAGIC, 16,int) 
#define SET_RELAY1			_IOW(MOTOR_MAGIC, 17,int) 
#define SET_RELAY2			_IOW(MOTOR_MAGIC, 18,int) 
#define SET_RELAY3			_IOW(MOTOR_MAGIC, 19,int) 
#define SET_RELAY4			_IOW(MOTOR_MAGIC, 20,int) 
#define GET_IOIN3			_IOW(MOTOR_MAGIC, 21,int) 
#define GET_IOIN2			_IOW(MOTOR_MAGIC, 22,int) 
#define GET_IOIN1			_IOW(MOTOR_MAGIC, 23,int) 

int main(int argc, char **argv) 
{ 
        int pin; 
        int cmd; 
        int fd; 
	unsigned long result;
        if ((sscanf(argv[2], "%d", &cmd) != 1) || (sscanf(argv[1],"%d", &pin) != 1) || (argc != 3))
        {
           fprintf(stderr, "Usage: am1808-gpio pin cmd\n"); 
           exit(1); 
        }

        fd = open("/dev/dm3730-gpio", 0); 
        if (fd < 0) 
		{ 
           perror("open device dm3730-gpio error"); 
           exit(1); 
        } 
        printf("pin=%d,cmd=%d\n",pin,cmd); 
        
	if(cmd == 2)
        {
		switch (pin)
		{
			case 1:
			ioctl(fd, GET_KEY10,&result); 
			printf("GET_KEY10 is %d\n",(int)result);
			break;
			case 2:
			ioctl(fd, GET_KEY9,&result); 
			printf("GET_KEY9 is %d\n",(int)result);
			break;
			case 3:
			ioctl(fd, GET_KEY8,&result); 
			printf("GET_KEY8 is %d\n",(int)result);
			break;
			case 4:
			ioctl(fd, GET_KEY7,&result); 
			printf("GET_KEY7 is %d\n",(int)result);
			break;
			case 5:
			ioctl(fd, GET_KEY6,&result); 
			printf("GET_KEY6 is %d\n",(int)result);
			break;
			case 6:
			ioctl(fd, GET_KEY5,&result); 
			printf("GET_KEY5 is %d\n",(int)result);
			break;
			case 7:
			ioctl(fd, GET_KEY4,&result); 
			printf("GET_KEY4 is %d\n",(int)result);
			break;
			case 8:
			ioctl(fd, GET_KEY3,&result); 
			printf("GET_KEY3 is %d\n",(int)result);
			break;
			case 9:
			ioctl(fd, GET_KEY2,&result); 
			printf("GET_KEY2 is %d\n",(int)result);
			break;
			case 10:
			ioctl(fd, GET_KEY1,&result); 
			printf("GET_KEY1 is %d\n",(int)result);
			break;
			case 11:
			ioctl(fd, GET_1V8_LED_INT,&result); 
			printf("GET_1V8_LED_INT is %d\n",(int)result);
			break;
			case 21:
			ioctl(fd, GET_IOIN3,&result); 
			printf("GET_IOIN3 is %d\n",(int)result);
			break;
			case 22:
			ioctl(fd, GET_IOIN2,&result); 
			printf("GET_IOIN2 is %d\n",(int)result);
			break;
			case 23:
			ioctl(fd, GET_IOIN1,&result); 
			printf("GET_IOIN1 is %d\n",(int)result);
			break;
			default:
			printf("error cmd\n");
			break;
		}            
			
        }
        else if (cmd <= 1)
        {
		switch (pin)
		{
			case 12:
			ioctl(fd, SET_BL_EN,cmd); 
			printf("SET_BL_EN pin level %d\n",cmd);
			break;
			case 13:
			ioctl(fd, SET_LED_EN,cmd); 
			printf("SET_LED_EN pin level %d\n",cmd);
			break;
			case 14 :
			ioctl(fd, SET_BUZ_EN,cmd); 
			printf("SET_BUZ_EN pin level %d\n",cmd);
			break;
			case 15:
			ioctl(fd, SET_LED1,cmd); 
			printf("SET_LED1 pin level %d\n",cmd);
			break;
			case 16:
			ioctl(fd, SET_LED2,cmd); 
			printf("SET_LED2 pin level %d\n",cmd);
			break;
			case 17:
			ioctl(fd, SET_RELAY1,cmd); 
			printf("SET_RELAY1 pin level %d\n",cmd);
			break;
			case 18:
			ioctl(fd, SET_RELAY2,cmd); 
			printf("SET_RELAY2 pin level %d\n",cmd);
			break;
			case 19:
			ioctl(fd, SET_RELAY3,cmd); 
			printf("SET_RELAY3 pin level %d\n",cmd);
			break;
			case 20:
			ioctl(fd, SET_RELAY4,cmd); 
			printf("SET_RELAY4 pin level %d\n",cmd);
			break;
			default:
			printf("error cmd\n");
			break;
		}           
        }
        close(fd); 
        return 0;
} 
