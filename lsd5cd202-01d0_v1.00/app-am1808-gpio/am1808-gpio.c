#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 
#define DEVICE_NAME "am1808-gpio" //Éè±¸Ãû(/dev/led) 
#define MOTOR_MAGIC 'g'
#define SET_GPIO_AD_RST 		_IOW(MOTOR_MAGIC, 2,int)    // GPIO7[13]
#define SET_GPIO_LCD_RST  		_IOW(MOTOR_MAGIC, 3,int)	// GPIO7[15]
#define SET_GPIO_WD_FD  		_IOW(MOTOR_MAGIC, 4,int)	// GPIO7[14]
#define SET_GPIO_GPRS_DTR  		_IOW(MOTOR_MAGIC, 6,int)	// GPIO7[9]
#define SET_GPIO_IRDA_CTL  		_IOW(MOTOR_MAGIC, 9,int)	// GPIO8[10]
#define SET_GPIO_BAT_CTL  		_IOW(MOTOR_MAGIC, 10,int)	// GPIO8[8]
#define SET_GPIO_LCD_BL  		_IOW(MOTOR_MAGIC, 11,int)	// GPIO7[11]

#define GET_GPIO_BTN_INT  		_IOW(MOTOR_MAGIC, 12,int)	// GPIO7[12]
#define GET_GPIO_AD_INT  		_IOW(MOTOR_MAGIC, 13,int)	// GPIO8[11]	
#define GET_GPIO_GPRS_DSR  		_IOW(MOTOR_MAGIC, 5,int)	// GPIO7[10]
#define GET_GPIO_GPRS_DCD  		_IOW(MOTOR_MAGIC, 7,int)	// GPIO7[8]
#define GET_GPIO_GPRS_RI  		_IOW(MOTOR_MAGIC, 8,int)	// GPIO8[9]

#define M_GPIO_AD_RST 	0
#define M_GPIO_LCD_RST 	1
#define M_GPIO_WD_FD 	2
#define M_GPIO_GPRS_DSR 3
#define M_GPIO_GPRS_DTR 4
#define M_GPIO_GPRS_DCD 5
#define M_GPIO_GPRS_RI 	6
#define M_GPIO_IRDA_CTL 7
#define M_GPIO_BAT_CTL	8
#define M_GPIO_LCD_BL  	9
#define M_GPIO_BTN_INT  10
#define M_GPIO_AD_INT 	11

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

        fd = open("/dev/am1808-gpio", 0); 
        if (fd < 0) 
		{ 
           perror("open device am1808-gpio error"); 
           exit(1); 
        } 
        printf("cmd=%d,pin=%d\n",cmd,pin); 
        
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
        close(fd); 
        return 0;
} 
