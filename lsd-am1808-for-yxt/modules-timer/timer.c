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
#include <mach/irqs.h>
#include <linux/clk.h>
#include "cslr_syscfg01_am1808.h"

#define DEVICE_NAME "timer" //设备名(/dev/pwm) 

#define MOTOR_MAGIC 'T'
#define	IOCTL_TIMERINIT  _IOW(MOTOR_MAGIC, 1,int)
#define IOCTL_TIMERSET	 _IOW(MOTOR_MAGIC, 2,int)
#define IOCTL_STATEGATE  _IOW(MOTOR_MAGIC, 3,int)
#define IOCTL_TIMEDEL 	 _IOW(MOTOR_MAGIC, 4,int)

#define TIMER1    1    // Timer2 TM12
#define TIMER2    2    // Timer2 TM34
#define TIMER3    3    // Timer3 TM12
//#define SYS_TIMER     4
#define TIMER4     4    // Timer3 TM34

#define TMR_TCR_CLKSRC12        (0x00000100u)
#define TMR_TCR_CLKSRC34        (0x01000000u)
/* 64 Bit mode with internal clock source */
#define TMR_CFG_64BIT_CLK_INT              (0x00000013u)

#define TMR_TGCR_TIMMODE        (0x0000000Cu)  // 1100
#define TMR_TGCR_TIMMODE_DUAL_UNCHAIN        (0x00000004u)  // 0100
#define TMR_TGCR_TIM34RS        (0x00000002u)
#define TMR_TGCR_TIM12RS        (0x00000001u)
#define TMR_TGCR_PLUSEN         (0x00000010u)


#define TMR_INTCTLSTAT_PRDINTEN12 (0x00000001u)
#define TMR_INTCTLSTAT_PRDINTEN34 (0x00010000u)
#define TMR_INTCTLSTAT_PRDINTSTAT12 (0x00000002u)
#define TMR_INTCTLSTAT_PRDINTSTAT34 (0x00020000u)

#define TMR_DISABLE                    	   (0x00000000u)  // 10 连续模式
#define TMR_ENABLE_ONCE                    (0x00400040u)   // 01 单次模式
#define TMR_ENABLE_CONT                    (0x00800080u)  // 10 连续模式

#define TMR_TIMER12                        (0x00003FFEu) /* Timer12 */
#define TMR_TIMER34                        (0x3FFE0000u) /* Timer34 */
#define TMR_TCR_ENAMODE12       (0x000000C0u)
#define TMR_TCR_ENAMODE34       (0x00C00000u)


typedef struct
{
    int id;//TIMER号
    unsigned int data;//TIMER初始值
    int timer_flag;
} sep_timer;

unsigned char timer1_flag = 0;
unsigned char timer2_flag = 0;
unsigned char timer3_flag = 0;
#define TIMER_Uint    150    /* system clock is 150MHZ */
static int sys_clock_count=0;


static int disable_timer(int timer)
{
    unsigned long reg_val;
    switch (timer)
    {
    case 1:   // Timer2 TM12
        // 停止定时器
	reg_val = __raw_readl(TIMER64P2_TCR);
	reg_val &= ~(TMR_TCR_ENAMODE12);
	__raw_writel(reg_val,TIMER64P2_TCR);
        break;
    case 2:   // Timer2 TM34
        // 停止定时器
	reg_val = __raw_readl(TIMER64P2_TCR);
	reg_val &= ~(TMR_TCR_ENAMODE34);
	__raw_writel(reg_val,TIMER64P2_TCR);
        break;
    case 3:  // Timer3 TM12
        // 停止定时器
	reg_val = __raw_readl(TIMER64P3_TCR);
	reg_val &= ~(TMR_TCR_ENAMODE12);
	__raw_writel(reg_val,TIMER64P3_TCR);
        break;
    case 4: // Timer3 TM34
        // 停止定时器
	reg_val = __raw_readl(TIMER64P3_TCR);
	reg_val &= ~(TMR_TCR_ENAMODE34);
	__raw_writel(reg_val,TIMER64P3_TCR);
        break;
    default:
        break;
    }
    return 0;
}
static int enable_timer(int timer)
{
    unsigned long reg_val;
    switch (timer)
    {
    case 1:   // Timer2 TM12
	// 启动定时器 设定单次模式
	reg_val = __raw_readl(TIMER64P2_TCR);
	reg_val |=  (TMR_ENABLE_ONCE & (TMR_TIMER12));
	__raw_writel(reg_val,TIMER64P2_TCR);
        break;
    case 2:   // Timer2 TM34
        // 启动定时器 设定单次模式
	reg_val = __raw_readl(TIMER64P2_TCR);
	reg_val |=  (TMR_ENABLE_ONCE & (TMR_TIMER34));
	__raw_writel(reg_val,TIMER64P2_TCR);
        break;
    case 3:  // Timer3 TM12
        // 启动定时器 设定单次模式
	reg_val = __raw_readl(TIMER64P3_TCR);
	reg_val |=  (TMR_ENABLE_ONCE & (TMR_TIMER12));
	__raw_writel(reg_val,TIMER64P3_TCR);
        break;
    case 4: // Timer3 TM34
        // 启动定时器 设定单次模式
	reg_val = __raw_readl(TIMER64P3_TCR);
	reg_val |=  (TMR_ENABLE_ONCE & (TMR_TIMER34));
	__raw_writel(reg_val,TIMER64P3_TCR);
        break;
    default:
        break;
    }
    return 0;
}



