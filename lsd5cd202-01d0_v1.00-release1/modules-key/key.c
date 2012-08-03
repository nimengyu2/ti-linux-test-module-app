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

#define DEVICE_NAME "key" //设备名(/dev/lcd) 

#define MOTOR_MAGIC 'k'
 
unsigned char key_status = 0;

void __iomem *busio_read_addr_cs0;

#define M_BTN_INT   (7*16+12)
 
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
#if 0       	
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
#endif
      	default:
       	printk("busior:cmd error\n");
       	return -EINVAL; 
   } 
   return 0;       
} 

// 本按键驱动的中断服务程序 
static irqreturn_t buttons_interrupt(int irq, void *dev_id) 
{ 
	unsigned char tmp; 
	static unsigned char u8_key = 0x00;
	static unsigned char u8_key_old = 0x00;

	tmp = readb(busio_read_addr_cs0);
	//printk("btn irq,tmp = %02x\n",tmp);
	tmp = tmp | 0xC0;
	//printk("btn irq,tmp = %02x\n",tmp);
	u8_key_old = u8_key;
	u8_key = ~tmp;
	//printk("u8_key_old = %02x,u8_key = %02x\n\n",u8_key_old,u8_key);

	if((u8_key == 0x00) && (u8_key_old != 0x00))
	{
		key_status = u8_key_old;
		//printk("detect key ,key_status = %02x\n\n",key_status);
	}
	
    return IRQ_RETVAL(IRQ_HANDLED); 
} 
 
// 在应用程序执行 open(“/dev/buttons”,…)时会调用到此函数， 在这里， 它的作用主要是注册 6 个按键的中断。
// 所用的中断类型是 IRQ_TYPE_EDGE_BOTH，也就是双沿触发，在上升沿和下降沿均会产生中断，这样做
// 是为了更加有效地判断按键状态 
static int s3c24xx_buttons_open(struct inode *inode, struct file *file) 
{ 
        // 正常返回 
        return 0; 
} 
 
// 此函数对应应用程序的系统调用 close(fd)函数，在此，它的主要作用是当关闭设备时释放 6 个按键的中断*
// 处理函数 
static int s3c24xx_buttons_close(struct inode *inode, struct file *file) 
{ 
    return 0; 
} 
 
 
// 对应应用程序的 read(fd,…)函数，主要用来向用户空间传递键值 
static int s3c24xx_buttons_read(struct file *filp, char __user *buff, size_t count, loff_t *offp) 
{ 
      unsigned long err; 
     // 一组键值被传递到用户空间 
     err = copy_to_user(buff, (const void *)(&key_status),1); 
  	 key_status = 0;
     return (err ? -EFAULT:1); 
} 

 
//  设备函数操作集，在此只有 ioctl函数，通常还有 read, write, open, close 等，因为本 LED驱动在下面已经
//  注册为 misc 设备，因此也可以不用 open/close  
static struct file_operations dev_fops = { 
 .owner = THIS_MODULE, 
 .ioctl = am1808_busiow_ioctl, 
 .open    =   s3c24xx_buttons_open, 
 .release =   s3c24xx_buttons_close,  
 .read    =   s3c24xx_buttons_read, 
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
	//printk("dev_init:busio_read_addr_cs0=0x%08x,ioremap=0x%08x\n",0x64000000,busio_read_addr_cs0);

	int err = 0; 
	// 注册中断函数 
	err = request_irq(gpio_to_irq(M_BTN_INT), buttons_interrupt, IRQ_TYPE_EDGE_BOTH,  
	          "BTN_INT", NULL); 
	//set_irq_type(gpio_to_irq(button_irqs[i].pin), IRQ_TYPE_EDGE_BOTH);


	if (err) 
	{ 
		disable_irq(gpio_to_irq(M_BTN_INT)); 
		free_irq(gpio_to_irq(M_BTN_INT), NULL);
		printk("request_irq failed \n"); 
    } 
	else
	{
		printk("request_irq sucess \n");
	}
	

   	ret = misc_register(&misc); //注册设备 
   	printk (DEVICE_NAME"\tinitialized\n"); //打印初始化信息 
   	return ret; 
} 
 
static void __exit dev_exit(void) 
{ 
	disable_irq(gpio_to_irq(M_BTN_INT)); 
    free_irq(gpio_to_irq(M_BTN_INT), NULL);    
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



