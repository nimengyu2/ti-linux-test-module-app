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

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/crc32.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/dm9000.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/irq.h>

#include <asm/delay.h>
#include <asm/irq.h>
#include <asm/io.h>

#define DEVICE_NAME "busiow" //设备名(/dev/lcd) 

#define MOTOR_MAGIC 'w'
#define SET_BUS_IO_W_NPLC_RST	_IOW(MOTOR_MAGIC, 0,int)
#define SET_BUS_IO_W_PLC_SET	_IOW(MOTOR_MAGIC, 1,int)
#define SET_BUS_IO_W_NM_PCTRL	_IOW(MOTOR_MAGIC, 2,int)
#define SET_BUS_IO_W_M_IGT		_IOW(MOTOR_MAGIC, 3,int)
#define SET_BUS_IO_W_NM_RST		_IOW(MOTOR_MAGIC, 4,int)
#define SET_BUS_IO_W_ALARM_LED	_IOW(MOTOR_MAGIC, 5,int)
#define SET_BUS_IO_W_BATC		_IOW(MOTOR_MAGIC, 6,int)
#define SET_BUS_IO_W_ESAM_RESET	_IOW(MOTOR_MAGIC, 7,int)


unsigned char g_u8_busio_w_value = 0xff; 
void __iomem *busio_write_addr;

void fn_delay_ms(unsigned int ms)
{
    //sleep(ms);
    mdelay(ms);
}

void fn_delay_us(unsigned int us)
{
    //sleep(ms);
    udelay(us);
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_lcd_write_cmd 
// 函数功能  :  写一个字节命令
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
static void fn_busio_write(unsigned char data)
{
	writeb(data, busio_write_addr);
	//fn_delay_us(2);
}

 
// ioctl 函数的实现 
// 在应用用户层将通过 ioctl 函数向内核传递参数，以控制 LED的输出状态 
static int am1808_busiow_ioctl( 
 struct inode *inode,  
 struct file *file,   
 unsigned int cmd,  
 unsigned long arg) 
{ 
   
	printk("busiow:arg = %d\n",(int)arg);	
	switch (cmd)
   	{
       	case SET_BUS_IO_W_NPLC_RST:
       	if(arg == 0)
		{
			g_u8_busio_w_value &= (~(1 << 0));			
		}
		else
		{
			g_u8_busio_w_value |= (1 << 0);
		}
		fn_busio_write(g_u8_busio_w_value);
       	break;

		case SET_BUS_IO_W_PLC_SET:
       	if(arg == 0)
		{
			g_u8_busio_w_value &= (~(1 << 1));			
		}
		else
		{
			g_u8_busio_w_value |= (1 << 1);
		}
		fn_busio_write(g_u8_busio_w_value);
       	break;

		case SET_BUS_IO_W_NM_PCTRL:
       	if(arg == 0)
		{
			g_u8_busio_w_value &= (~(1 << 2));			
		}
		else
		{
			g_u8_busio_w_value |= (1 << 2);
		}
		fn_busio_write(g_u8_busio_w_value);
       	break;

		case  SET_BUS_IO_W_M_IGT:
       	if(arg == 0)
		{
			g_u8_busio_w_value &= (~(1 << 3));			
		}
		else
		{
			g_u8_busio_w_value |= (1 << 3);
		}
		fn_busio_write(g_u8_busio_w_value);
       	break;

		case  SET_BUS_IO_W_NM_RST:
       	if(arg == 0)
		{
			g_u8_busio_w_value &= (~(1 << 4));			
		}
		else
		{
			g_u8_busio_w_value |= (1 << 4);
		}
		fn_busio_write(g_u8_busio_w_value);
       	break;

		case SET_BUS_IO_W_ALARM_LED:
       	if(arg == 0)
		{
			g_u8_busio_w_value &= (~(1 << 5));			
		}
		else
		{
			g_u8_busio_w_value |= (1 << 5);
		}
		fn_busio_write(g_u8_busio_w_value);
       	break;

		case  SET_BUS_IO_W_BATC:
       	if(arg == 0)
		{
			g_u8_busio_w_value &= (~(1 << 6));			
		}
		else
		{
			g_u8_busio_w_value |= (1 << 6);
		}
		fn_busio_write(g_u8_busio_w_value);
       	break;

		case  SET_BUS_IO_W_ESAM_RESET:
       	if(arg == 0)
		{
			g_u8_busio_w_value &= (~(1 << 7));			
		}
		else
		{
			g_u8_busio_w_value |= (1 << 7);
		}
		fn_busio_write(g_u8_busio_w_value);
       	break;

      	default:
       	printk("busiow:cmd error\n");
       	return -EINVAL; 
   } 
   return 0;       
} 
 
 
//  设备函数操作集，在此只有 ioctl函数，通常还有 read, write, open, close 等，因为本 LED驱动在下面已经
//  注册为 misc 设备，因此也可以不用 open/close  
static struct file_operations dev_fops = { 
 .owner = THIS_MODULE, 
 .ioctl = am1808_busiow_ioctl, 
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
    //int i;  
    //int status;

	busio_write_addr = ((volatile unsigned long)ioremap(0x64000000,4));
	fn_busio_write(g_u8_busio_w_value);

   	ret = misc_register(&misc); //注册设备 
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
MODULE_AUTHOR("Lierda nmy"); 



