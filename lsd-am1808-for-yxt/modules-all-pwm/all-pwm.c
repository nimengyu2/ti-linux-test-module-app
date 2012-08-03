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

#define DEVICE_NAME "all-pwm" //设备名(/dev/pwm) 

#define MOTOR_MAGIC 'P'
#define PWM1_IOCTL_SET	 _IOW(MOTOR_MAGIC, 2,int)
#define PWM1_IOCTL_STOP	 _IOW(MOTOR_MAGIC, 3,int)


struct pwm_config
{
        unsigned long  pwm_num;//通道号
	unsigned long  pwm_ctrl;//模式
	unsigned long  pwm_div;//分频比
	unsigned long  pwm_period;//周期  计数值
	unsigned long  pwm_data;//数据  占空比
} ;


int fn_pwm0_set(unsigned long  pwm_period,unsigned long  pwm_data)
{
	unsigned short u16_tmp;
	//int i;
	//clk_enable(&pwm1_clk);	

	// 开启PWM同步	
	u16_tmp = __raw_readw(SYSCFG0_CFGCHIP1);
	u16_tmp &= (~SYSCFG0_CFGCHIP1_TBCLKSYNC);
	__raw_writew(u16_tmp, SYSCFG0_CFGCHIP1);	

	// 写入周期值
	u16_tmp = pwm_period;
	__raw_writew(u16_tmp, EHRPWM1_TBPRD);	
	//printk("pwm1b set period = %d \n",u16_tmp);

	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM1_TBPHSHR);
	
	// 增计数  分频 128*14	PRDLD=1  PHSEN=0  CTRMODE=0
	// Time-Base Control Register
	//u16_tmp = (0x07 << 10) | (0x07 << 7) | (3 << 4);    
	// 不分频	
	u16_tmp = (3 << 4);  
	__raw_writew(u16_tmp, EHRPWM1_TBCTL);
	
	// 比较控制器 SHDWBMODE=1 SHDWAMODE=1
	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM1_CMPCTL);

	// 输出控制A  CAU=2  PRD=1  ZRO = 0
	u16_tmp = (0x02 << 8) | (0x01 << 2) | (0x00);    
	__raw_writew(u16_tmp, EHRPWM1_AQCTLA);

	// 输出控制B  CBU=2  PRD=1  ZRO = 0
	u16_tmp = (0x02 << 4) | (0x01 << 2) | (0x00);    
	__raw_writew(u16_tmp, EHRPWM1_AQCTLB);

	// 比较值
	u16_tmp = pwm_data;
	__raw_writew(u16_tmp, EHRPWM1_CMPA);
	// 比较值
	u16_tmp = pwm_data;
	__raw_writew(u16_tmp, EHRPWM1_CMPB);

	// 计数值为0
	// Time-Base Counter Register
	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM1_TBCNT);

	// 死区控制直接输出
	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM1_DBCTL);

	// PC short 旁路
	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM1_PCCTL);

	//  死区时间
	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM1_TZSEL);

	u16_tmp = (1 << 2) | (1);    
	__raw_writew(u16_tmp, EHRPWM1_TZCTL);

	// 使能时钟	
	//clk_enable(&pwm1_clk);	

	//  使能TB时钟
	u16_tmp = __raw_readw(SYSCFG0_CFGCHIP1);
	u16_tmp |= (SYSCFG0_CFGCHIP1_TBCLKSYNC);
	__raw_writew(u16_tmp, SYSCFG0_CFGCHIP1);	

	printk("pwm0(pwm1a) set output on\n");
	return 0;
}

int fn_pwm0_stop(unsigned long  pwm_period,unsigned long  pwm_data)
{
	unsigned short u16_tmp;	
	u16_tmp = __raw_readw(SYSCFG0_CFGCHIP1);
	u16_tmp &= (~SYSCFG0_CFGCHIP1_TBCLKSYNC);
	__raw_writew(u16_tmp, SYSCFG0_CFGCHIP1);		
	printk("pwm0(pwm1a) set output off\n");
	return 0;
}


