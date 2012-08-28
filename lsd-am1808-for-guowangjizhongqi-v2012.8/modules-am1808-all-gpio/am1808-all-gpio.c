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

#define M_GPIO_OUTPUT  	0
#define M_GPIO_INPUT	1
#define M_GPIO_OUTPUT_HIGH  	1
#define M_GPIO_OUTPUT_LOW		0

#define DEVICE_NAME "am1808-all-gpio" //设备名(/dev/led) 
#define MOTOR_MAGIC 'g'
#define AM1808_GPIO_CMD_BASE		_IOW(MOTOR_MAGIC, 0,int)
#define SET_GPIO_DIRECT                 (AM1808_GPIO_CMD_BASE + 0)     // cmd是这个值  arg的值  M_GPIO_INPUT=输入  M_GPIO_OUTPUT=输出 
#define SET_GPIO_VALUE_BASE 		(AM1808_GPIO_CMD_BASE + 200)   // cmd是这个值  arg的值  M_GPIO_OUTPUT_HIGH=高电平  M_GPIO_OUTPUT_LOW=高电平 
#define GET_GPIO_VALUE_BASE 		(AM1808_GPIO_CMD_BASE + 400)   // cmd是这个值  arg的值表示返回值的指针

	
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
	// LCD_RST
	{
		0*16+12,     
		"AFSX / GPIO0[12]",  
		SYSCFG0_PINMUX0, 
		CSL_SYSCFG_PINMUX0_PINMUX0_15_12_MASK,
		CSL_SYSCFG_PINMUX0_PINMUX0_15_12_SHIFT,
		CSL_SYSCFG_PINMUX0_PINMUX0_15_12_GPIO0_12,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_LOW
	},
	// LCD_BL
	{
		0*16+14,     
		"ACLKX / GPIO0[14]",  
		SYSCFG0_PINMUX0, 
		CSL_SYSCFG_PINMUX0_PINMUX0_7_4_MASK,
		CSL_SYSCFG_PINMUX0_PINMUX0_7_4_SHIFT,
		CSL_SYSCFG_PINMUX0_PINMUX0_7_4_GPIO0_14,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_LOW
	},
	
	// KEY_1 
	{
		4*16+7,     
		"EMA_A_23/MMCSD0_CLK/GPIO4[7]",  
		SYSCFG0_PINMUX10, 
		CSL_SYSCFG_PINMUX10_PINMUX10_3_0_MASK,
		CSL_SYSCFG_PINMUX10_PINMUX10_3_0_SHIFT,
		CSL_SYSCFG_PINMUX10_PINMUX10_3_0_GPIO4_7,
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// KEY_2
	{
		4*16+6,     
		"EMA_A_22/MMCSD0_CMD/GPIO4[6]",  
		SYSCFG0_PINMUX10, 
		CSL_SYSCFG_PINMUX10_PINMUX10_7_4_MASK,
		CSL_SYSCFG_PINMUX10_PINMUX10_7_4_SHIFT,
		CSL_SYSCFG_PINMUX10_PINMUX10_7_4_GPIO4_6,
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// KEY_3
	{
		4*16+0,     
		"EMA_A_16/MMCSD0_D5/GPIO4[0]",  
		SYSCFG0_PINMUX10, 
		CSL_SYSCFG_PINMUX10_PINMUX10_31_28_MASK,
		CSL_SYSCFG_PINMUX10_PINMUX10_31_28_SHIFT,
		CSL_SYSCFG_PINMUX10_PINMUX10_31_28_GPIO4_0,
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// KEY_4
	{
		4*16+1,     
		"EMA_A_17/MMCSD0_D4/GPIO4[1]",  
		SYSCFG0_PINMUX10, 
		CSL_SYSCFG_PINMUX10_PINMUX10_27_24_MASK,
		CSL_SYSCFG_PINMUX10_PINMUX10_27_24_SHIFT,
		CSL_SYSCFG_PINMUX10_PINMUX10_27_24_GPIO4_1,
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// KEY_5
	{
		4*16+2,     
		"EMA_A_18/MMCSD0_D3/GPIO4[2]",  
		SYSCFG0_PINMUX10, 
		CSL_SYSCFG_PINMUX10_PINMUX10_23_20_MASK,
		CSL_SYSCFG_PINMUX10_PINMUX10_23_20_SHIFT,
		CSL_SYSCFG_PINMUX10_PINMUX10_23_20_GPIO4_2,
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// KEY_6
	{
		4*16+3,     
		"EMA_A_19/MMCSD0_D2/GPIO4[3]",  
		SYSCFG0_PINMUX10, 
		CSL_SYSCFG_PINMUX10_PINMUX10_19_16_MASK,
		CSL_SYSCFG_PINMUX10_PINMUX10_19_16_SHIFT,
		CSL_SYSCFG_PINMUX10_PINMUX10_19_16_GPIO4_3,
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// SAM_RST
	{
		0*16+6,     
		"AXR14 / McBSP1_CLKR / GPIO0[6]", 
		SYSCFG0_PINMUX1, 
		CSL_SYSCFG_PINMUX1_PINMUX1_7_4_MASK,
		CSL_SYSCFG_PINMUX1_PINMUX1_7_4_SHIFT,
		CSL_SYSCFG_PINMUX1_PINMUX1_7_4_GPIO0_6,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},	
	// ZB_SET
	{
		6*16+6,     
		"VPIF_CLKIN1 / UHPI_HDS1n / GPIO6[6]",  
		SYSCFG0_PINMUX14, 
		CSL_SYSCFG_PINMUX14_PINMUX14_7_4_MASK,
		CSL_SYSCFG_PINMUX14_PINMUX14_7_4_SHIFT,
		CSL_SYSCFG_PINMUX14_PINMUX14_7_4_GPIO6_6,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// ZB_RST
	{
		6*16+7,     
		"VPIF_CLKIN0 / UHPI_HCSn / GPIO6[7] / RPI_2xTXCLK",  
		SYSCFG0_PINMUX14, 
		CSL_SYSCFG_PINMUX14_PINMUX14_3_0_MASK,
		CSL_SYSCFG_PINMUX14_PINMUX14_3_0_SHIFT,
		CSL_SYSCFG_PINMUX14_PINMUX14_3_0_GPIO6_7,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// STATE4 
	{
		5*16+10,     
		"EMA_A_10/GPIO5[10]",  
		SYSCFG0_PINMUX11, 
		CSL_SYSCFG_PINMUX11_PINMUX11_23_20_MASK,
		CSL_SYSCFG_PINMUX11_PINMUX11_23_20_SHIFT,
		CSL_SYSCFG_PINMUX11_PINMUX11_23_20_GPIO5_10,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// STATE3
	{
		5*16+9,     
		"EMA_A_9/GPIO5[9]",  
		SYSCFG0_PINMUX11, 
		CSL_SYSCFG_PINMUX11_PINMUX11_27_24_MASK,
		CSL_SYSCFG_PINMUX11_PINMUX11_27_24_SHIFT,
		CSL_SYSCFG_PINMUX11_PINMUX11_27_24_GPIO5_9,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// STATE2
	{
		5*16+15,     
		"EMA_A_15/MMCSD0_D6/GPIO5[15]",  
		SYSCFG0_PINMUX11, 
		CSL_SYSCFG_PINMUX11_PINMUX11_3_0_MASK,
		CSL_SYSCFG_PINMUX11_PINMUX11_3_0_SHIFT,
		CSL_SYSCFG_PINMUX11_PINMUX11_3_0_GPIO5_15,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// STATE1
	{
		5*16+13,     
		"EMA_A_13/GPIO5[13]",  
		SYSCFG0_PINMUX11, 
		CSL_SYSCFG_PINMUX11_PINMUX11_11_8_MASK,
		CSL_SYSCFG_PINMUX11_PINMUX11_11_8_SHIFT,
		CSL_SYSCFG_PINMUX11_PINMUX11_11_8_GPIO5_13,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// STATE0
	{
		5*16+4,     
		"EMA_A_4/GPIO5[4]",  
		SYSCFG0_PINMUX12, 
		CSL_SYSCFG_PINMUX12_PINMUX12_15_12_MASK,
		CSL_SYSCFG_PINMUX12_PINMUX12_15_12_SHIFT,
		CSL_SYSCFG_PINMUX12_PINMUX12_15_12_GPIO5_4,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// U1_DCD
	{
		5*16+7,     
		"EMA_A_7/GPIO5[7]",  
		SYSCFG0_PINMUX12, 
		CSL_SYSCFG_PINMUX12_PINMUX12_3_0_MASK,
		CSL_SYSCFG_PINMUX12_PINMUX12_3_0_SHIFT,
		CSL_SYSCFG_PINMUX12_PINMUX12_3_0_GPIO5_7,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// U1_DTR
	{
		5*16+8,     
		"EMA_A_8/GPIO5[8]",  
		SYSCFG0_PINMUX11, 
		CSL_SYSCFG_PINMUX11_PINMUX11_31_28_MASK,
		CSL_SYSCFG_PINMUX11_PINMUX11_31_28_SHIFT,
		CSL_SYSCFG_PINMUX11_PINMUX11_31_28_GPIO5_8,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// U1_RI
	{
		5*16+11,     
		"EMA_A_11/GPIO5[11]",  
		SYSCFG0_PINMUX11, 
		CSL_SYSCFG_PINMUX11_PINMUX11_19_16_MASK,
		CSL_SYSCFG_PINMUX11_PINMUX11_19_16_SHIFT,
		CSL_SYSCFG_PINMUX11_PINMUX11_19_16_GPIO5_11,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// GPRS_IGT
	{
		5*16+14,     
		"EMA_A_14/MMCSD0_D7/GPIO5[14]",  
		SYSCFG0_PINMUX11, 
		CSL_SYSCFG_PINMUX11_PINMUX11_7_4_MASK,
		CSL_SYSCFG_PINMUX11_PINMUX11_7_4_SHIFT,
		CSL_SYSCFG_PINMUX11_PINMUX11_7_4_GPIO5_14,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// GPRS_RST
	{
		4*16+5,     
		"EMA_A_21/MMCSD0_D0/GPIO4[5]",  
		SYSCFG0_PINMUX10, 
		CSL_SYSCFG_PINMUX10_PINMUX10_11_8_MASK,
		CSL_SYSCFG_PINMUX10_PINMUX10_11_8_SHIFT,
		CSL_SYSCFG_PINMUX10_PINMUX10_11_8_GPIO4_5,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// GPRS_PCTRL
	{
		4*16+4,     
		"EMA_A_20/MMCSD0_D1/GPIO4[4]",  
		SYSCFG0_PINMUX10, 
		CSL_SYSCFG_PINMUX10_PINMUX10_15_12_MASK,
		CSL_SYSCFG_PINMUX10_PINMUX10_15_12_SHIFT,
		CSL_SYSCFG_PINMUX10_PINMUX10_15_12_GPIO4_4,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},

	// YX_B 
	{
		2*16+14,     
		"TM64P3_IN12/SPI1_SCS[0]/EPWM1B/PRU0_R30[7]/GP2[14]",  
		SYSCFG0_PINMUX5, 
		CSL_SYSCFG_PINMUX5_PINMUX5_7_4_MASK,
		CSL_SYSCFG_PINMUX5_PINMUX5_7_4_SHIFT,
		CSL_SYSCFG_PINMUX5_PINMUX5_7_4_GPIO2_14,
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// YX_A
	{
		2*16+13,     
		"SPI1_CLK/GP2[13]",  
		SYSCFG0_PINMUX5, 
		CSL_SYSCFG_PINMUX5_PINMUX5_11_8_MASK,
		CSL_SYSCFG_PINMUX5_PINMUX5_11_8_SHIFT,
		CSL_SYSCFG_PINMUX5_PINMUX5_11_8_GPIO2_13,
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// WDI
	{
		6*16+4,     
		"PRU1_R31[4]/VPIF_CLKIN2/MMCSD1_DAT[3]/PRU1_R30[3]/GP6[4]",  
		SYSCFG0_PINMUX19, 
		CSL_SYSCFG_PINMUX19_PINMUX19_11_8_MASK,
		CSL_SYSCFG_PINMUX19_PINMUX19_11_8_SHIFT,
		CSL_SYSCFG_PINMUX19_PINMUX19_11_8_GPIO6_4,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
	// ERR_LER
	{
		0*16+9,     
		"PRU0_R31[16]/AMUTE/PRU0_R30[16]/UART2_RTS/GP0[9]",  
		SYSCFG0_PINMUX0, 
		CSL_SYSCFG_PINMUX0_PINMUX0_27_24_MASK,
		CSL_SYSCFG_PINMUX0_PINMUX0_27_24_SHIFT,
		CSL_SYSCFG_PINMUX0_PINMUX0_27_24_GPIO0_9,
		M_GPIO_OUTPUT,
		M_GPIO_OUTPUT_HIGH
	},
};


int fn_am1808_all_gpio_init(void)
{
	int gpio_cnt;	
	int i;
	unsigned long TmpRegVal;
	int status;

	gpio_cnt = sizeof(am1808_all_gpio)/sizeof(struct st_am1808_gpio);	
	printk("am1808_all_gpio count = %d\n",gpio_cnt);
	
	for(i = 0; i < gpio_cnt; i++)
	{
		//if(am1808_all_gpio[i].gpio_num != 0)
		{
			TmpRegVal  = __raw_readl(am1808_all_gpio[i].syscfg0_pinmux);
			TmpRegVal &= ~(am1808_all_gpio[i].syscfg0_pinmux_mask);
			TmpRegVal |=  (am1808_all_gpio[i].syscfg0_pinmux_function << am1808_all_gpio[i].syscfg0_pinmux_shift);
			__raw_writel(TmpRegVal,am1808_all_gpio[i].syscfg0_pinmux);
			
			status = gpio_request(am1808_all_gpio[i].gpio_num, am1808_all_gpio[i].gpio_all_function);
			if (status < 0) {
				printk("am1808-all-gpio:error can not request GPIO %s\n", am1808_all_gpio[i].gpio_all_function);
				return status;
			}
		
			if(am1808_all_gpio[i].gpio_direction == M_GPIO_OUTPUT)
			{
				gpio_direction_output(am1808_all_gpio[i].gpio_num,am1808_all_gpio[i].gpio_output_state);
				printk("am1808-all-gpio:configure %s as gpio output,output state is %d\n",
					am1808_all_gpio[i].gpio_all_function,am1808_all_gpio[i].gpio_output_state); 
			}
			else if(am1808_all_gpio[i].gpio_direction == M_GPIO_INPUT)
			{
				gpio_direction_input(am1808_all_gpio[i].gpio_num);
				printk("am1808-all-gpio:configure %s as gpio input\n",
					am1808_all_gpio[i].gpio_all_function); 
			}
			else
			{
				printk("am1808-all-gpio:error direction error\n");
				return -1;
			}
		}		  
		
	}
	return 0;
}

int fn_am1808_all_gpio_exit(void)
{
	int gpio_cnt;	
	int i;

	gpio_cnt = sizeof(am1808_all_gpio)/sizeof(struct st_am1808_gpio);	
	printk("am1808_all_gpio count = %d\n",gpio_cnt);
	
	for(i = 0; i < gpio_cnt; i++)
	{
		gpio_free(am1808_all_gpio[i].gpio_num);
	}
	return 0;
}

int fn_search_gpio_index(int gpio_num)
{
	int i;
	int size;
	size = sizeof(am1808_all_gpio)/sizeof(st_am1808_gpio);
	
	for(i = 0; i < size; i++)
	{
		if(am1808_all_gpio[i].gpio_num == gpio_num)
			return i;
	}
	return -1;
}




// ioctl 函数的实现 
// 在应用用户层将通过 ioctl 函数向内核传递参数，以控制 LED的输出状态 
static int am1808_gpio_ioctl( 
 struct inode *inode,  
 struct file *file,   
 unsigned int cmd,  
 unsigned long arg) 
{ 
	int gpio_num;	
	
		
	#if 0
	if(cmd >= GET_GPIO_VALUE_BASE)
	{
		printk("am1808-gpio:GET_GPIO_VALUE_BASE arg=%d\r\n",arg);
	}
	else if(cmd >= SET_GPIO_VALUE_BASE)
	{
		printk("am1808-gpio:SET_GPIO_VALUE_BASE arg=%d\r\n",arg);
	}
	else if(cmd >= SET_GPIO_DIRECT)
	{
		printk("am1808-gpio:SET_GPIO_DIRECT arg=%d\r\n",arg);
	}
	else
	{
		printk("am1808-gpio:cmd err arg=%d\r\n",arg);
	}
	printk("am1808-gpio:cmd interger is %d,hex is 0x%08x\n",(int)cmd,(unsigned int)cmd);
	printk("am1808-gpio:arg interger is %d,hex is 0x%08x\n",(int)arg,(unsigned int)arg);
	#endif
	if(cmd >= (GET_GPIO_VALUE_BASE + 200))
	{
		printk("am1808-gpio:cmd error,out\n");
	}
	else if(cmd >= GET_GPIO_VALUE_BASE)
	{
		gpio_num = cmd - GET_GPIO_VALUE_BASE;
		if(fn_search_gpio_index(gpio_num) == -1)
		{
			printk("am1808-gpio:cmd=GET_GPIO_VALUE_BASE gpio_num=%d not exist\n",gpio_num);
		}
		else
		{
			if(((unsigned long*)arg) == ((unsigned long*)NULL))
			{
				printk("am1808-gpio:cmd=GET_GPIO_VALUE_BASE arg not right\n");
			}			
			if(gpio_get_value(gpio_num) == 0)			
				*((unsigned long*)arg) = 0;
			else
				*((unsigned long*)arg) = 1;
		}
	}
	else if(cmd >= SET_GPIO_VALUE_BASE)
	{
		gpio_num = cmd - SET_GPIO_VALUE_BASE;
		if(fn_search_gpio_index(gpio_num) == -1)
		{
			printk("am1808-gpio:cmd=SET_GPIO_VALUE_BASE gpio_num=%d not exist\n",gpio_num);
		}
		else
		{
			//printk("gpio_num=%d\r\n",gpio_num);			
			if(arg == M_GPIO_OUTPUT_HIGH)
			{
				gpio_set_value(gpio_num,1); 
			}
			else if(arg == M_GPIO_OUTPUT_LOW)
			{
				gpio_set_value(gpio_num,0); 
			}
			else
			{
				printk("am1808-gpio:cmd=SET_GPIO_VALUE_BASE arg not right\n");
			}
		}
	}
	// 设定gpio方向
	else if(cmd >= SET_GPIO_DIRECT)
	{
		gpio_num = cmd - SET_GPIO_DIRECT;
		if(fn_search_gpio_index(gpio_num) == -1)
		{
			printk("am1808-gpio:cmd=SET_GPIO_DIRECT gpio_num=%d not exist\n",gpio_num);
		}
		else
		{
			//printk("gpio_num=%d\r\n",gpio_num);					
			if(arg == M_GPIO_OUTPUT)
			{
				gpio_direction_output(gpio_num,M_GPIO_OUTPUT_HIGH);
			}
			else if(arg == M_GPIO_INPUT)
			{
				gpio_direction_input(gpio_num);
			}
			else
			{
				printk("am1808-gpio:cmd=SET_GPIO_DIRECT arg not right\n");
			}
		}
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
	int ret;
	int size;
	fn_am1808_all_gpio_init();

   	ret = misc_register(&misc); //注册设备 
   	printk (DEVICE_NAME"\tinitialized\n"); //打印初始化信息 
	
	size = sizeof(am1808_all_gpio)/sizeof(st_am1808_gpio);
	printk("sizeof am1808_all_gpio =%d,sizeof st_am1808_gpio=%d,size=%d\r\n",
			sizeof(am1808_all_gpio),sizeof(st_am1808_gpio),size);
	printk("SET_GPIO_DIRECT=0x%08x,SET_GPIO_VALUE_BASE=0x%08x,GET_GPIO_VALUE_BASE=0x%08x\r\n",
			SET_GPIO_DIRECT,SET_GPIO_VALUE_BASE,GET_GPIO_VALUE_BASE);

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
