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
//#include "am1808-mux.h"

#define DEVICE_NAME "am1808-mux" //设备名(/dev/led) 
#define MOTOR_MAGIC 'm'
#define SET_MUX_IIC0 		_IOW(MOTOR_MAGIC, 2,int)    
#define SET_MUX_UART0 		_IOW(MOTOR_MAGIC, 3,int) 
#define SET_MUX_UART1 		_IOW(MOTOR_MAGIC, 4,int) 
#define SET_MUX_UART2 		_IOW(MOTOR_MAGIC, 5,int) 
#define SET_MUX_PRU_UART	_IOW(MOTOR_MAGIC, 6,int) 
#define SET_MUX_PWM			_IOW(MOTOR_MAGIC, 7,int) 
#define SET_MUX_SPI0		_IOW(MOTOR_MAGIC, 8,int) 

#define M_GPIO_OUTPUT  	0
#define M_GPIO_INPUT	1
#define M_GPIO_FUNCTION 2

#define M_GPIO_OUTPUT_HIGH  	1
#define M_GPIO_OUTPUT_LOW		0	
#define M_GPIO_OUTPUT_UNKNOW    2

#define M_GPIO_NUM_NOIO_FUNC	16*16
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

struct st_am1808_gpio am1808_mux_iic0[] = 
{
	{
		1*16+5,     
		"SPI1_SCSn_7 / I2C0_SCL / Timer64P2_OUT12 / GPIO1[5]",  
		SYSCFG0_PINMUX4, 
		CSL_SYSCFG_PINMUX4_PINMUX4_11_8_MASK,
		CSL_SYSCFG_PINMUX4_PINMUX4_11_8_SHIFT,
		CSL_SYSCFG_PINMUX4_PINMUX4_11_8_I2C0_SCL,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		1*16+4,     
		"SPI1_SCSn_6 / I2C0_SDA / Timer64P3_OUT12 / GPIO1[4]",  
		SYSCFG0_PINMUX4, 
		CSL_SYSCFG_PINMUX4_PINMUX4_15_12_MASK,
		CSL_SYSCFG_PINMUX4_PINMUX4_15_12_SHIFT,
		CSL_SYSCFG_PINMUX4_PINMUX4_15_12_I2C0_SDA,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
};

struct st_am1808_gpio am1808_mux_uart0[] = 
{
	{
		8*16+4,     
		"SPI0_SCSn_5 / UART0_RXD / GPIO8[4] / EMAC_MII_RXD[3]",  
		SYSCFG0_PINMUX3, 
		CSL_SYSCFG_PINMUX3_PINMUX3_19_16_MASK,
		CSL_SYSCFG_PINMUX3_PINMUX3_19_16_SHIFT,
		CSL_SYSCFG_PINMUX3_PINMUX3_19_16_UART0_RXD,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		8*16+3,     
		"SPI0_SCSn_4 / UART0_TXD / GPIO8[3] / EMAC_MII_RXD[2]",  
		SYSCFG0_PINMUX3, 
		CSL_SYSCFG_PINMUX3_PINMUX3_23_20_MASK,
		CSL_SYSCFG_PINMUX3_PINMUX3_23_20_SHIFT,
		CSL_SYSCFG_PINMUX3_PINMUX3_23_20_UART0_TXD,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
};

struct st_am1808_gpio am1808_mux_uart1[] = 
{
	{
		1*16+1,     
		"SPI1_SCSn_3 / UART1_RXD / SATA_LED / GPIO1[1]",  
		SYSCFG0_PINMUX4, 
		CSL_SYSCFG_PINMUX4_PINMUX4_27_24_MASK,
		CSL_SYSCFG_PINMUX4_PINMUX4_27_24_SHIFT,
		CSL_SYSCFG_PINMUX4_PINMUX4_27_24_UART1_RXD,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		1*16+0,     
		"SPI1_SCSn_2 / UART1_TXD / SATA_CP_POD / GPIO1[0]",  
		SYSCFG0_PINMUX4, 
		CSL_SYSCFG_PINMUX4_PINMUX4_31_28_MASK,
		CSL_SYSCFG_PINMUX4_PINMUX4_31_28_SHIFT,
		CSL_SYSCFG_PINMUX4_PINMUX4_31_28_UART1_TXD,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		0*16+10,     
		"AHCLKX / USB_REFCLKIN / UART1_CTSn / GPIO0[10]",  
		SYSCFG0_PINMUX0, 
		CSL_SYSCFG_PINMUX0_PINMUX0_23_20_MASK,
		CSL_SYSCFG_PINMUX0_PINMUX0_23_20_SHIFT,
		CSL_SYSCFG_PINMUX0_PINMUX0_23_20_UART1_CTS,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		0*16+11,     
		"AHCLKR / UART1_RTSn / GPIO0[11]",  
		SYSCFG0_PINMUX0, 
		CSL_SYSCFG_PINMUX0_PINMUX0_19_16_MASK,
		CSL_SYSCFG_PINMUX0_PINMUX0_19_16_SHIFT,
		CSL_SYSCFG_PINMUX0_PINMUX0_19_16_UART1_RTS,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
};

struct st_am1808_gpio am1808_mux_uart2[] = 
{
	
};

struct st_am1808_gpio am1808_mux_pru_uart[] = 
{
	// am1808 南方电力核心板子 AXR start
	{
		1*16+13,     
		"AXR5  / McBSP0_C LKX / GPIO1[13]  /  EMAC_MII_TXCLK",  // 485-RX3
		SYSCFG0_PINMUX2, 
		CSL_SYSCFG_PINMUX2_PINMUX2_11_8_MASK,
		CSL_SYSCFG_PINMUX2_PINMUX2_11_8_SHIFT,
		CSL_SYSCFG_PINMUX2_PINMUX2_11_8_AXR0_5,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		8*16+7,     
		"AXR0  / E CAP1_APWM0 /  GPIO8 [7]  / EMAC _MII_TXD[0 ] / McB SP0_CL KS",  //485-TX1
		SYSCFG0_PINMUX2, 
		CSL_SYSCFG_PINMUX2_PINMUX2_31_28_MASK,
		CSL_SYSCFG_PINMUX2_PINMUX2_31_28_SHIFT,
		CSL_SYSCFG_PINMUX2_PINMUX2_31_28_AXR0_0,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		1*16+12,     
		"AXR4  / McB SP0 _FS R /  GPIO1[12]  / EMAC_MII_COL",  //485-TX3
		SYSCFG0_PINMUX2, 
		CSL_SYSCFG_PINMUX2_PINMUX2_15_12_MASK,
		CSL_SYSCFG_PINMUX2_PINMUX2_15_12_SHIFT,
		CSL_SYSCFG_PINMUX2_PINMUX2_15_12_AXR0_4,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		1*16+11,     
		"AXR3  / McB SP0 _FS X  / GPIO1 [11] / EMAC_MII_TXD[3]",  //485-RX2
		SYSCFG0_PINMUX2, 
		CSL_SYSCFG_PINMUX2_PINMUX2_19_16_MASK,
		CSL_SYSCFG_PINMUX2_PINMUX2_19_16_SHIFT,
		CSL_SYSCFG_PINMUX2_PINMUX2_19_16_AXR0_3,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		1*16+10,     
		"AXR2 / McBSP0_DR / GPIO1[10] / EMAC_MII_TXD[2]",  //485-TX2
		SYSCFG0_PINMUX2, 
		CSL_SYSCFG_PINMUX2_PINMUX2_23_20_MASK,
		CSL_SYSCFG_PINMUX2_PINMUX2_23_20_SHIFT,
		CSL_SYSCFG_PINMUX2_PINMUX2_23_20_GPIO1_10,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		1*16+9,     
		"AXR1 / McBSP0_DX / GPIO1[9] / EMAC_MII_TXD[1]",  //485-RX1
		SYSCFG0_PINMUX2, 
		CSL_SYSCFG_PINMUX2_PINMUX2_27_24_MASK,
		CSL_SYSCFG_PINMUX2_PINMUX2_27_24_SHIFT,
		CSL_SYSCFG_PINMUX2_PINMUX2_27_24_AXR0_1,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		0*16+0,     
		"AXR8 / McBSP1_CLKS / ECAP1_APWM1 / GPIO0[0]",  //EUART_TX
		SYSCFG0_PINMUX1, 
		CSL_SYSCFG_PINMUX1_PINMUX1_31_28_MASK,
		CSL_SYSCFG_PINMUX1_PINMUX1_31_28_SHIFT,
		CSL_SYSCFG_PINMUX1_PINMUX1_31_28_AXR0_8,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		0*16+3,     
		"AXR11 /  McBSP1_FSX  / GPIO0[3]",  //AXR11
		SYSCFG0_PINMUX1, 
		CSL_SYSCFG_PINMUX1_PINMUX1_19_16_MASK,
		CSL_SYSCFG_PINMUX1_PINMUX1_19_16_SHIFT,
		CSL_SYSCFG_PINMUX1_PINMUX1_19_16_AXR0_11,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		0*16+2,     
		"AXR10 / McBSP1_DR / GPIO0[2]",  //AXR10
		SYSCFG0_PINMUX1, 
		CSL_SYSCFG_PINMUX1_PINMUX1_23_20_MASK,
		CSL_SYSCFG_PINMUX1_PINMUX1_23_20_SHIFT,
		CSL_SYSCFG_PINMUX1_PINMUX1_23_20_AXR0_10,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		0*16+1,     
		"AXR9  / McB SP1 _DX  / GPIO0[1]",  //EUART_RX
		SYSCFG0_PINMUX1, 
		CSL_SYSCFG_PINMUX1_PINMUX1_27_24_MASK,
		CSL_SYSCFG_PINMUX1_PINMUX1_27_24_SHIFT,
		CSL_SYSCFG_PINMUX1_PINMUX1_27_24_AXR0_9,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	// am1808 南方电力核心板子 AXR end
};

struct st_am1808_gpio am1808_mux_pwm[] = 
{
	{
		M_GPIO_NUM_NOIO_FUNC,     
		"SPI0_ENAn / EPWM0_B / EMAC_MII_RXDV",    // PWM_IR_38KHZ
		SYSCFG0_PINMUX3, 
		CSL_SYSCFG_PINMUX3_PINMUX3_7_4_MASK,
		CSL_SYSCFG_PINMUX3_PINMUX3_7_4_SHIFT,
		CSL_SYSCFG_PINMUX3_PINMUX3_7_4_EPWM0B,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		0*16+7,     
		"AXR15 / EPWMN0_TZ[0] / ECAP2_APWM2 / GPIO0[7]",  //E_CLK
		SYSCFG0_PINMUX1, 
		CSL_SYSCFG_PINMUX1_PINMUX1_3_0_MASK,
		CSL_SYSCFG_PINMUX1_PINMUX1_3_0_SHIFT,
		CSL_SYSCFG_PINMUX1_PINMUX1_3_0_ECAP2,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
};

struct st_am1808_gpio am1808_mux_spi0[] = 
{
	{
		1*16+8,     
		"SPI0_CLK / EPWM0_A / GPIO1[8] / EMAC_MII_RXCLK",  
		SYSCFG0_PINMUX3, 
		CSL_SYSCFG_PINMUX3_PINMUX3_3_0_MASK,
		CSL_SYSCFG_PINMUX3_PINMUX3_3_0_SHIFT,
		CSL_SYSCFG_PINMUX3_PINMUX3_3_0_SPI0_CLK,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		8*16+6,     
		"SPI0_SOMI / EPWM0_SYNCI / GPIO8[6] / EMAC_MII_RXER",  
		SYSCFG0_PINMUX3, 
		CSL_SYSCFG_PINMUX3_PINMUX3_11_8_MASK,
		CSL_SYSCFG_PINMUX3_PINMUX3_11_8_SHIFT,
		CSL_SYSCFG_PINMUX3_PINMUX3_11_8_SPI0_SOMI0,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		8*16+5,     
		"SPI0_SIMO / EPWM0_SYNC / GPIO8[5] / EMAC_MII_CRS",  
		SYSCFG0_PINMUX3, 
		CSL_SYSCFG_PINMUX3_PINMUX3_15_12_MASK,
		CSL_SYSCFG_PINMUX3_PINMUX3_15_12_SHIFT,
		CSL_SYSCFG_PINMUX3_PINMUX3_15_12_SPI0_SIMO0,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		8*16+2,     
		"SPI0_SCSn_3 / UART0_CTSn / GPIO8[2] / EMAC_MII_RXD[1] / SATA_MP_SWITCH",  
		SYSCFG0_PINMUX3, 
		CSL_SYSCFG_PINMUX3_PINMUX3_27_24_MASK,
		CSL_SYSCFG_PINMUX3_PINMUX3_27_24_SHIFT,
		CSL_SYSCFG_PINMUX3_PINMUX3_27_24_NSPI0_SCS3,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	{
		8*16+1,     
		"SPI0_SCSn_2 / UART0_RTSn / GPIO8[1] / EMAC_MII_RXD[0] / SATA_CP_DET",  
		SYSCFG0_PINMUX3, 
		CSL_SYSCFG_PINMUX3_PINMUX3_31_28_MASK,
		CSL_SYSCFG_PINMUX3_PINMUX3_31_28_SHIFT,
		CSL_SYSCFG_PINMUX3_PINMUX3_31_28_NSPI0_SCS2,
		M_GPIO_FUNCTION,
		M_GPIO_OUTPUT_UNKNOW
	},
	
};

int fn_am1808_all_gpio_init(struct st_am1808_gpio *p_st,int size)
{
	int gpio_cnt;	
	int i;
	unsigned long TmpRegVal;
	int status;

	//gpio_cnt = sizeof((**p_st))/sizeof(struct st_am1808_gpio);	
	gpio_cnt = size;
	printk("am1808-mux count = %d\n",gpio_cnt);
	
	for(i = 0; i < gpio_cnt; i++)
	{
		TmpRegVal  = __raw_readl((*p_st).syscfg0_pinmux);
		TmpRegVal &= ~((*p_st).syscfg0_pinmux_mask);
		TmpRegVal |=  ((*p_st).syscfg0_pinmux_function << (*p_st).syscfg0_pinmux_shift);
		__raw_writel(TmpRegVal,(*p_st).syscfg0_pinmux);
		
		if((*p_st).gpio_num != M_GPIO_NUM_NOIO_FUNC)
		{
			gpio_free((*p_st).gpio_num);
		}		
		
		if((*p_st).gpio_direction != M_GPIO_FUNCTION)	
		{
				status = gpio_request((*p_st).gpio_num, (*p_st).gpio_all_function);
				if (status < 0) {
					printk("am1808-mux:error can not request GPIO %s\n", (*p_st).gpio_all_function);
					return status;
				}
		
				if((*p_st).gpio_direction == M_GPIO_OUTPUT)
				{
					gpio_direction_output((*p_st).gpio_num,(*p_st).gpio_output_state);
					printk("am1808-mux:configure %s as gpio output,output state is %d\n",
						(*p_st).gpio_all_function,(*p_st).gpio_output_state); 
				}
				else if((*p_st).gpio_direction == M_GPIO_INPUT)
				{
					gpio_direction_input((*p_st).gpio_num);
					printk("am1808-mux:configure %s as gpio input\n",
						(*p_st).gpio_all_function); 
				}
				else
				{
					printk("am1808-mux:error direction error\n");
					return -1;
				}
		}	
		else if((*p_st).gpio_direction == M_GPIO_FUNCTION)	
		{
			printk("am1808-mux:configure %s as other function\n",
						(*p_st).gpio_all_function); 
		}  
		p_st++;
	}
	return 0;
}

int fn_am1808_all_gpio_exit(struct st_am1808_gpio *p_st,int size)
{
	int gpio_cnt;	
	int i;

	//gpio_cnt = sizeof((**p_st))/sizeof(struct st_am1808_gpio);	
	gpio_cnt = size;
	printk("am1808-mux count = %d\n",gpio_cnt);
	
	for(i = 0; i < gpio_cnt; i++)
	{
		if((*p_st).gpio_num != M_GPIO_NUM_NOIO_FUNC)
		{
			gpio_free((*p_st).gpio_num);
		}	
		p_st++;
	}
}

// ioctl 函数的实现 
// 在应用用户层将通过 ioctl 函数向内核传递参数，以控制 LED的输出状态 
static int am1808_gpio_ioctl( 
 struct inode *inode,  
 struct file *file,   
 unsigned int cmd,  
 unsigned long arg) 
{ 
	
   	switch (cmd)
   	{	
		case SET_MUX_IIC0:
		fn_am1808_all_gpio_init(am1808_mux_iic0,sizeof(am1808_mux_iic0)/sizeof(struct st_am1808_gpio));
		break;
		case SET_MUX_UART0:
		fn_am1808_all_gpio_init(am1808_mux_uart0,sizeof(am1808_mux_uart0)/sizeof(struct st_am1808_gpio));
		break;
		case SET_MUX_UART1:
		fn_am1808_all_gpio_init(am1808_mux_uart1,sizeof(am1808_mux_uart1)/sizeof(struct st_am1808_gpio));
		break;
		case SET_MUX_UART2:
		fn_am1808_all_gpio_init(am1808_mux_uart2,sizeof(am1808_mux_uart2)/sizeof(struct st_am1808_gpio));
		break;
		case SET_MUX_PRU_UART:
		fn_am1808_all_gpio_init(am1808_mux_pru_uart,sizeof(am1808_mux_pru_uart)/sizeof(struct st_am1808_gpio));
		break;
		case SET_MUX_PWM:
		fn_am1808_all_gpio_init(am1808_mux_pwm,sizeof(am1808_mux_pwm)/sizeof(struct st_am1808_gpio));
		break;
		case SET_MUX_SPI0:
		fn_am1808_all_gpio_init(am1808_mux_spi0,sizeof(am1808_mux_spi0)/sizeof(struct st_am1808_gpio));
		break;
		default:
		printk("am1808-mux:ioctl cmd error\n");
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
	int ret;
	//fn_am1808_all_gpio_init(am1808_all_gpio);
	fn_am1808_all_gpio_init(am1808_mux_iic0,sizeof(am1808_mux_iic0)/sizeof(struct st_am1808_gpio));
		
	fn_am1808_all_gpio_init(am1808_mux_uart0,sizeof(am1808_mux_uart0)/sizeof(struct st_am1808_gpio));

	fn_am1808_all_gpio_init(am1808_mux_uart1,sizeof(am1808_mux_uart1)/sizeof(struct st_am1808_gpio));

	//fn_am1808_all_gpio_init(am1808_mux_uart2,sizeof(am1808_mux_uart2)/sizeof(struct st_am1808_gpio));

	fn_am1808_all_gpio_init(am1808_mux_pru_uart,sizeof(am1808_mux_pru_uart)/sizeof(struct st_am1808_gpio));

	fn_am1808_all_gpio_init(am1808_mux_pwm,sizeof(am1808_mux_pwm)/sizeof(struct st_am1808_gpio));

	fn_am1808_all_gpio_init(am1808_mux_spi0,sizeof(am1808_mux_spi0)/sizeof(struct st_am1808_gpio));

   	ret = misc_register(&misc); //注册设备 
   	printk (DEVICE_NAME"\tinitialized\n"); //打印初始化信息 
   	return ret; 
} 
 
static void __exit dev_exit(void) 
{ 
	//fn_am1808_all_gpio_exit(am1808_all_gpio);
	fn_am1808_all_gpio_exit(am1808_mux_iic0,sizeof(am1808_mux_iic0)/sizeof(struct st_am1808_gpio));
		
	fn_am1808_all_gpio_exit(am1808_mux_uart0,sizeof(am1808_mux_uart0)/sizeof(struct st_am1808_gpio));

	fn_am1808_all_gpio_exit(am1808_mux_uart1,sizeof(am1808_mux_uart1)/sizeof(struct st_am1808_gpio));

	//fn_am1808_all_gpio_exit(am1808_mux_uart2,sizeof(am1808_mux_uart2)/sizeof(struct st_am1808_gpio));

	fn_am1808_all_gpio_exit(am1808_mux_pru_uart,sizeof(am1808_mux_pru_uart)/sizeof(struct st_am1808_gpio));

	fn_am1808_all_gpio_exit(am1808_mux_pwm,sizeof(am1808_mux_pwm)/sizeof(struct st_am1808_gpio));

	fn_am1808_all_gpio_exit(am1808_mux_spi0,sizeof(am1808_mux_spi0)/sizeof(struct st_am1808_gpio));
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
