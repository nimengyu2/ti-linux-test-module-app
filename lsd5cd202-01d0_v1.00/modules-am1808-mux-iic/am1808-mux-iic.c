#include <linux/miscdevice.h> 
#include <linux/delay.h> 
#include <asm/irq.h> 
#include <mach/hardware.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/mm.h> 
#include <linux/fs.h> 
#include <linux/types.h> 
#include <linux/delay.h> 
#include <linux/moduleparam.h> 
#include <linux/slab.h> 
#include <linux/errno.h> 
#include <linux/ioctl.h> 
#include <linux/cdev.h> 
#include <linux/string.h> 
#include <linux/list.h> 
#include <linux/pci.h> 
#include <linux/gpio.h> 
#include <asm/uaccess.h> 
#include <asm/atomic.h> 
#include <asm/unistd.h>
#include <linux/version.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/completion.h>
#include <mach/cputype.h>
#include <mach/hardware.h>
#include <mach/mux.h>
#include <mach/gpio.h>
#include <asm/gpio.h>
#include <linux/delay.h>
#include <mach/da8xx.h>
#include <linux/clk.h>
#include "cslr_syscfg01_am1808.h"
//#include "am1808-all-gpio.h"

#define DEVICE_NAME "am1808-mux-iic" //设备名(/dev/led) 
#if 0
#define MOTOR_MAGIC 'i'
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
#endif

#define M_GPIO_OUTPUT  	0
#define M_GPIO_INPUT	1
#define M_GPIO_OUTPUT_HIGH  	1
#define M_GPIO_OUTPUT_LOW		0	
struct st_am1808_gpio
{
	int gpio_num;  // gpio端口号
	char* gpio_all_function; // 该gpio所有功能
	volatile void __iomem *syscfg0_pinmux; // 该gpio使用的pinmux寄存器号
	unsigned long syscfg0_pinmux_mask; // mask
	unsigned long syscfg0_pinmux_shift; // 偏移
	unsigned long syscfg0_pinmux_function; // gpio功能
	int gpio_direction;  // 该gpio方向，输入输出
	int gpio_output_state; // 输出状态
}st_am1808_gpio;

struct st_am1808_gpio am1808_all_gpio[] = 
{
	// am1808 南方电力核心板子 通信接口使用 start
	{
		1*16+5,     
		"SPI1_SCSn_7 / I2C0_SCL / Timer64P2_OUT12 / GPIO1[5]",  
		SYSCFG0_PINMUX4, 
		CSL_SYSCFG_PINMUX4_PINMUX4_11_8_MASK,
		CSL_SYSCFG_PINMUX4_PINMUX4_11_8_SHIFT,
		CSL_SYSCFG_PINMUX4_PINMUX4_11_8_I2C0_SCL,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*16+4,     
		"SPI1_SCSn_6 / I2C0_SDA / Timer64P3_OUT12 / GPIO1[4]",  
		SYSCFG0_PINMUX4, 
		CSL_SYSCFG_PINMUX4_PINMUX4_15_12_MASK,
		CSL_SYSCFG_PINMUX4_PINMUX4_15_12_SHIFT,
		CSL_SYSCFG_PINMUX4_PINMUX4_15_12_I2C0_SDA,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// am1808 南方电力核心板子 通信接口使用 start
};


int fn_am1808_all_gpio_init(void)
{
	int gpio_cnt;	
	int i;
	unsigned long TmpRegVal;
	int status;

	gpio_cnt = sizeof(am1808_all_gpio)/sizeof(struct st_am1808_gpio);	
	printk("am1808_mux_setting count = %d\n",gpio_cnt);
	
	for(i = 0; i < gpio_cnt; i++)
	{
		TmpRegVal  = __raw_readl(am1808_all_gpio[i].syscfg0_pinmux);
		TmpRegVal &= ~(am1808_all_gpio[i].syscfg0_pinmux_mask);
		TmpRegVal |=  (am1808_all_gpio[i].syscfg0_pinmux_function << am1808_all_gpio[i].syscfg0_pinmux_shift);
		__raw_writel(TmpRegVal,am1808_all_gpio[i].syscfg0_pinmux);
		  
		
		printk("am1808_mux_setting:configure %s as iic output\n",
				am1808_all_gpio[i].gpio_all_function); 
	}
}

int fn_am1808_all_gpio_exit(void)
{
#if 0
	int gpio_cnt;	
	int i;

	gpio_cnt = sizeof(am1808_all_gpio)/sizeof(struct st_am1808_gpio);	
	printk("am1808_all_gpio count = %d\n",gpio_cnt);
	
	for(i = 0; i < gpio_cnt; i++)
	{
		gpio_free(am1808_all_gpio[i].gpio_num);
	}
#endif
}


#if 0
// ioctl 函数的实现 
// 在应用用户层将通过 ioctl 函数向内核传递参数，以控制 LED的输出状态 
static int am1808_gpio_ioctl( 
 struct inode *inode,  
 struct file *file,   
 unsigned int cmd,  
 unsigned long arg) 
{ 
	#if 1
	printk("am1808-gpio:cmd interger is %d,hex is 0x%08x\n",(int)cmd,(unsigned int)cmd);
	printk("am1808-gpio:arg interger is %d,hex is 0x%08x\n",(int)arg,(unsigned int)arg);
	#endif
   	switch (cmd)
   	{	
		case SET_GPIO_AD_RST:   // GPIO7[13]
		if(arg == 0)
		{
			gpio_set_value(7*16+13,0); 
		}
		else
		{
			gpio_set_value(7*16+13,1); 
		}
		break;	

		case GET_GPIO_GPRS_DCD:  // GPIO7[8]
 		if(arg != 0)
		{
			if(gpio_get_value(7*16+8) == 0)			
				*((unsigned long*)arg) = 0;
			else
				*((unsigned long*)arg) = 1;
		}
		break;

		default:
		printk("am1808-gpio:ioctl cmd error\n");
		return -EINVAL; 
   	} 
   	return 0;       
} 
#endif 
 
//  设备函数操作集，在此只有 ioctl函数，通常还有 read, write, open, close 等，因为本 LED驱动在下面已经
//  注册为 misc 设备，因此也可以不用 open/close  
static struct file_operations dev_fops = { 
 .owner = THIS_MODULE, 
 //.ioctl = am1808_gpio_ioctl, 
}; 
  
//  把 LED驱动注册为 MISC 设备 
static struct miscdevice misc = { 
  //动态设备号
  .minor = MISC_DYNAMIC_MINOR,  
  .name = DEVICE_NAME, 
  .fops = &dev_fops, 
}; 
 
 
// 设备初始化 
static int __init dev_init(void) 
{
	int ret;
	fn_am1808_all_gpio_init();

   	ret = misc_register(&misc); //注册设备 
   	printk (DEVICE_NAME"\tinitialized\n"); //打印初始化信息 
   	return ret; 
} 
 
static void __exit dev_exit(void) 
{ 
	fn_am1808_all_gpio_exit();
   	misc_deregister(&misc); 
} 
 
// 模块初始化，仅当使用 insmod/podprobe 命令加载时有用，
// 如果设备不是通过模块方式加载，此处将不会被调用 
module_init(dev_init); 

// 卸载模块，当该设备通过模块方式加载后，
// 可以通过 rmmod 命令卸载，将调用此函数 
module_exit(dev_exit);

// 版权信息 
MODULE_LICENSE("GPL"); 
// 开发者信息 
MODULE_AUTHOR("Lierda EA nmy"); 