//CLK  150MHZ
// 输入参数timer 表示要操作的定时器 1-4,其中定时器4固定是1ms一次
// 输入参数 val 表示定时时间，单位是us
static void set_timer_val(int timer, unsigned int val)
{
    unsigned int timer_val = 0;
    timer_val = val * TIMER_Uint;  // val 表示几个us
    disable_timer(timer);
    switch (timer)  
    {
    case 1:   // Timer2 TM12
        timer1_flag = 0;
	__raw_writel(0,TIMER64P2_TIM12);
        __raw_writel(timer_val,TIMER64P2_PRD12);
        break;
    case 2:  // Timer2 TM34
        timer2_flag = 0;
        __raw_writel(0,TIMER64P2_TIM34);
        __raw_writel(timer_val,TIMER64P2_PRD34);
        break;
    case 3:  // Timer3 TM12
        timer3_flag = 0;
        __raw_writel(0,TIMER64P3_TIM12);
        __raw_writel(timer_val,TIMER64P3_PRD12);
        break;
    case 4:  // Timer3 TM34
	__raw_writel(0,TIMER64P3_TIM34);
        __raw_writel(1000*TIMER_Uint,TIMER64P3_PRD34);
        break;
    default:
        break;
    }
    enable_timer(timer);
}



static irqreturn_t timer2_interrupt(int irq, void *dev_id) 
{
	unsigned long reg_val = 0;
	
	// 清除中断
	reg_val = __raw_readl(TIMER64P2_INTCTLSTAT);
	if(reg_val & TMR_INTCTLSTAT_PRDINTSTAT12)
	{
		// Timer2 TM12
        	timer1_flag = 1;
		printk("timer 1 int\n");
		disable_timer(1);
	}
	if(reg_val & TMR_INTCTLSTAT_PRDINTSTAT34)
	{
		// Timer2 TM34
        	timer2_flag = 1;
		printk("timer 2 int\n");
		disable_timer(2);
	}

	reg_val |= (TMR_INTCTLSTAT_PRDINTEN12 | TMR_INTCTLSTAT_PRDINTSTAT34);
	__raw_writel(reg_val,TIMER64P2_INTCTLSTAT);

	return IRQ_HANDLED;
}

static irqreturn_t timer3_interrupt(int irq, void *dev_id) 
{
	unsigned long reg_val = 0;
	
	// 清除中断
	reg_val = __raw_readl(TIMER64P3_INTCTLSTAT);
	if(reg_val & TMR_INTCTLSTAT_PRDINTSTAT12)
	{
		// Timer3 TM12
        	timer3_flag = 1;
		printk("timer 3 int\n");
		disable_timer(3);
	}
	if(reg_val & TMR_INTCTLSTAT_PRDINTSTAT34)
	{
		// Timer3 TM34
        	sys_clock_count++;
		disable_timer(4);
		set_timer_val(4,1000);
	}

	reg_val |= (TMR_INTCTLSTAT_PRDINTEN12 | TMR_INTCTLSTAT_PRDINTSTAT34);
	__raw_writel(reg_val,TIMER64P3_INTCTLSTAT);

	return IRQ_HANDLED;
}


