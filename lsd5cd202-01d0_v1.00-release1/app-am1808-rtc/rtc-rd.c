/**
 * @file i2c_rx8025t_test.c
 * @brief test for rx8025t on /dev/i2c-0
 *
 * Copyright (C) 2006-2011, toby
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author toby.zhang <toby.chs@gmail.com>
 * @date 2011-7-24 21:30:07
 * @version 0.01
 *
 * This file is pary of driver for RX8025T.
 */

/**
 * @note
 *  \n
 *  
 * \b Change \b Logs: \n
 * 2011.06.01   创建初始 by toby \n
 * 
 */

/**
 * @addtogroup api_i2c_rx8025t_test
 * @{ 
 */

//#include "i2c_rx8025t_test.h"

// TODO: Add your code here

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

#if 0
/*
 * 定义
 * struct i2c_rdwr_ioctl_data
 * struct i2c_msg
 * 一些用到的宏
 * 要和内核一样, 请查看
 * KERNEL_ROOT/include/linux/i2c-dev.h
 * KERNEL_ROOT/include/linux/i2c.h
 */
#define I2C_RETRIES	0x0701
#define I2C_TIMEOUT	0x0702
#define I2C_RDWR	0x0707

/*
 * Define in KERNEL_ROOT/include/linux/i2c.h
 */
struct i2c_msg
{
	unsigned short addr;
	unsigned short flags;
#define I2C_M_TEN		0x0010	/* this is a ten bit chip address */
#define I2C_M_RD		0x0001	/* read data, from slave to master */
#define I2C_M_NOSTART		0x4000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_REV_DIR_ADDR	0x2000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK	0x1000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NO_RD_ACK		0x0800	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_RECV_LEN		0x0400	/* length will be first received byte */
	unsigned short len; /* msg length				*/
	unsigned char *buf; /* pointer to msg data			*/
};

/*
 * This is the structure as used in the I2C_RDWR ioctl call
 * Define in KERNEL_ROOT/include/linux/i2c-dev.h
 */ 
struct i2c_rdwr_ioctl_data
{
	struct i2c_msg *msgs; /* pointers to i2c_msgs */

	/* nmsgs这个数量决定了有多少开始信号，对于“单开始时序”，取1*/
	int nmsgs; /* number of i2c_msgs */
};
#endif

unsigned char date[7];

/*
 * MAIN FUNCTION
 */
int i2c_rx8025t_test_rd(void)
{
	int fd;
	int ret;
	unsigned long u32_i;
	struct i2c_rdwr_ioctl_data rx8025_ram_data;

	fd = open("/dev/i2c-1", O_RDWR);

	/*
	 * /dev/i2c-0是在注册i2c-dev.c后产生的，代表一个可操作的适配器。如果不使用i2c-dev.c
	 * 的方式，就没有，也不需要这个节点。
	 */
	if(fd<0)
	{
		perror("open error");
	}

	/*
	 * 因为操作时序中，最多是用到2个开始信号（字节读操作中），所以此将
	 * rx8025_ram_data.nmsgs配置为2
	 */
	rx8025_ram_data.nmsgs = 2;
	rx8025_ram_data.msgs = (struct i2c_msg*)malloc(rx8025_ram_data.nmsgs*sizeof(struct i2c_msg));
	if(!rx8025_ram_data.msgs)
	{
		perror("rx8025_ram_data.msgs malloc error");
		exit(1);
	}

	ioctl(fd, I2C_TIMEOUT, 10);/*超时时间*/
	ioctl(fd, I2C_RETRIES, 2);/*重复次数*/

	/*
	 * 写操作
	 */
#if 0
	rx8025_ram_data.nmsgs = 1; // 发送二个消息, 即一次操作
	(rx8025_ram_data.msgs[0]).len = 2; // 1个RX8025T写入目标的地址和1个数据
	(rx8025_ram_data.msgs[0]).addr = 0x50; // RX8025T 设备地址
	(rx8025_ram_data.msgs[0]).flags = 0; // 写操作
	(rx8025_ram_data.msgs[0]).buf = (unsigned char*)malloc(2);
	(rx8025_ram_data.msgs[0]).buf[0] = 0x10; // RX8025T写入目标的地址
	(rx8025_ram_data.msgs[0]).buf[1] = 0x58; // 要写入RX8025T的数据 

	ret = ioctl(fd, I2C_RDWR, (unsigned long)&rx8025_ram_data);
	if(ret<0)
	{
		perror("I2C: WRITE ioctl error");
	}
	sleep(1);
#endif

	/*
	 * 读操作
	 */

	(rx8025_ram_data.msgs[0]).buf = (unsigned char*)malloc(1);
	(rx8025_ram_data.msgs[1]).buf = (unsigned char*)malloc(1); // 存放返回值的地址。
	for(u32_i = 0;u32_i < 7;u32_i++)
	{
		rx8025_ram_data.nmsgs=2; // 发送二个消息,即二次操作
		(rx8025_ram_data.msgs[0]).len = 1; // RX8025T目标数据的地址长度
		(rx8025_ram_data.msgs[0]).addr = 0x51; // RX8025T设备地址
		(rx8025_ram_data.msgs[0]).flags = 0; // 写操作, 地址
		(rx8025_ram_data.msgs[0]).buf[0] = 2+u32_i; // RX8025T数据地址
		(rx8025_ram_data.msgs[1]).len = 1; // 读出的数据长度, 一个字节
		(rx8025_ram_data.msgs[1]).addr = 0x51;// RX8025T设备地址 
		(rx8025_ram_data.msgs[1]).flags = I2C_M_RD; // 读操作
		(rx8025_ram_data.msgs[1]).buf[0] = 0;//初始化读缓冲
		ret = ioctl(fd, I2C_RDWR, (unsigned long)&rx8025_ram_data);
		if(ret<0)
		{
			perror("I2C: READ ioctl error");
		}

		printf("buf[0] = %x\n", (rx8025_ram_data.msgs[1]).buf[0]);
		date[u32_i] = (rx8025_ram_data.msgs[1]).buf[0];
		usleep(1000*10);
	}	
	date[0] = date[0] & 0x7f;
	date[1] = date[1] & 0x7f;
	date[2] = date[2] & 0x3f;
	date[3] = date[3] & 0x3f;
	date[4] = date[4] & 0x07;
	date[5] = date[5] & 0x1f;
	date[6] = date[6] & 0xff;
	printf("sec = (BCD)%x,min = (BCD)%x,hour = (BCD)%x,day = (BCD)%x,week = (BCD)%x,month = (BCD)%x,year = (BCD)%x \n",
		date[0],date[1],date[2],date[3],date[4],date[5],date[6]);
	//printf("sec = %d,min = %d,hour = %d,day = %d,week = %d,month = %d,year = %d \n",
	//	date[0],date[1],date[2],date[3],date[4],date[5],date[6]);

	// free buff
	free((rx8025_ram_data.msgs[0]).buf);
	free((rx8025_ram_data.msgs[1]).buf);

	close(fd);

	return 0;
}



int main(int argc, char **argv)
{
	i2c_rx8025t_test_rd();
}

/**
 * Close the Doxygen group.
 * @}
 */

