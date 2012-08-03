/**
 * @file serial.c
 * @brief 串口设置
 *
 * Copyright (C) 2001-2011, LSD Science & Technology Co.,Ltd
 * All rights reserved.
 * Software License Agreement
 *
 * LSD Science & Technology (LSD) is supplying this software for use solely
 * its suppliers, and is protected. You may not combine this software with
 * "viral" open software in order to form a larger program.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
 * NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
 * NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. LSD SHALL NOT, UNDER ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 * DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @author toby.zhang <zxj@lierda.com>
 * @date 2011-9-22 17:15:20
 * @version 0.01
 *
 * This file is pary of LSD-S2WIFI_V1.0.
 */

/**
 * @note
 * 串口设置 \n
 *
 * \b Change \b Logs: \n
 * 2011.09.22   创建初始 by toby \n
 *
 */

/**
 * @addtogroup api_serial
 * @{
 */

#include "serial.h"
#include <debug.h>
#include <data_types.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>

// TODO: Add your code here

int g_serial_fd; //!< 串口设备句柄
struct termios g_serial_config_old; //!< 久串口配置信息
struct termios g_serial_config_new; //!< 新串口配置信息
pthread_mutex_t g_serial_mutex; //!< 线程同步互斥对象

/**
 * 串口设置初始化
 *
 * @author Toby (2011-9-22)
 *
 * @param serial_device 所要使用的串口设备
 *
 * @return bool_t ture:成功/false:失败
 */
bool_t serial_open(const char *serial_device)
{
    int ret;

    ret = pthread_mutex_init(&g_serial_mutex, NULL);
    if(ret != 0)
    {
        return(false);
    }

    //g_serial_fd = open(serial_device, O_RDWR | O_NOCTTY | O_NDELAY);
    g_serial_fd = open(serial_device, O_RDWR | O_NOCTTY);

    if(g_serial_fd == -1)
    {
        perror("open serial_device error!\n");
        return(false);
    }

    debug_out("[serial] open %s ok\r\n", serial_device);

    /*
     * config serial_config
     */
    tcgetattr(g_serial_fd, &g_serial_config_old);
    //cfmakeraw(&g_serial_config_new);
    //cfsetispeed(&g_serial_config_new, B115200);
    //cfsetospeed(&g_serial_config_new, B115200);
    //tcsetattr(g_serial_fd, TCSANOW, &g_serial_config_new);

    return(true);
}

/**
 * 关闭串口
 *
 * @author toby.zhang (2011-9-22)
 *
 * @return bool_t 永远返回正确
 */
bool_t serial_close(void)
{
    int ret;

    /*
     * 释放互斥量
     */
    ret = pthread_mutex_destroy(&g_serial_mutex);
    if(ret != 0)
    {
        return(false);
    }

    /*
     * 如果串口已经打开
     */
    if(g_serial_fd > 0)
    {
        tcsetattr(g_serial_fd, TCSANOW, &g_serial_config_old);
        close(g_serial_fd);
        g_serial_fd = -1;
    }

    return(true);
}

/**
 * 串口设置
 *
 * @author Toby (2011-9-29)
 *
 * @param speed 需要设置的波特率(十进制格式)
 *
 * @return bool_t true:设置正确/false:设置错误
 */