int sep4020_timer_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    static sep_timer print_timer;
    copy_from_user(&print_timer, (sep_timer *)arg, sizeof(print_timer));
    switch (cmd)
    {
	case IOCTL_TIMERINIT:
	enable_timer(print_timer.id);
	break;
	case IOCTL_TIMERSET:
	set_timer_val(print_timer.id, print_timer.data);
	break;
	case IOCTL_STATEGATE:
	if(print_timer.id == TIMER1)
	{
		print_timer.timer_flag = timer1_flag;
		if(copy_to_user((sep_timer *)arg, &print_timer, sizeof(print_timer)))
		{
			printk("copy_to_user error\n");
			return -EFAULT;
		}		
		if(print_timer.timer_flag == 1) 
			timer1_flag = 0;
	}
	else if(print_timer.id == TIMER2)
	{
		//printk("get irq head\n");
		print_timer.timer_flag = timer2_flag;
		if(copy_to_user((sep_timer *)arg, &print_timer, sizeof(print_timer)))
		{
			printk("copy_to_user error\n");
			return -EFAULT;
		}
		if(print_timer.timer_flag == 1) 
			timer2_flag = 0;
	}
	else if(print_timer.id == TIMER3)
	{
		//printk("get irq pfstep\n");
		print_timer.timer_flag = timer3_flag;
		if(copy_to_user((sep_timer *)arg, &print_timer, sizeof(print_timer)))
		{
			printk("copy_to_user error\n");
			return -EFAULT;
		}
		if(print_timer.timer_flag == 1) 
			timer3_flag = 0;
	}
	else if(print_timer.id == TIMER4)
	{
		print_timer.timer_flag = sys_clock_count;
		if(copy_to_user((sep_timer*)arg, &print_timer, sizeof  (print_timer)))
		{
			printk("copy_to_user error\n");
			return -EFAULT;
		}
	}
	else 
	        return  -EFAULT;   
	break;
	case IOCTL_TIMEDEL:
	{
	    disable_timer(print_timer.id);    
	    timer1_flag = 0;
	    timer2_flag = 0;
	    timer3_flag = 0;
	    if(print_timer.id == TIMER4) 
			sys_clock_count=0;
	}
	break;
	default:
	return -ENOTTY;
    }

    return 0;
}

void fn_timer2_init()
{
	unsigned long reg_val = 0;
	int err;

	// 使用内部时钟
	reg_val = 0;
	__raw_writel(reg_val,TIMER64P2_TCR);

	// 清除定时器计数的值
	__raw_writel(0,TIMER64P2_TIM12);
	__raw_writel(0,TIMER64P2_TIM34);

	// TMR_TGCR_TIM34RS TMR_TGCR_TIM12RS 置1表示设定为复位状态
	// TMR_TGCR_TIMMODE 定时器模式清0
	reg_val = __raw_readl(TIMER64P2_TGCR);
	reg_val = reg_val & (~( TMR_TGCR_TIMMODE | TMR_TGCR_TIM34RS |
                                     TMR_TGCR_TIM12RS));
	__raw_writel(reg_val,TIMER64P2_TGCR);

	// TMR_TGCR_PLUSEN = Disablebackwardcompatibility.Newtimer features areavailable.
	// TMR_TGCR_TIMMODE_DUAL_UNCHAIN  32bit 双个 不连续
	// TMR_TGCR_TIM34RS TMR_TGCR_TIM12RS = is notin reset退出复位
	reg_val = __raw_readl(TIMER64P2_TGCR);
	reg_val |= (TMR_TGCR_TIMMODE_DUAL_UNCHAIN | TMR_TGCR_TIM34RS |
                                    TMR_TGCR_TIM12RS | TMR_TGCR_PLUSEN); 
	__raw_writel(reg_val,TIMER64P2_TGCR);

	// 设定定时周期
	// 经过实际测试，timer的时钟使150MHZ
	__raw_writel(150*1000*1000,TIMER64P2_PRD12);
	__raw_writel(300*1000*1000,TIMER64P2_PRD34);

	// 使能中断
	reg_val = __raw_readl(TIMER64P2_INTCTLSTAT);
	reg_val |= (TMR_INTCTLSTAT_PRDINTEN12 | TMR_INTCTLSTAT_PRDINTEN34);
	__raw_writel(reg_val,TIMER64P2_INTCTLSTAT);

	// 清除定时器模式
	reg_val = __raw_readl(TIMER64P2_TCR);
	reg_val &= ~(TMR_TCR_ENAMODE12 | TMR_TCR_ENAMODE34);
	__raw_writel(reg_val,TIMER64P2_TCR);

	// 注册中断函数 
	err = request_irq(IRQ_DA850_TINT12_2, timer2_interrupt, NULL,  
	          NULL, NULL); 
	if (err)
	{
		printk("request_irq IRQ_DA850_TINT12_2 error\n");
	} 
	else
	{
		printk("request_irq IRQ_DA850_TINT12_2 ok\n");
	}
}