int fn_pwm1_set(unsigned long  pwm_period,unsigned long  pwm_data)
{
	unsigned short u16_tmp;
	//int i;
	//unsigned long u32_period;
	//clk_enable(&pwm1_clk);	

	// 开启PWM同步	
	u16_tmp = __raw_readw(SYSCFG0_CFGCHIP1);
	u16_tmp &= (~SYSCFG0_CFGCHIP1_TBCLKSYNC);
	__raw_writew(u16_tmp, SYSCFG0_CFGCHIP1);	

	
	// 写入周期值
	u16_tmp = pwm_period;
	__raw_writew(u16_tmp, EHRPWM0_TBPRD);	
	//printk("pwm1b set period = %d \n",u16_tmp);

	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM0_TBPHSHR);
	
	// 增计数  分频 128*14	PRDLD=1  PHSEN=0  CTRMODE=0
	// Time-Base Control Register
	//u16_tmp = (0x07 << 10) | (0x07 << 7) | (3 << 4);    
	// 不分频	
	u16_tmp = (3 << 4);  
	__raw_writew(u16_tmp, EHRPWM0_TBCTL);
	
	// 比较控制器 SHDWBMODE=1 SHDWAMODE=1
	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM0_CMPCTL);

	// 输出控制A  CAU=2  PRD=1  ZRO = 0
	u16_tmp = (0x02 << 8) | (0x01 << 2) | (0x00);    
	__raw_writew(u16_tmp, EHRPWM0_AQCTLA);

	// 输出控制B  CBU=2  PRD=1  ZRO = 0
	u16_tmp = (0x02 << 4) | (0x01 << 2) | (0x00);    
	__raw_writew(u16_tmp, EHRPWM0_AQCTLB);

	// 比较值
	u16_tmp = pwm_data;
	__raw_writew(u16_tmp, EHRPWM0_CMPA);
	// 比较值
	u16_tmp = pwm_data;
	__raw_writew(u16_tmp, EHRPWM0_CMPB);

	// 计数值为0
	// Time-Base Counter Register
	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM0_TBCNT);

	// 死区控制直接输出
	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM0_DBCTL);

	// PC short 旁路
	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM0_PCCTL);

	//  死区时间
	u16_tmp = 0;    
	__raw_writew(u16_tmp, EHRPWM0_TZSEL);

	u16_tmp = (1 << 2) | (1);    
	__raw_writew(u16_tmp, EHRPWM0_TZCTL);

	// 使能时钟	
	//clk_enable(&pwm1_clk);	

	//  使能TB时钟
	u16_tmp = __raw_readw(SYSCFG0_CFGCHIP1);
	u16_tmp |= (SYSCFG0_CFGCHIP1_TBCLKSYNC);
	__raw_writew(u16_tmp, SYSCFG0_CFGCHIP1);	

	printk("pwm1(pwm0b) set output on\n");

}

int fn_pwm1_stop(unsigned long  pwm_period,unsigned long  pwm_data)
{
	unsigned short u16_tmp;	
	u16_tmp = __raw_readw(SYSCFG0_CFGCHIP1);
	u16_tmp &= (~SYSCFG0_CFGCHIP1_TBCLKSYNC);
	__raw_writew(u16_tmp, SYSCFG0_CFGCHIP1);		
	printk("pwm1(pwm0b) set output off\n");
}

