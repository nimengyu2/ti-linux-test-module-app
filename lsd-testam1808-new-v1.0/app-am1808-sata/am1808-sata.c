#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 


#define M_GPIO_OUTPUT  	0
#define M_GPIO_INPUT	1
#define M_GPIO_OUTPUT_HIGH  	1
#define M_GPIO_OUTPUT_LOW		0

#define DEVICE_NAME "am1808-sata" //设备名(/dev/led) 
#define MOTOR_MAGIC 's'
#define AM1808_GPIO_CMD_BASE		_IOW(MOTOR_MAGIC, 0,int)
#define SET_GPIO_DIRECT                 (AM1808_GPIO_CMD_BASE + 0)     // cmd是这个值  arg的值  M_GPIO_INPUT=输入  M_GPIO_OUTPUT=输出 


int main(int argc, char **argv) 
{ 
        int pin; 
        int cmd; 
        int fd; 
	unsigned long result;
#if 0
        if ((sscanf(argv[2], "%d", &cmd) != 1) || (sscanf(argv[1],"%d", &pin) != 1) || (argc != 3))
        {
           fprintf(stderr, "Usage: am1808-gpio pin cmd\n"); 
           exit(1); 
        }
#endif

        fd = open("/dev/am1808-sata", 0);
        if (fd < 0) 
		{ 
           perror("open device am1808-sata error"); 
           exit(1); 
        } 
        //printf("cmd=%d,pin=%d\n",cmd,pin); 
        
#if 0
	if(cmd == 2)
        {
			switch (pin)
			{
				case M_GPIO_BTN_INT:
				ioctl(fd, GET_GPIO_BTN_INT,&result); 
				printf("btn_int gpio is %d\n",(int)result);
				break;
				case M_GPIO_AD_INT:
				ioctl(fd, GET_GPIO_AD_INT,&result); 
				printf("ad_int gpio is %d\n",(int)result);
				break;
				case M_GPIO_GPRS_DSR:
				ioctl(fd, GET_GPIO_GPRS_DSR,&result); 
				printf("GPRS_DSR gpio is %d\n",(int)result);
				break;
				case M_GPIO_GPRS_DCD:
				ioctl(fd, GET_GPIO_GPRS_DCD,&result); 
				printf("GPRS_DCD gpio is %d\n",(int)result);
				break;
				case M_GPIO_GPRS_RI:
				ioctl(fd, GET_GPIO_GPRS_RI,&result); 
				printf("GPRS_RI gpio is %d\n",(int)result);
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
				case M_GPIO_AD_RST:
				ioctl(fd, SET_GPIO_AD_RST,cmd); 
				break;
				case M_GPIO_LCD_RST:
				ioctl(fd, SET_GPIO_LCD_RST,cmd); 
				break;
				case M_GPIO_WD_FD :
				ioctl(fd, SET_GPIO_WD_FD,cmd); 
				break;
				case M_GPIO_GPRS_DTR:
				ioctl(fd, SET_GPIO_GPRS_DTR,cmd); 
				break;
				case M_GPIO_IRDA_CTL:
				ioctl(fd, SET_GPIO_IRDA_CTL,cmd); 
				break;
				case M_GPIO_BAT_CTL:
				ioctl(fd, SET_GPIO_BAT_CTL,cmd); 
				break;
				case M_GPIO_LCD_BL:
				ioctl(fd, SET_GPIO_LCD_BL,cmd); 
				break;
				default:
				printf("error cmd\n");
				break;
			}           
        }
#endif
	
	ioctl(fd, SET_GPIO_DIRECT,M_GPIO_OUTPUT); 
        close(fd); 
        return 0;
} 
