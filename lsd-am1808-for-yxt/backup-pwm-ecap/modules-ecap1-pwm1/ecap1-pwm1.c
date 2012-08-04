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

#define DEVICE_NAME "pwm2" //设备名(/dev/pwm) 

#define MOTOR_MAGIC 'E'
#define SET_PWM2_ON	_IOW(MOTOR_MAGIC, 2,int)
#define SET_PWM2_OFF 	_IOW(MOTOR_MAGIC, 3,int)

struct ecap
{
	unsigned long freq;
	unsigned long duty;	// 0 - 1000
	unsigned long tick_hz;  // 时钟	
	unsigned long period_ticks;  // 周期数
	unsigned long duty_ticks; // 占空比数
	void __iomem	*mmio_base;
};

struct ecap ecap2_config;

// 时间戳计数器
#define TIMER_CTR_REG			0x0
// 捕获寄存器2
#define CAPTURE_2_REG			0x0c
// 捕获寄存器3
#define CAPTURE_3_REG			0x10
// 捕获寄存器4
#define CAPTURE_4_REG			0x14
#define CAPTURE_CTRL2_REG		0x2A

#define ECTRL2_SYNCOSEL_MASK		(0x03 << 6)

// 模式选择为 1为APWM模式 0为捕获模式
#define ECTRL2_MDSL_ECAP		BIT(9)
// 时间戳计数器停止或者运行
#define ECTRL2_CTRSTP_FREERUN		BIT(4)
// 输出极性控制
#define ECTRL2_PLSL_LOW			BIT(10)
// 计数器同步使能
#define ECTRL2_SYNC_EN			BIT(5)

int fn_ecap_stop(void)
{
	unsigned short reg_val;
	reg_val = __raw_readw(ecap2_config.mmio_base + CAPTURE_CTRL2_REG);
	__raw_writew( reg_val & (~BIT(4)), ecap2_config.mmio_base + CAPTURE_CTRL2_REG);

	reg_val = __raw_readw(ecap2_config.mmio_base + CAPTURE_CTRL2_REG);
	printk("%s:CAPTURE_CTRL2_REG=0x%08x\n",__FUNCTION__,reg_val);
	return 0;
}

int fn_ecap_run(void)
{
	unsigned short reg_val;
	reg_val = __raw_readw(ecap2_config.mmio_base + CAPTURE_CTRL2_REG);
	__raw_writew( reg_val | BIT(4), ecap2_config.mmio_base + CAPTURE_CTRL2_REG);

	reg_val = __raw_readw(ecap2_config.mmio_base + CAPTURE_CTRL2_REG);
	printk("%s:CAPTURE_CTRL2_REG=0x%08x\n",__FUNCTION__,reg_val);
	return 0;
}

int fn_ecap_setpol(int pol)
{
	unsigned short reg_val;
	reg_val = __raw_readw(ecap2_config.mmio_base + CAPTURE_CTRL2_REG);
	reg_val =  ( reg_val & ~BIT(10)) | (!pol << 10);
 	__raw_writew(reg_val, ecap2_config.mmio_base + CAPTURE_CTRL2_REG);
	return 0;
}

int fn_ecap_setperiod(unsigned long period_ticks)
{
	unsigned short reg_val;
	// 设置周期
	// p->period_ticks为周期计数器
	// 设定周期的值
	__raw_writel(period_ticks - 1, ecap2_config.mmio_base + CAPTURE_3_REG);

	reg_val = __raw_readw(ecap2_config.mmio_base + CAPTURE_3_REG);
	printk("%s:CAPTURE_3_REG=0x%08x\n",__FUNCTION__,reg_val);

	// 设置为APWM模式  计数器运行模式
	__raw_writew(ECTRL2_MDSL_ECAP | ECTRL2_SYNCOSEL_MASK |
			 ECTRL2_CTRSTP_FREERUN, ecap2_config.mmio_base + CAPTURE_CTRL2_REG);
	reg_val = __raw_readw(ecap2_config.mmio_base + CAPTURE_CTRL2_REG);
	printk("%s:CAPTURE_CTRL2_REG=0x%08x\n",__FUNCTION__,reg_val);
	return 0;
}

int fn_ecap_setduty(unsigned long duty_ticks)
{
	unsigned short reg_val;
	// 设置占空比
	// 设置为APWM模式  计数器运行模式
	__raw_writew(ECTRL2_MDSL_ECAP | ECTRL2_SYNCOSEL_MASK | 
		ECTRL2_CTRSTP_FREERUN, ecap2_config.mmio_base + CAPTURE_CTRL2_REG);
	reg_val = __raw_readw(ecap2_config.mmio_base + CAPTURE_CTRL2_REG);
	printk("%s:CAPTURE_CTRL2_REG=0x%08x\n",__FUNCTION__,reg_val);

	// 根据占空比大小判断
	if (duty_ticks > 0) {
		// 写占空比数
		__raw_writel(duty_ticks, ecap2_config.mmio_base + CAPTURE_4_REG);
		reg_val = __raw_readw(ecap2_config.mmio_base + CAPTURE_4_REG);
		printk("%s:CAPTURE_4_REG=0x%08x\n",__FUNCTION__,reg_val);
	} else {
		__raw_writel(duty_ticks, ecap2_config.mmio_base + CAPTURE_2_REG);
		__raw_writel(0, ecap2_config.mmio_base + TIMER_CTR_REG);
	}


	return 0;
}