//--------------------------------------------------------------------------//
struct ecap
{
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
//--------------------------------------------------------------------------//

int fn_pwm2_set(unsigned long  pwm_period,unsigned long  pwm_data)
{
	ecap2_config.mmio_base = DA8XX_ECAP1_BASE;
	ecap2_config.period_ticks = pwm_period; 
	ecap2_config.duty_ticks = pwm_data;
	printk("ecap_config.period_ticks=%d\n",ecap2_config.period_ticks);
	printk("ecap_config.duty_ticks=%d\n",ecap2_config.duty_ticks);
	fn_ecap_setperiod(ecap2_config.period_ticks);
	fn_ecap_setduty(ecap2_config.duty_ticks);
	fn_ecap_run();
	printk("pwm2(ecap1_apwm1) set output on\n");
}

int fn_pwm2_stop(unsigned long  pwm_period,unsigned long  pwm_data)
{
	ecap2_config.mmio_base = DA8XX_ECAP1_BASE;
	ecap2_config.period_ticks = pwm_period; 
	ecap2_config.duty_ticks = pwm_data;
	fn_ecap_stop();	
	printk("pwm2(ecap1_apwm1) set output off\n");
}

int fn_pwm3_set(unsigned long  pwm_period,unsigned long  pwm_data)
{
	ecap2_config.mmio_base = DA8XX_ECAP2_BASE;
	ecap2_config.period_ticks = pwm_period; 
	ecap2_config.duty_ticks = pwm_data;
	printk("ecap_config.period_ticks=%d\n",ecap2_config.period_ticks);
	printk("ecap_config.duty_ticks=%d\n",ecap2_config.duty_ticks);
	fn_ecap_setperiod(ecap2_config.period_ticks);
	fn_ecap_setduty(ecap2_config.duty_ticks);
	fn_ecap_run();
	printk("pwm3(ecap2_apwm2) set output on\n");
}

int fn_pwm3_stop(unsigned long  pwm_period,unsigned long  pwm_data)
{
	ecap2_config.mmio_base = DA8XX_ECAP2_BASE;
	ecap2_config.period_ticks = pwm_period; 
	ecap2_config.duty_ticks = pwm_data;
	fn_ecap_stop();	
	printk("pwm3(ecap2_apwm2) set output off\n");
}


void PWM1_Set_Freq( unsigned long freq )
{
	unsigned long temp;
        unsigned long offset;
	struct pwm_config    *pwm_ptr;
	pwm_ptr = (struct pwm_config * )freq;
	if(pwm_ptr == NULL)
	{
		printk("pwm_ptr is null,exit\n");
		return 1;
	}
	printk("pwm_ptr->pwm_num=%d\n",pwm_ptr->pwm_num);
	printk("pwm_ptr->pwm_period=%d,pwm_ptr->pwm_data=%d\n",pwm_ptr->pwm_period,pwm_ptr->pwm_data);
	switch (pwm_ptr->pwm_num) 
	{
		case 0:
		fn_pwm0_set(pwm_ptr->pwm_period,pwm_ptr->pwm_data);
		break;
		case 1:
		fn_pwm1_set(pwm_ptr->pwm_period,pwm_ptr->pwm_data);
		break;
		case 2:
		fn_pwm2_set(pwm_ptr->pwm_period,pwm_ptr->pwm_data);
		break;
		case 3:
		fn_pwm3_set(pwm_ptr->pwm_period,pwm_ptr->pwm_data);
		break;
		default:
		printk("pwm_ptr->pwm_num may be 0123\n");
		break;
	}	
}

void PWM1_Stop(unsigned long freq)
{
	unsigned long temp;
        unsigned long offset;
	struct pwm_config    *pwm_ptr;
	pwm_ptr = (struct pwm_config * )freq;
	if(pwm_ptr == NULL)
	{
		printk("pwm_ptr is null,exit\n");
		return 1;
	}
	printk("pwm_ptr->pwm_num=%d\n",pwm_ptr->pwm_num);
	switch (pwm_ptr->pwm_num) 
	{
		case 0:
		fn_pwm0_stop(pwm_ptr->pwm_period,pwm_ptr->pwm_data);
		break;
		case 1:
		fn_pwm1_stop(pwm_ptr->pwm_period,pwm_ptr->pwm_data);
		break;
		case 2:
		fn_pwm2_stop(pwm_ptr->pwm_period,pwm_ptr->pwm_data);
		break;
		case 3:
		fn_pwm3_stop(pwm_ptr->pwm_period,pwm_ptr->pwm_data);
		break;
		default:
		printk("pwm_ptr->pwm_num may be 0123\n");
		break;
	}	
}

 
 
// ioctl 函数的实现 
// 在应用用户层将通过 ioctl 函数向内核传递参数，以控制 LED的输出状态 
static int am1808_led_ioctl( 
 struct inode *inode,  
 struct file *file,   
 unsigned int cmd,  
 unsigned long arg) 
{ 
	switch (cmd) {
		case PWM1_IOCTL_SET:
		printk("cmd=PWM1_IOCTL_SET\n");
		PWM1_Set_Freq(arg);
		break;

		case PWM1_IOCTL_STOP:
		printk("cmd=PWM1_IOCTL_STOP\n");
		PWM1_Stop(arg);
		break;
			
		default:
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
