#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
 
#define DEVICE_NAME "am1808-all-gpio" //Éè±¸Ãû(/dev/led) 
#define MOTOR_MAGIC 'g'
#define SET_GPIO_BASE			_IOW(MOTOR_MAGIC, 0,int)
#define SET_GPIO_AD_RST 		(SET_GPIO_BASE+7*16+13)  // GPIO7[13]
#define SET_GPIO_LCD_RST  		(SET_GPIO_BASE+7*16+15)	// GPIO7[15]
#define SET_GPIO_WD_FD  		(SET_GPIO_BASE+7*16+14)	// GPIO7[14]
#define SET_GPIO_GPRS_DTR  		(SET_GPIO_BASE+7*16+9)	// GPIO7[9]
#define SET_GPIO_IRDA_CTL  		(SET_GPIO_BASE+8*16+10)	// GPIO8[10]
#define SET_GPIO_BAT_CTL  		(SET_GPIO_BASE+8*16+8)	// GPIO8[8]
#define SET_GPIO_LCD_BL  		(SET_GPIO_BASE+7*16+11)	// GPIO7[11]

#define GET_GPIO_BASE			_IOW(MOTOR_MAGIC, 16*16,int)
#define GET_GPIO_BTN_INT  		(GET_GPIO_BASE + 7*16+12)	// GPIO7[12]
#define GET_GPIO_AD_INT  		(GET_GPIO_BASE + 8*16+11)	// GPIO8[11]	
#define GET_GPIO_GPRS_DSR  		(GET_GPIO_BASE + 7*16+10)	// GPIO7[10]
#define GET_GPIO_GPRS_DCD  		(GET_GPIO_BASE + 7*16+8)	// GPIO7[8]
#define GET_GPIO_GPRS_RI  		(GET_GPIO_BASE + 8*16+9)	// GPIO8[9]



int main(int argc, char **argv) 
{ 
        int pin; 
        int cmd; 
        int fd; 
		unsigned long result;
        
        fd = open("/dev/am1808-all-gpio", 0); 
        if (fd < 0) 
		{ 
           perror("open device am1808-all-gpio error"); 
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
		while(1)
		{
			ioctl(fd, SET_GPIO_GPRS_DTR ,1); 
			usleep(300000);
			ioctl(fd, SET_GPIO_GPRS_DTR ,0); 
			usleep(300000);
		}
        close(fd); 
        return 0;
} 
