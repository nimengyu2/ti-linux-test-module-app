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

#define DEVICE_NAME "busior" //设备名(/dev/lcd) 

#define MOTOR_MAGIC 'r'
#define GET_BUS_IO_BTN_UP		_IOW(MOTOR_MAGIC, 0,int)
#define GET_BUS_IO_BTN_DOWN		_IOW(MOTOR_MAGIC, 1,int)
#define GET_BUS_IO_BTN_LEFT		_IOW(MOTOR_MAGIC, 2,int)
#define GET_BUS_IO_BTN_RIGHT	_IOW(MOTOR_MAGIC, 3,int)
#define GET_BUS_IO_BTN_ENTER	_IOW(MOTOR_MAGIC, 4,int)
#define GET_BUS_IO_BTN_CANCEL	_IOW(MOTOR_MAGIC, 5,int)

#define GET_BUS_IO_YX0			_IOW(MOTOR_MAGIC, 6,int)
#define GET_BUS_IO_YX1			_IOW(MOTOR_MAGIC, 7,int)
#define GET_BUS_IO_POWER_DOWN	_IOW(MOTOR_MAGIC, 8,int)
#define GET_BUS_IO_POWER_UP		_IOW(MOTOR_MAGIC, 9,int)

#define GET_BUS_IO_STATE0		_IOW(MOTOR_MAGIC, 10,int)
#define GET_BUS_IO_STATE1		_IOW(MOTOR_MAGIC, 11,int)
#define GET_BUS_IO_STATE2		_IOW(MOTOR_MAGIC, 12,int)
#define GET_BUS_IO_STATE3		_IOW(MOTOR_MAGIC, 13,int)
#define GET_BUS_IO_STATE4		_IOW(MOTOR_MAGIC, 14,int)
#define GET_BUS_IO_BTN_OPEN		_IOW(MOTOR_MAGIC, 15,int)
#define GET_BUS_IO_BTN_PROG		_IOW(MOTOR_MAGIC, 16,int)


void __iomem *busio_read_addr_cs0;
void __iomem *busio_read_addr_cs1;
void __iomem *busio_read_addr_cs2;

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
static int fn_busio_read(void __iomem *addr,unsigned char dx)
{
	unsigned char result;	
	result = readb(addr);
	//printk("readb:addr=0x%08x,result=0x%08x\n",addr,result);
	//printk("readb:dx=%d\n",dx);
	if(dx <= 7)
	{
		if((result&(1 << dx)) == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return -1;
	}
}

 
// ioctl 函数的实现 
// 在应用用户层将通过 ioctl 函数向内核传递参数，以控制 LED的输出状态 
static int am1808_busiow_ioctl( 
 struct inode *inode,  
 struct file *file,   
 unsigned int cmd,  
 unsigned long arg) 
{ 
	int *result;
	result = (int*)arg;	
	if(arg != 0)	
	{
		//printk("busiow:arg = %d\n",*result);	
	}	
	else
	{
		*result = -1;		
		return -1;
	}
	switch (cmd)
   	{
       	case GET_BUS_IO_BTN_UP:
       	*result = fn_busio_read(busio_read_addr_cs0,0);
       	break;
		case GET_BUS_IO_BTN_DOWN:
       	*result = fn_busio_read(busio_read_addr_cs0,1);
       	break;
		case GET_BUS_IO_BTN_LEFT:
       	*result = fn_busio_read(busio_read_addr_cs0,2);
       	break;
		case GET_BUS_IO_BTN_RIGHT:
       	*result = fn_busio_read(busio_read_addr_cs0,3);
       	break;
		case GET_BUS_IO_BTN_ENTER:
       	*result = fn_busio_read(busio_read_addr_cs0,4);
       	break;
		case GET_BUS_IO_BTN_CANCEL:
       	*result = fn_busio_read(busio_read_addr_cs0,5);
       	break;

		case GET_BUS_IO_YX0:
       	*result = fn_busio_read(busio_read_addr_cs1,0);
       	break;
		case GET_BUS_IO_YX1:
       	*result = fn_busio_read(busio_read_addr_cs1,1);
       	break;
		case GET_BUS_IO_POWER_DOWN:
       	*result = fn_busio_read(busio_read_addr_cs1,2);
       	break;
		case GET_BUS_IO_POWER_UP:
       	*result = fn_busio_read(busio_read_addr_cs1,3);
       	break;

		case GET_BUS_IO_STATE0:
       	*result = fn_busio_read(busio_read_addr_cs2,0);
       	break;
		case GET_BUS_IO_STATE1:
       	*result = fn_busio_read(busio_read_addr_cs2,1);
       	break;
		case GET_BUS_IO_STATE2:
       	*result = fn_busio_read(busio_read_addr_cs2,2);
       	break;
		case GET_BUS_IO_STATE3:
       	*result = fn_busio_read(busio_read_addr_cs2,3);
       	break;
		case GET_BUS_IO_STATE4:
       	*result = fn_busio_read(busio_read_addr_cs2,4);
       	break;
		case GET_BUS_IO_BTN_OPEN:
       	*result = fn_busio_read(busio_read_addr_cs2,5);
       	break;
		case GET_BUS_IO_BTN_PROG:
       	*result = fn_busio_read(busio_read_addr_cs2,6);
       	break;

      	default:
       	printk("busior:cmd error\n");
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

	busio_read_addr_cs0 = ((volatile unsigned long)ioremap(0x64000000,1));
	busio_read_addr_cs1 = ((volatile unsigned long)ioremap(0x64000004,1));
	busio_read_addr_cs2 = ((volatile unsigned long)ioremap(0x64000008,1));
	//printk("dev_init:busio_read_addr_cs0=0x%08x,ioremap=0x%08x\n",0x64000000,busio_read_addr_cs0);
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



