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

#define DEVICE_NAME "am1808-gpio" //设备名(/dev/led) 

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
	
		case SET_GPIO_LCD_RST:  // GPIO7[15]
		if(arg == 0)
		{
			gpio_set_value(7*16+15,0); 
		}
		else
		{
			gpio_set_value(7*16+15,1); 
		}
		break;

		case SET_GPIO_WD_FD:   // GPIO7[14]
		if(arg == 0)
		{
			gpio_set_value(7*16+14,0); 
		}
		else
		{
			gpio_set_value(7*16+14,1); 
		}
		break;

		case SET_GPIO_GPRS_DTR: // GPIO7[9]
		if(arg == 0)
		{
			gpio_set_value(7*16+9,0); 
		}
		else
		{
			gpio_set_value(7*16+9,1); 
		}
		break;

		case SET_GPIO_IRDA_CTL: // GPIO8[10]
		if(arg == 0)
		{
			gpio_set_value(8*16+10,0); 
		}
		else
		{
			gpio_set_value(8*16+10,1); 
		}
		break;

		case SET_GPIO_BAT_CTL: // GPIO8[8]
		if(arg == 0)
		{
			gpio_set_value(8*16+8,0); 
		}
		else
		{
			gpio_set_value(8*16+8,1); 
		}
		break;

		case SET_GPIO_LCD_BL: // GPIO7[11]
		if(arg == 0)
		{
			gpio_set_value(7*16+11,0); 
		}
		else
		{
			gpio_set_value(7*16+11,1); 
		}
		break;

		case GET_GPIO_BTN_INT:  // GPIO7[12]
 		if(arg != 0)
		{
			if(gpio_get_value(7*16+12) == 0)			
				*((unsigned long*)arg) = 0;
			else
				*((unsigned long*)arg) = 1;
		}
		break;

		case GET_GPIO_AD_INT:  // GPIO8[11]
 		if(arg != 0)
		{
			if(gpio_get_value(8*16+11) == 0)			
				*((unsigned long*)arg) = 0;
			else
				*((unsigned long*)arg) = 1;
		}
		break;

		case GET_GPIO_GPRS_DSR:  // GPIO7[10]
 		if(arg != 0)
		{
			if(gpio_get_value(7*16+10) == 0)			
				*((unsigned long*)arg) = 0;
			else
				*((unsigned long*)arg) = 1;
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

		case GET_GPIO_GPRS_RI:  // GPIO8[9]
 		if(arg != 0)
		{
			if(gpio_get_value(8*16+9) == 0)			
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
 
 
//  设备函数操作集，在此只有 ioctl函数，通常还有 read, write, open, close 等，因为本 LED驱动在下面已经
//  注册为 misc 设备，因此也可以不用 open/close  
static struct file_operations dev_fops = { 
 .owner = THIS_MODULE, 
 .ioctl = am1808_gpio_ioctl, 
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
	unsigned long TmpRegVal;
	int status;
	int ret;

	// SET_GPIO_AD_RST 	 GPIO7[13]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX16);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX16_PINMUX16_19_16_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX16_PINMUX16_19_16_GPIO7_13 << CSL_SYSCFG_PINMUX16_PINMUX16_19_16_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX16);
	printk("am1808-gpio:configure GPIO7_13 as gpio\n");   
	status = gpio_request(7*16+13, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 7*16+13);
		return status;
	}
	gpio_direction_output(7*16+13,1);	

	// SET_GPIO_LCD_RST 	 GPIO7[15]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX16);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX16_PINMUX16_11_8_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX16_PINMUX16_11_8_GPIO7_15 << CSL_SYSCFG_PINMUX16_PINMUX16_11_8_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX16);
	printk("am1808-gpio:configure GPIO7_15 as gpio\n");   
	status = gpio_request(7*16+15, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 7*16+15);
		return status;
	}
	gpio_direction_output(7*16+15,1);

	// SET_GPIO_WD_FD 	 GPIO7[14]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX16);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX16_PINMUX16_15_12_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX16_PINMUX16_15_12_GPIO7_14 << CSL_SYSCFG_PINMUX16_PINMUX16_15_12_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX16);
	printk("am1808-gpio:configure GPIO7_14 as gpio\n");  
	status = gpio_request(7*16+14, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 7*16+14);
		return status;
	}
	gpio_direction_output(7*16+14,1);

	// GET_GPIO_GPRS_DSR 	 GPIO7[10]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX16);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX16_PINMUX16_31_28_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX16_PINMUX16_31_28_GPIO7_10 << CSL_SYSCFG_PINMUX16_PINMUX16_31_28_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX16);
	printk("am1808-gpio:configure GPIO7_10 as gpio\n");   
	status = gpio_request(7*16+10, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 7*16+10);
		return status;
	}
	gpio_direction_input(7*16+10);

	// SET_GPIO_GPRS_DTR 	 GPIO7[9]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX17);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX17_PINMUX17_3_0_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX17_PINMUX17_3_0_GPIO7_9 << CSL_SYSCFG_PINMUX17_PINMUX17_3_0_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX17);
	printk("am1808-gpio:configure GPIO7_9 as gpio\n");   
	status = gpio_request(7*16+9, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 7*16+9);
		return status;
	}
	gpio_direction_output(7*16+9,1);

	// GET_GPIO_GPRS_DCD 	 GPIO7[8]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX17);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX17_PINMUX17_7_4_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX17_PINMUX17_7_4_GPIO7_8 << CSL_SYSCFG_PINMUX17_PINMUX17_7_4_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX17);
	printk("am1808-gpio:configure GPIO7_8 as gpio\n");   
	status = gpio_request(7*16+8, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 7*16+8);
		return status;
	}
	gpio_direction_input(7*16+8);

	// GET_GPIO_GPRS_RI 	 GPIO8[9]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX19);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX19_PINMUX19_3_0_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX19_PINMUX19_3_0_GPIO8_9 << CSL_SYSCFG_PINMUX19_PINMUX19_3_0_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX19);
	printk("am1808-gpio:configure GPIO8_9 as gpio\n");   
	status = gpio_request(8*16+9, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 8*16+9);
		return status;
	}
	gpio_direction_input(8*16+9);

	// SET_GPIO_IRDA_CTL 	 GPIO8[10]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX18);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX18_PINMUX18_31_28_MASK );
	TmpRegVal |=  (CSL_SYSCFG_PINMUX18_PINMUX18_31_28_GPIO8_10 << CSL_SYSCFG_PINMUX18_PINMUX18_31_28_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX18);
	printk("am1808-gpio:configure GPIO8_10 as gpio\n");   
	status = gpio_request(8*16+10, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 8*16+10);
		return status;
	}
	gpio_direction_output(8*16+10,1);

	// SET_GPIO_BAT_CTL 	 GPIO8[8]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX19);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX19_PINMUX19_7_4_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX19_PINMUX19_7_4_GPIO8_8 << CSL_SYSCFG_PINMUX19_PINMUX19_7_4_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX19);
	printk("am1808-gpio:configure GPIO8_8 as gpio\n");   
	status = gpio_request(8*16+8, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 8*16+8);
		return status;
	}
	gpio_direction_output(8*16+8,1);

	// SET_GPIO_LCD_BL 	 GPIO7[11]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX16);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX16_PINMUX16_27_24_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX16_PINMUX16_27_24_GPIO7_11 << CSL_SYSCFG_PINMUX16_PINMUX16_27_24_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX16);
	printk("am1808-gpio:configure GPIO7_11 as gpio\n");   
	status = gpio_request(7*16+11, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 7*16+11);
		return status;
	}
	gpio_direction_output(7*16+11,1);


	// GET_GPIO_BTN_INT 	 GPIO7[12]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX16);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX16_PINMUX16_23_20_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX16_PINMUX16_23_20_GPIO7_12 << CSL_SYSCFG_PINMUX16_PINMUX16_23_20_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX16);
	printk("am1808-gpio:configure GPIO7_12 as gpio\n");   
	status = gpio_request(7*16+12, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 7*16+12);
		return status;
	}
	gpio_direction_input(7*16+12);

	
	// GET_GPIO_AD_INT 	 GPIO8[11]
    TmpRegVal  = __raw_readl(SYSCFG0_PINMUX18);
	TmpRegVal &= ~(CSL_SYSCFG_PINMUX18_PINMUX18_27_24_MASK);
	TmpRegVal |=  (CSL_SYSCFG_PINMUX18_PINMUX18_27_24_GPIO8_11 << CSL_SYSCFG_PINMUX18_PINMUX18_27_24_SHIFT);
	__raw_writel(TmpRegVal,SYSCFG0_PINMUX18);
	printk("am1808-gpio:configure GPIO8_11 as gpio\n");   
	status = gpio_request(8*16+11, "gpio_test\n");
	if (status < 0) {
		printk("am1808-gpio:error can not open GPIO %d\n", 8*16+11);
		return status;
	}
	gpio_direction_input(8*16+11);

   	ret = misc_register(&misc); //注册设备 
   	printk (DEVICE_NAME"\tinitialized\n"); //打印初始化信息 
   	return ret; 
} 
 
static void __exit dev_exit(void) 
{ 
	gpio_free(7*16+13);
	gpio_free(7*16+15);
	gpio_free(7*16+14);
	gpio_free(7*16+12);
	gpio_free(8*16+11);
	gpio_free(7*16+10);
	gpio_free(7*16+9);
	gpio_free(7*16+8);
	gpio_free(8*16+9);
	gpio_free(8*16+10);
	gpio_free(8*16+8);	
	gpio_free(7*16+11);
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
