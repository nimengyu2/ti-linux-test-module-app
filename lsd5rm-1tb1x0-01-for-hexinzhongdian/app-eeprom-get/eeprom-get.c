#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#pragma pack(1)					// 指定按1字节对齐

typedef  struct  __eeprom_data {
	unsigned int  sn_code;			// 主板编号(从1开始，32位)，出厂不能重复，必须于主板上的贴条一样，因为用于做系统的mac地址
	char    year;				// 生产的年
	char    month;				// 生产的月
	char    day;				// 生产的天
} eeprom_data;

#pragma pack()					// 取消指定对齐，恢复缺省对齐

int main(int argc, char **argv)
{
	int  fd, ret;
	char  *eprom, mac_string[100];
	eeprom_data   *eepromconfig;
	struct i2c_rdwr_ioctl_data e2prom_data;
	unsigned long sn;
	unsigned char year,month,day;

	fd = open("/dev/i2c-3", O_RDWR);

	/*
	 * /dev/i2c-0是在注册i2c-dev.c后产生的，代表一个可操作的适配器。如果不使用i2c-dev.c
	 * 的方式，就没有，也不需要这个节点。
	 */
	if(fd < 0)
	{
		printf("\n\r open /dev/i2c-3 error");
		return 0;
	}

	/*
	 * 因为操作时序中，最多是用到2个开始信号（字节读操作中），所以此将
	 * e2prom_data.nmsgs配置为2
	 */
	e2prom_data.nmsgs = 2;
	e2prom_data.msgs = (struct i2c_msg *)malloc(e2prom_data.nmsgs * sizeof(struct i2c_msg));
	if(!e2prom_data.msgs)
	{
		printf("\n\r e2prom_data.msgs malloc error");
		return 0;
	}

	ioctl(fd, I2C_TIMEOUT, 1);/*超时时间*/
	ioctl(fd, I2C_RETRIES, 2);/*重复次数*/

	
	/******read data from e2prom*******/
	e2prom_data.nmsgs=2;
	(e2prom_data.msgs[0]).len=1;		//e2prom 目标数据的地址
	(e2prom_data.msgs[0]).addr=0x50;	// e2prom 设备地址
	(e2prom_data.msgs[0]).flags=0;		//write
	(e2prom_data.msgs[0]).buf=(unsigned char*)malloc(2);
	(e2prom_data.msgs[0]).buf[0]=0x10;	//e2prom数据地址
	(e2prom_data.msgs[1]).len=7;		//读出的数据
	(e2prom_data.msgs[1]).addr=0x50;	// e2prom 设备地址 
	(e2prom_data.msgs[1]).flags=I2C_M_RD;	//read
	(e2prom_data.msgs[1]).buf=(unsigned char*)malloc(7);	//存放返回值的地址。
	(e2prom_data.msgs[1]).buf[0]=0;		//初始化读缓冲


	ret=ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);
	if(ret<0)
	{
		printf("\n\r ioctl error read");
		return 0;
	}
	else
	{
		printf("Read from  EEPROM address=0x%02x\n",	(e2prom_data.msgs[0]).buf[0]);
		printf("data=%02x %02x %02x %02x %02x %02x %02x \n",	
				(e2prom_data.msgs[1]).buf[0],
				(e2prom_data.msgs[1]).buf[1],
				(e2prom_data.msgs[1]).buf[2],
				(e2prom_data.msgs[1]).buf[3],
				(e2prom_data.msgs[1]).buf[4],
				(e2prom_data.msgs[1]).buf[5],
				(e2prom_data.msgs[1]).buf[6]);
	}

	eepromconfig = (eeprom_data   *)(e2prom_data.msgs[1].buf);
	printf("eepromconfig.sn_code=%d\n",eepromconfig->sn_code);
	printf("eepromconfig.year=%d\n",eepromconfig->year);
	printf("eepromconfig.month=%d\n",eepromconfig->month);
	printf("eepromconfig.day=%d\n",eepromconfig->day);

	// free buff
	free((e2prom_data.msgs[0]).buf);
	free((e2prom_data.msgs[1]).buf);

	// free buff
	free(e2prom_data.msgs);

	close(fd);

	return 0;
}