bool_t serial_baudrate_set(int baudrate)
{
    /*
     * 进行新的串口设置
     */
    bzero(&g_serial_config_new, sizeof(g_serial_config_new));
    if(tcgetattr(g_serial_fd, &g_serial_config_new) != 0)
    {
        return(false);
    }
    tcflush(g_serial_fd, TCIOFLUSH);

    //cfsetispeed(&g_serial_config_new, B115200);
    //cfsetospeed(&g_serial_config_new, B115200);

    /*
     * 110/300/600/1200/2400/4800/9600/14400/19200/38400/57600/
     * 115200/230400.
     */
    //cfmakeraw(&g_serial_config_new);
    switch(baudrate) // 设置输入输出波特率相同
    {
    case 110:
        //g_serial_config_new.c_cflag |= B110;
        cfsetispeed(&g_serial_config_new, B110);
        cfsetospeed(&g_serial_config_new, B110);
        break;
    case 300:
        //g_serial_config_new.c_cflag |= B300;
        cfsetispeed(&g_serial_config_new, B300);
        cfsetospeed(&g_serial_config_new, B300);
        break;
    case 600:
        //g_serial_config_new.c_cflag |= B600;
        cfsetispeed(&g_serial_config_new, B600);
        cfsetospeed(&g_serial_config_new, B600);
        break;
    case 1200:
        //g_serial_config_new.c_cflag |= B1200;
        cfsetispeed(&g_serial_config_new, B1200);
        cfsetospeed(&g_serial_config_new, B1200);
        break;
    case 2400:
        //g_serial_config_new.c_cflag |= B2400;
        cfsetispeed(&g_serial_config_new, B2400);
        cfsetospeed(&g_serial_config_new, B2400);
        break;
    case 4800:
        //g_serial_config_new.c_cflag |= B4800;
        cfsetispeed(&g_serial_config_new, B4800);
        cfsetospeed(&g_serial_config_new, B4800);
        break;
    case 9600:
        //g_serial_config_new.c_cflag |= B9600;
        cfsetispeed(&g_serial_config_new, B9600);
        cfsetospeed(&g_serial_config_new, B9600);
        break;
    //case 14400:
    //    g_serial_config_new.c_cflag |= B14400;
    //    break;
    case 19200:
        //g_serial_config_new.c_cflag |= B19200;
        cfsetispeed(&g_serial_config_new, B19200);
        cfsetospeed(&g_serial_config_new, B19200);
        break;
    case 38400:
        //g_serial_config_new.c_cflag |= B38400;
        cfsetispeed(&g_serial_config_new, B38400);
        cfsetospeed(&g_serial_config_new, B38400);
        break;
    case 57600:
        //g_serial_config_new.c_cflag |= B57600;
        cfsetispeed(&g_serial_config_new, B57600);
        cfsetospeed(&g_serial_config_new, B57600);
        break;
    case 115200:
        //g_serial_config_new.c_cflag |= B115200;
        cfsetispeed(&g_serial_config_new, B115200);
        cfsetospeed(&g_serial_config_new, B115200);
        break;
    case 230400:
        //g_serial_config_new.c_cflag |= B230400;
        cfsetispeed(&g_serial_config_new, B230400);
        cfsetospeed(&g_serial_config_new, B230400);
        break;
    default:
        return(false);
    }

    /*
     * Enable the receiver and set local mode
     */
    g_serial_config_new.c_cflag |= (CLOCAL | CREAD);

    g_serial_config_new.c_iflag |= IGNPAR; // Ignore parity errors
    g_serial_config_new.c_oflag &= ~OPOST; // Choosing Raw Output

    tcflush(g_serial_fd, TCIFLUSH);
    if(tcsetattr(g_serial_fd, TCSANOW, &g_serial_config_new) != 0)
    {
        return(false);
    }

    return(true);
}


/**
 * 设置串口特率，波数据位，校验位以及停止位
 *
 * @author Toby (2011-9-29)
 *
 * @param baudrate 波特率
 * @param datasize 数据位 5/6/7/8
 * @param parity 校验位 0:None/1:Odd/2:Even/3:Mark/4:Space
 * @param stop 停止位 1/2
 *
 * @return bool_t true:设置正确/false:设置错误
 */
