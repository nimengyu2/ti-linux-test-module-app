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
#include <asm/gpio.h>
#include <linux/delay.h>
#include <linux/clk.h>


#define M_GPIO_OUTPUT  	0
#define M_GPIO_INPUT	1
#define M_GPIO_OUTPUT_HIGH  	1
#define M_GPIO_OUTPUT_LOW		0

#define DEVICE_NAME "am335x-gpio" //设备名(/dev/led) 
#define MOTOR_MAGIC 'g'
#define AM1808_GPIO_CMD_BASE		_IOW(MOTOR_MAGIC, 0,int)
#define SET_GPIO_DIRECT                 (AM1808_GPIO_CMD_BASE + 0)     // cmd是这个值  arg的值  M_GPIO_INPUT=输入  M_GPIO_OUTPUT=输出 
#define SET_GPIO_VALUE_BASE 		(AM1808_GPIO_CMD_BASE + 200)   // cmd是这个值  arg的值  M_GPIO_OUTPUT_HIGH=高电平  M_GPIO_OUTPUT_LOW=高电平 
#define GET_GPIO_VALUE_BASE 		(AM1808_GPIO_CMD_BASE + 400)   // cmd是这个值  arg的值表示返回值的指针

	
struct st_am1808_gpio
{
	int gpio_num;  // gpio端口号
	char* gpio_all_function; // 该gpio所有功能
	int gpio_direction;  // 该gpio方向，输入输出
	int gpio_output_state; // 输出状态
}st_am1808_gpio;

struct st_am1808_gpio am1808_all_gpio[] = 
{
	{
		1*32+16,     
		"GPIO1[16] --- GPIO1",  
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+17,     
		"GPIO1[17] --- GPIO2",  
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+18,     
		"GPIO1[18] --- GPIO3",  
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+19,     
		"GPIO1[19] --- GPIO4",  
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+20,     
		"GPIO1[20] --- GPIO5",  
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+21,     
		"GPIO1[21] --- GPIO6",  
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+22,     
		"GPIO1[22] --- GPIO7",  
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+23,     
		"GPIO1[23] --- GPIO8", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+24,     
		"GPIO1[24] --- GPIO9",  
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+25,     
		"GPIO1[25] --- GPIO10", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+26,     
		"GPIO1[26] --- GPIO11", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+27,     
		"GPIO1[27] --- GPIO12", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		0*32+4,     
		"GPIO0[4] --- GPIO13", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		0*32+5,     
		"GPIO0[5] --- GPIO14", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},


	{
		3*32+5,     
		"GPIO3[5] --- PWM2_OUT", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		3*32+6,     
		"GPIO3[6] --- PWM1_OUT", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},


	{
		1*32+9,     
		"GPIO1[9] --- UART4_TXD", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		1*32+8,     
		"GPIO1[8] --- UART4_RXD", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},

	{
		0*32+15,     
		"GPIO0[15] --- UART1_TXD", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		0*32+14,     
		"GPIO0[14] --- UART1_RXD", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},

	{
		0*32+7,     
		"GPIO0[7] --- UART3_TXD", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		0*32+6,     
		"GPIO0[6] --- UART3_RXD", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},


	{
		0*32+3,     
		"GPIO0[3] --- UART2_TXD", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		0*32+2,     
		"GPIO0[2] --- UART2_RXD", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},

