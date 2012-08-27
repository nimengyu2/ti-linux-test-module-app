#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h>
#include "am1808_gpio.h" 

int main(int argc, char **argv) 
{ 
        int pin; 
        int cmd; 
        int fd; 
	unsigned long result;
        if ((sscanf(argv[2], "%d", &cmd) != 1) || (sscanf(argv[1],"%d", &pin) != 1) || (argc != 3))
        {
           fprintf(stderr, "Usage: gpio-test pin cmd\n"); 
	   fprintf(stderr, "Usage: cmd 0 1 set gpio value\n"); 
	   fprintf(stderr, "Usage: cmd 2 get gpio value\n"); 
           exit(1); 
        }

	// init gpio
        fn_am1808_gpio_init();
        printf("pin=%d,cmd=%d\n",pin,cmd); 
        
	if(cmd == 2)
        {
		switch (pin)
		{
			case YX_B:
			case YX_A:
			result = fn_get_am1808_gpio_value(pin); 
			printf("get pin %d GPIO%d[%d] value is %d\n",pin,pin/16,pin%16,(int)result);
			break;
			default:
			printf("ERROR:cmd=2,pin exit\n");
			break;
		}            	
        }
        else if (cmd <= 1)
        {
		switch (pin)
		{
			case ZB_SET:
			case ZB_RST:
			//case U1_RTS:
			//case U1_CTS:
			case STATE4:
			case STATE3:
			case STATE2:
			case STATE1:
			case STATE0:
			case U1_DCD:
			case U1_DTR:	
			case U1_RI:
			case GPRS_IGT:
			case GPRS_RST:
			case GPRS_PCTRL:
			case WDI:
			fn_set_am1808_gpio_value(pin,cmd);
			printf("set pin %d GPIO%d[%d] value=%d\n",pin,pin/16,pin%16,cmd);
			break;
			default:
			printf("ERROR:cmd=%d,pin exit\n",cmd);
			break;
		}           
        }
	// exit 
        fn_am1808_gpio_exit();
        return 0;
} 
