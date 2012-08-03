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
#include <asm/gpio.h>

#define DEVICE_NAME "led" //设备名(/dev/led) 

#define MOTOR_MAGIC 'L'
#define SET_LED2 _IOW(MOTOR_MAGIC, 2,int)
#define SET_LED3 _IOW(MOTOR_MAGIC, 3,int)
#define SET_LED4 _IOW(MOTOR_MAGIC, 4,int)
#define SET_LED5 _IOW(MOTOR_MAGIC, 5,int)
 
static int gpio_num[5];
static int gpio_pin[5];
 
// ioctl 函数的实现 
// 在应用用户层将通过 ioctl 函数向内核传递参数，以控制 LED的输出状态 
static int am1808_led_ioctl( 
 struct inode *inode,  
 struct file *file,   
 unsigned int cmd,  
 unsigned long arg) 
{ 
   int gpio_num_tmp;
   printk("cmd is %d \n",cmd);
   switch (cmd)
   {
       case SET_LED2:
       gpio_num_tmp = gpio_num[2];
       break;
       case SET_LED3:
       gpio_num_tmp = gpio_num[3];
       break;
       case SET_LED4:
       gpio_num_tmp = gpio_num[4];
       break;
       case SET_LED5:
       gpio_num_tmp = gpio_num[5];
       break;       
       default:
       printk("cmd may be 2 3 4 5 \n");
       return -EINVAL; 
   } 

   printk("arg is %d \n",arg); 
   if(arg == 0)
   {
      gpio_set_value(gpio_num_tmp,0); 
   }
   else if(arg == 1)
   {
     gpio_set_value(gpio_num_tmp,1); 
   }
   else
   {
     printk("arg may be 0|1 \n");
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
    int i;  
    int status;

    gpio_num[5] = 2*16+0;  // gpio_num = (bank_num * 16) + pin_num 
    gpio_pin[5] = DA850_GPIO2_0;

    gpio_num[4] = 2*16+6;  // gpio_num = (bank_num * 16) + pin_num 
    gpio_pin[4] = DA850_GPIO2_6;

    gpio_num[3] = 2*16+8;  // gpio_num = (bank_num * 16) + pin_num 
    gpio_pin[3] = DA850_GPIO2_8;

    gpio_num[2] = 2*16+15;  // gpio_num = (bank_num * 16) + pin_num 
    gpio_pin[2] = DA850_GPIO2_15;

   for(i = 2; i <=5 ; i++)
   {
	    // init/set pinmux 
	   status = davinci_cfg_reg(gpio_pin[i]);
	   if (status < 0) {
		printk("pin could not be muxed for GPIO functionality %d\n",
								gpio_num[i]);
		return status;
	   }
	
	   status = gpio_request(gpio_num[i], "gpio_test\n");
	   if (status < 0) {
	 	printk("ERROR can not open GPIO %d\n", gpio_num[i]);
		return status;
	   }
	   gpio_direction_output(gpio_num[i],0); 
   }    
     
   ret = misc_register(&misc); //注册设备 
   printk (DEVICE_NAME"\tinitialized\n"); //打印初始化信息 
   return ret; 
} 
 
static void __exit dev_exit(void) 
{ 
   gpio_free(gpio_num[5]);
   gpio_free(gpio_num[4]);
   gpio_free(gpio_num[3]);
   gpio_free(gpio_num[2]);

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
MODULE_AUTHOR("FriendlyARM Inc."); 