	{
		0*32+29,     
		"GPIO0[29] --- UART5_TXD", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
	{
		3*32+0,     
		"GPIO3[0] --- UART5_RXD", 
		M_GPIO_INPUT,
		M_GPIO_OUTPUT_HIGH
	},
};


int fn_am1808_all_gpio_init(void)
{
	int gpio_cnt;	
	int i;
	//unsigned long TmpRegVal;
	int status;

	gpio_cnt = sizeof(am1808_all_gpio)/sizeof(struct st_am1808_gpio);	
	printk("am1808_all_gpio count = %d\n",gpio_cnt);
	
	for(i = 0; i < gpio_cnt; i++)
	{
		//if(am1808_all_gpio[i].gpio_num != 0)
		{
			#if 0
			TmpRegVal  = __raw_readl(am1808_all_gpio[i].syscfg0_pinmux);
			TmpRegVal &= ~(am1808_all_gpio[i].syscfg0_pinmux_mask);
			TmpRegVal |=  (am1808_all_gpio[i].syscfg0_pinmux_function << am1808_all_gpio[i].syscfg0_pinmux_shift);
			__raw_writel(TmpRegVal,am1808_all_gpio[i].syscfg0_pinmux);
			#endif
			
			status = gpio_request(am1808_all_gpio[i].gpio_num, am1808_all_gpio[i].gpio_all_function);
			if (status < 0) {
				printk("am335x-gpio:error can not request GPIO %s\n", am1808_all_gpio[i].gpio_all_function);
				return status;
			}
		
			if(am1808_all_gpio[i].gpio_direction == M_GPIO_OUTPUT)
			{
				gpio_direction_output(am1808_all_gpio[i].gpio_num,am1808_all_gpio[i].gpio_output_state);
				printk("am335x-gpio:configure %s as gpio output,output state is %d\n",
					am1808_all_gpio[i].gpio_all_function,am1808_all_gpio[i].gpio_output_state); 
			}
			else if(am1808_all_gpio[i].gpio_direction == M_GPIO_INPUT)
			{
				gpio_direction_input(am1808_all_gpio[i].gpio_num);
				printk("am335x-gpio:configure %s as gpio input\n",
					am1808_all_gpio[i].gpio_all_function); 
			}
			else
			{
				printk("am335x-gpio:error direction error\n");
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
 struct file *file,   
 unsigned int cmd,  
 unsigned long arg) 
{ 
	int gpio_num;	
	
		
	#if 0
	if(cmd >= GET_GPIO_VALUE_BASE)
	{
		printk("am335x-gpio:GET_GPIO_VALUE_BASE arg=%d\r\n",arg);
	}
	else if(cmd >= SET_GPIO_VALUE_BASE)
	{
		printk("am335x-gpio:SET_GPIO_VALUE_BASE arg=%d\r\n",arg);
	}
	else if(cmd >= SET_GPIO_DIRECT)
	{
		printk("am335x-gpio:SET_GPIO_DIRECT arg=%d\r\n",arg);
	}
	else
	{
		printk("am335x-gpio:cmd err arg=%d\r\n",arg);
	}
	printk("am335x-gpio:cmd interger is %d,hex is 0x%08x\n",(int)cmd,(unsigned int)cmd);
	printk("am335x-gpio:arg interger is %d,hex is 0x%08x\n",(int)arg,(unsigned int)arg);
	#endif
	if(cmd >= (GET_GPIO_VALUE_BASE + 200))
	{
		printk("am335x-gpio:cmd error,out\n");
	}
	else if(cmd >= GET_GPIO_VALUE_BASE)
	{
		gpio_num = cmd - GET_GPIO_VALUE_BASE;
		if(fn_search_gpio_index(gpio_num) == -1)
		{
			printk("am335x-gpio:cmd=GET_GPIO_VALUE_BASE gpio_num=%d not exist\n",gpio_num);
		}
		else
		{
			if(((unsigned long*)arg) == ((unsigned long*)NULL))
			{
				printk("am335x-gpio:cmd=GET_GPIO_VALUE_BASE arg not right\n");
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
			printk("am335x-gpio:cmd=SET_GPIO_VALUE_BASE gpio_num=%d not exist\n",gpio_num);
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
				printk("am335x-gpio:cmd=SET_GPIO_VALUE_BASE arg not right\n");
			}
		}
	}
	// 设定gpio方向
	else if(cmd >= SET_GPIO_DIRECT)
	{
		gpio_num = cmd - SET_GPIO_DIRECT;
		if(fn_search_gpio_index(gpio_num) == -1)
		{
			printk("am335x-gpio:cmd=SET_GPIO_DIRECT gpio_num=%d not exist\n",gpio_num);
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
				printk("am335x-gpio:cmd=SET_GPIO_DIRECT arg not right\n");
			}
		}
	}
   	return 0;       
} 
 
 
//  设备函数操作集，在此只有 ioctl函数，通常还有 read, write, open, close 等，因为本 LED驱动在下面已经
//  注册为 misc 设备，因此也可以不用 open/close  
static struct file_operations dev_fops = { 
 .owner = THIS_MODULE, 
 .unlocked_ioctl = am1808_gpio_ioctl, 
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