bool_t serial_baudrate_datasize_parity_stop_set(int baudrate,
                                                int datasize,
                                                int parity,
                                                int stop)
{
    /*
     * 设置波特率
     */
    if(serial_baudrate_set(baudrate) == false)
    {
        return(false);
    }

    /*
     * 进行新的串口设置
     */
    bzero(&g_serial_config_new, sizeof(g_serial_config_new));
    if(tcgetattr(g_serial_fd, &g_serial_config_new) != 0)
    {
        return(false);
    }
    tcflush(g_serial_fd, TCIOFLUSH);

    /*
     * 数据位 5/6/7/8.
     */
    g_serial_config_new.c_cflag &= ~CSIZE; // Mask the character size bits
    switch(datasize)
    {
    case 5:
        g_serial_config_new.c_cflag |= CS5;
        break;
    case 6:
        g_serial_config_new.c_cflag |= CS6;
        break;
    case 7:
        g_serial_config_new.c_cflag |= CS7;
        break;
    case 8:
        g_serial_config_new.c_cflag |= CS8;
        break;
    default:
        return(false);
    }

    /*
     * 校验位 0:None/1:Odd/2:Even/3:Mark/4:Space.
     */
    switch(parity)
    {
    case 0: // 无检验
        g_serial_config_new.c_cflag &= ~PARENB; // Disable parity bit
        g_serial_config_new.c_iflag &= ~(INPCK | ISTRIP); //Disable parity check
        break;
    case 1: // 奇校验
        g_serial_config_new.c_cflag |= PARENB; // Enable parity bit
        g_serial_config_new.c_cflag |= PARODD; // Use odd instead of even
        g_serial_config_new.c_iflag |= INPCK; // Enable parity check
        break;
    case 2: // 偶校验
        g_serial_config_new.c_cflag |= PARENB; // Enable parity
        g_serial_config_new.c_cflag &= ~PARODD; // even
        g_serial_config_new.c_iflag |= (INPCK | ISTRIP); // Enable parity check
        break;
    case 3: /*as no parity*/
    case 4:
        g_serial_config_new.c_cflag &= ~PARENB; // Disable parity bit
        g_serial_config_new.c_iflag &= ~(INPCK | ISTRIP); //Disable parity check
        g_serial_config_new.c_cflag &= ~CSTOPB;
        break;
    default:
        return(false);
    }

    /*
     * 停止位 1/2
     */
    switch(stop)
    {
    case 1:
        g_serial_config_new.c_cflag &= ~CSTOPB;
        break;
    case 2:
        g_serial_config_new.c_cflag |= CSTOPB;
        break;
    default:
        return(false);
    }

    /*
     * Choosing Raw Input
     */
    g_serial_config_new.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    //g_serial_config_new.c_lflag &= (~ECHO);
    //g_serial_config_new.c_lflag &= (~ICANON);

    /*
     * 设置等待时间和最小接受字符
     */
    // 100ms超时
    // inter-character timer unused
    g_serial_config_new.c_cc[VTIME] = 1;
    // blocking rd until 1 chars received
    g_serial_config_new.c_cc[VMIN] = 1000;

    tcflush(g_serial_fd, TCIOFLUSH);

    if(tcsetattr(g_serial_fd, TCSANOW, &g_serial_config_new) != 0)
    {
        return(false);
    }

    return(true);
}

/**
 * 从串口中读取一个字节
 *
 * @author Toby (2011-9-22)
 *
 * @param byte 读取的字节存储空间
 *
 * @return bool_t true:读取成功/false:读取失败
 */
bool_t serial_read_char(char *byte)
{
    int err;

    err = read(g_serial_fd, byte, 1);
    if(err == -1)
    {
        return(false);
    }

    return(true);
}

/**
 * 从串口中读取指定大小字节数
 *
 * @author Toby (2011-9-22)
 *
 * @param byte 读取的字节存储空间
 * @param num 指定要读取的数据大小
 *
 * @return bool_t true:读取成功/false:读取失败
 */
bool_t serial_read(char *byte, int num)
{
    int err;

    err = read(g_serial_fd, byte, num);
    if(err == -1)
    {
        return(false);
    }

    return(true);
}

/**
 * 向串口发送一个字节
 *
 * @author Toby (2011-9-22)
 *
 * @param byte 需要向发送的字节
 *
 * @return bool_t true:发送成功/false:发送失败
 */
bool_t serial_send_char(char byte)
{
    int err;
    err = write(g_serial_fd, &byte, 1);
    if(err == -1)
    {
        return(false);
    }

    return(true);
}

/**
 * 向串口发送定大小的字节数
 *
 * @author Toby (2011-9-22)
 *
 * @param byte 需要向发送的字节
 * @param num 指定要发送的数据大小
 *
 * @return bool_t true:发送成功/false:发送失败
 */
bool_t serial_send(char *byte, int num)
{
    int err;
    err = write(g_serial_fd, byte, num);
    if(err == -1)
    {
        return(false);
    }

    return(true);
}

/**
 * Close the Doxygen group.
 * @}
 */