void fn_timer3_init()
{
	unsigned long reg_val = 0;
	int err;

	// 使用内部时钟
	reg_val = 0;
	__raw_writel(reg_val,TIMER64P3_TCR);

	// 清除定时器计数的值
	__raw_writel(0,TIMER64P3_TIM12);
	__raw_writel(0,TIMER64P3_TIM34);

	// TMR_TGCR_TIM34RS TMR_TGCR_TIM12RS 置1表示设定为复位状态
	// TMR_TGCR_TIMMODE 定时器模式清0
	reg_val = __raw_readl(TIMER64P3_TGCR);
	reg_val = reg_val & (~( TMR_TGCR_TIMMODE | TMR_TGCR_TIM34RS |
                                     TMR_TGCR_TIM12RS));
	__raw_writel(reg_val,TIMER64P3_TGCR);

	// TMR_TGCR_PLUSEN = Disablebackwardcompatibility.Newtimer features areavailable.
	// TMR_TGCR_TIMMODE_DUAL_UNCHAIN  32bit 双个 不连续
	// TMR_TGCR_TIM34RS TMR_TGCR_TIM12RS = is notin reset退出复位
	reg_val = __raw_readl(TIMER64P3_TGCR);
	reg_val |= (TMR_TGCR_TIMMODE_DUAL_UNCHAIN | TMR_TGCR_TIM34RS |
                                    TMR_TGCR_TIM12RS | TMR_TGCR_PLUSEN); 
	__raw_writel(reg_val,TIMER64P3_TGCR);

	// 设定定时周期
	// 经过实际测试，timer的时钟使150MHZ
	__raw_writel(150*1000*1000,TIMER64P3_PRD12);
	__raw_writel(300*1000*1000,TIMER64P3_PRD34);

	// 使能中断
	reg_val = __raw_readl(TIMER64P3_INTCTLSTAT);
	reg_val |= (TMR_INTCTLSTAT_PRDINTEN12 | TMR_INTCTLSTAT_PRDINTEN34);
	__raw_writel(reg_val,TIMER64P3_INTCTLSTAT);

	// 清除定时器模式
	reg_val = __raw_readl(TIMER64P3_TCR);
	reg_val &= ~(TMR_TCR_ENAMODE12 | TMR_TCR_ENAMODE34);
	__raw_writel(reg_val,TIMER64P3_TCR);

	// 注册中断函数 
	err = request_irq(IRQ_DA850_TINT12_3, timer3_interrupt, NULL,  
	          NULL, NULL); 
	if (err)
	{
		printk("request_irq IRQ_DA850_TINT12_3 error\n");
	} 
	else
	{
		printk("request_irq IRQ_DA850_TINT12_3 ok\n");
	}
}


void fn_get_timer_status(void)
{
	//unsigned long reg_val = 0;
	printk("TIMER64P3_TIM12=0x%08x\n",__raw_readl(TIMER64P3_TIM12));
	printk("TIMER64P3_TIM34=0x%08x\n",__raw_readl(TIMER64P3_TIM34));
	printk("TIMER64P3_TCR=0x%08x\n",__raw_readl(TIMER64P3_TCR));
	printk("TIMER64P3_TGCR=0x%08x\n",__raw_readl(TIMER64P3_TGCR));
	printk("TIMER64P3_PRD12=0x%08x\n",__raw_readl(TIMER64P3_PRD12));
	printk("TIMER64P3_PRD34=0x%08x\n",__raw_readl(TIMER64P3_PRD34));
	printk("TIMER64P3_INTCTLSTAT=0x%08x\n",__raw_readl(TIMER64P3_INTCTLSTAT));
}

//  设备函数操作集，在此只有 ioctl函数，通常还有 read, write, open, close 等，因为本 LED驱动在下面已经
//  注册为 misc 设备，因此也可以不用 open/close  
static struct file_operations dev_fops = { 
 	.owner = THIS_MODULE, 
 	.ioctl = sep4020_timer_ioctl, 
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
	fn_timer2_init();
	fn_timer3_init();
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
