#ifndef _AM1808_GPIO_H_
#define _AM1808_GPIO_H_

#define M_GPIO_OUTPUT  	0
#define M_GPIO_INPUT	1
#define M_GPIO_OUTPUT_HIGH  	1
#define M_GPIO_OUTPUT_LOW		0

//#define DEVICE_NAME "/dev/am1808-all-gpio" //设备名(/dev/led) 
#define MOTOR_MAGIC 'g'
#define SET_GPIO_DIRECT                 (_IOW(MOTOR_MAGIC, 0,int))     // cmd是这个值  arg的值 1=输入  0=输出 
#define SET_GPIO_VALUE_BASE 		(_IOW(MOTOR_MAGIC, 0,int)+200)   // cmd是这个值  arg的值 1=高电平  0=高电平 
#define GET_GPIO_VALUE_BASE 		(_IOW(MOTOR_MAGIC, 0,int)+400)   // cmd是这个值  arg的值表示返回值的指针

#define KEY_UP  	(4*16+1)
#define KEY_DOWN  	(4*16+0)
#define KEY_LEFT  	(4*16+2)
#define KEY_RIGHT  	(4*16+6)
#define KEY_OK  	(4*16+7)
#define KEY_CANCEL  	(4*16+3)

#define LCD_RST       (0*16+12)    // 12
#define LCD_BL        (0*16+14)    // 14

#define ZB_SET  	(6*16+6)   // 102
#define ZB_RST  	(6*16+7)   // 103
//#define U1_RTS  	(0*16+11)  // 11
//#define U1_CTS  	(0*16+10)  // 10
#define STATE4   	(5*16+10)  // 90
#define STATE3   	(5*16+9)   // 89
#define STATE2   	(5*16+15)  // 95
#define STATE1   	(5*16+13)  // 93
#define STATE0   	(5*16+4)   // 84
#define U1_DCD   	(5*16+7)   // 87
#define U1_DTR   	(5*16+8)   // 88
#define U1_RI    	(5*16+11)  // 91
#define GPRS_IGT 	(5*16+14)  // 94
#define GPRS_RST 	(4*16+5)   // 69
#define GPRS_PCTRL 	(4*16+4)   // 68
#define YX_B  		(2*16+14)  // 46
#define YX_A  		(2*16+13)  // 45
#define WDI  		(6*16+4)   // 100
#define CHG_STATE 	0
#define CHG_EN 		0
#define RTC_INT         0
#define V12V_FAIL        0


#define ERR_LED        9

extern int fn_am1808_gpio_init(void);
extern int fn_get_am1808_gpio_value(int gpio_num);
extern int fn_set_am1808_gpio_value(int gpio_num,int value);
extern int fn_set_am1808_gpio_direct(int gpio_num,int direct);
extern int fn_am1808_gpio_exit(void);

#endif