// 经过测试,发现时钟应该就是150MHZ,但是实际通过示波器测试的时候发现
// 38KHZ的时候,频率不是很准,这个是因为示波器在测试频率的时候不是按照
// 计数法测的,而是纯粹看示波器表面的时间间距,不同档位的频率是不一样的
// 后来使用万用表测频率还是比较准确的
#define M_SYSTEM_CLOCK  150000000

// pwm初始化
int fn_pwm_init(void)
{
	printk("ecap2_config.period_ticks=%d\n",ecap2_config.period_ticks);
	printk("ecap2_config.duty_ticks=%d\n",ecap2_config.duty_ticks);
	fn_ecap_setperiod(ecap2_config.period_ticks);
	fn_ecap_setduty(ecap2_config.duty_ticks);
	fn_ecap_run();
	printk("ecap2_apwm2 set output on\n");
	return 1;
}

// PWM输出关闭
void fn_pwm_exit(void)
{
	fn_ecap_stop();	
	printk("ecap2_apwm2 set output off\n");
}

 
// ioctl 函数的实现 
// 在应用用户层将通过 ioctl 函数向内核传递参数，以控制 LED的输出状态 
static int am1808_led_ioctl( 
 struct inode *inode,  
 struct file *file,   
 unsigned int cmd,  
 unsigned long arg) 
{ 
	struct ecap* p_ecap;   
	p_ecap = (struct ecap* )arg;	
	printk("ioctl:p_ecap->freq=%d\n",p_ecap->freq);
	printk("ioctl:p_ecap->duty=%d\n",p_ecap->duty);
	//printk("cmd is %d \n",cmd);
   	switch (cmd)
   	{
       		case SET_PWM2_ON:
		ecap2_config.freq = p_ecap->freq;
		ecap2_config.duty = p_ecap->duty;  // 1-100
		ecap2_config.tick_hz = 150000000;
		ecap2_config.period_ticks = ecap2_config.tick_hz/ecap2_config.freq; 
		ecap2_config.duty_ticks =(ecap2_config.period_ticks*ecap2_config.duty)/100;
       		fn_pwm_init();
      		break;
      		case SET_PWM2_OFF:
       		fn_pwm_exit();
       		break;       
       		default:
      		 printk("cmd may be 2 3 4 5 \n");
       		return -EINVAL; 
  	 } 
  	 return 0;       
} 
 
 
//  设备函数操作集，在此只有 ioctl函数，通常还有 read, write, open, close 等，因为本 LED驱动在下面已经
//  注册为 misc 设备，因此也可以不用 open/close  
static struct file_operations dev_fops = { 
 	.owner = THIS_MODULE, 
 	.ioctl = am1808_led_ioctl, 
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
    	//int status;
	unsigned long TmpRegVal;
	#if 0
    	// 配置这个引脚为为EPWM0B功能
    	status = davinci_cfg_reg(DA850_EPWM1A);
    	if (status < 0) {
		printk("AM1808-PWM pin could not be muxed for GPIO functionality %d\n",DA850_EPWM1A);
		return status;
    	}
	#endif
	// GET_GPIO_GPRS_RI 	 GPIO8[9]

	// 设定引脚功能 EPWM0	
    	//TmpRegVal  = __raw_readl(SYSCFG0_PINMUX3);
	//TmpRegVal &= ~(CSL_SYSCFG_PINMUX3_PINMUX3_7_4_MASK);
	//TmpRegVal |=  (CSL_SYSCFG_PINMUX3_PINMUX3_7_4_EPWM0B << CSL_SYSCFG_PINMUX3_PINMUX3_7_4_SHIFT);
	//__raw_writel(TmpRegVal,SYSCFG0_PINMUX3);		
		
	//g_u32_pwm_output_freq = 38000;
       	//fn_pwm_init();
	//fn_pwm_exit();		
	ecap2_config.mmio_base = DA8XX_ECAP1_BASE;
	ecap2_config.freq = 38000;
	ecap2_config.duty = 50;  // 1-100
	ecap2_config.tick_hz = 150000000;
	ecap2_config.period_ticks = ecap2_config.tick_hz/ecap2_config.freq; 
	ecap2_config.duty_ticks =(ecap2_config.period_ticks*ecap2_config.duty)/100;
	//fn_pwm_init();

    	// 注册设备 
    	ret = misc_register(&misc); 
    	printk (DEVICE_NAME"\tinitialized\n"); //打印初始化信息 
    	return ret; 
} 
 
static void __exit dev_exit(void) 
{ 
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
MODULE_AUTHOR("LSD-ARM team nmy ."); 
