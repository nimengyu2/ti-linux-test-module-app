//-----------------------------------------------------------------------------
//
//                                 timer中断程序
//                                  源文件(*.c)
//
//
//                    版权所有(C)2005-2010 利尔达科技有限公司
//
//
// 文件名    : template.c
// 作者      : nmy
// 生成日期  : 2011-09-21
//
// arm gcc   : arm-none-linux-gnueabi-gcc 4.5.3
//
// 版本记录  : V1.00  创建第一版   2010-09-11 15:30
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "config.h"
#include <stdio.h> 
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <sys/types.h> 
#include <sys/stat.h> 

unsigned char u8_sdcard_exist = 0;
unsigned char u8_sdcard_updateini_exist = 0;
unsigned char u8_sdcard_s2e_exist = 0;
unsigned char u8_update = 0;
unsigned char u8_del = 0;

int fn_get_parameter(char *buf)
{
    int ret;
    sscanf(buf,"updates=%d",&u8_update);
    sscanf(buf,"del=%d",&u8_del);
}

int fn_printf_config_information(void)
{
    int ret;
    printf("\nnow printf all config information start\n");
    printf("update=%d\n",u8_update);
    printf("del=%d\n",u8_del);
    printf("now printf all config information end\n");
}

int fn_get_sdcard_exist(void)
{
	int ret;

	ret = system("dmesg > /tmp/dmesg.txt");
	if(ret == -1)
	{
		printf("ERROR:system dmesg error,you need redo filesystem\n");
		return -1;
	}
	else
	{
		printf("OK:system dmesg success\n");
	}
	
	FILE *fp;
    	fp = fopen("/tmp/dmesg.txt","rw");
    	if(fp == NULL)
    	{
        	printf("ERROR:fopen /tmp/dmesg.txt failed\n");
        	return -1;
    	}
    	else
   	{
		printf("OK:fopen /tmp/dmesg.txt ok\n");
    	}	


	char temp1[20];
	char temp2[20]; 
	char buf[255];
	char *ret_buf;
	
    	while(1)
    	{
		ret_buf = fgets(buf,sizeof(buf),fp);
        	if(ret_buf == NULL)
        	{
			u8_sdcard_exist = 0;
            		break;
        	}
		ret = sscanf(buf,"mmcblk0:%s",&temp2);
        	if(ret == 1)
        	{
			u8_sdcard_exist = 1;
            		break;
        	}
	}
	if(u8_sdcard_exist == 1)
	{
		printf("OK:find sdcard,mmcblk0,u8_sdcard_exist=%d\n",u8_sdcard_exist);
		return 0;
	}
	else
	{
		printf("ERROR:can't find sdcard,mmcblk0,u8_sdcard_exist=%d\n",u8_sdcard_exist);	
		return -1;
	}
	
}


#if 1
//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_get_config (void)
{
    FILE *fp;
    fp = fopen(M_S2WIFI_CONFIG_CFG_PATH,"rw");
    if(fp == NULL)
    {
        #if (CONFIG_DEBUG >= 1)
        printf("fopen M_FILE_S2WIFI_CONFIG_PATH failed\n");
        #endif
        return -1;
    }
    else
    {
        #if (CONFIG_DEBUG >= 3)
        printf("fopen M_FILE_S2WIFI_CONFIG_PATH sucess\n");
        #endif
    }

    char buf[100];
    int ret;
    char *ret_buf;
    int get_para_right_cnt = 0;
    while(1)
    {
        ret_buf = fgets(buf,sizeof(buf),fp);
        if(ret_buf == NULL)
        {
            break;
        }
        ret = fn_get_parameter(buf);
        if(ret != -1)
        {
            get_para_right_cnt++;
        }
    }
    #if (CONFIG_DEBUG >= 3)
    printf("get_para_right_cnt = %d\n",get_para_right_cnt);
    #endif

    fclose(fp);
    return 0;
}
#endif


int fn_get_sdcard_updateini_exist(void)
{
	if(u8_sdcard_exist == 1)
	{
		FILE *fp;
	    	fp = fopen(M_S2WIFI_CONFIG_CFG_PATH,"r");
	    	if(fp == NULL)
	    	{
			u8_sdcard_updateini_exist = 0;
			printf("ERROR:fopen /media/mmcblk0p1/update.ini failed,u8_sdcard_updateini_exist=0\n");
			return -1;
	    	}
	    	else
	   	{			
			u8_sdcard_updateini_exist = 1;
			printf("OK:fopen /media/mmcblk0p1/update.ini ok,u8_sdcard_updateini_exist=1\n");
			return 0;
	    	}	
	}
	else
	{
		u8_sdcard_updateini_exist = 0;
		printf("ERROR:no sdcard exist,no update.ini exist,u8_sdcard_updateini_exist=0\n");
		return -1;
	}
}

int fn_get_sdcard_s2e_exist(void)
{
	printf("u8_sdcard_exist=%d\n",u8_sdcard_exist);
	if(u8_sdcard_exist == 1)
	{
		FILE *fp;
	    	fp = fopen("/media/mmcblk0p1/s2e","r");
	    	if(fp == NULL)
	    	{
			u8_sdcard_s2e_exist = 0;
			printf("ERROR:fopen /media/mmcblk0p1/s2e failed\n");
			return -1;
	    	}
	    	else
	   	{			
			u8_sdcard_s2e_exist = 1;
			printf("OK:fopen /media/mmcblk0p1/s2e ok\n");
			return 0;
	    	}	
	}
	else
	{
		u8_sdcard_s2e_exist = 0;
		printf("ERROR:no sdcard exist,no s2e exist\n");
		return -1;
	}
}


int fn_file_write_to_end(char *pathname,unsigned char *data,int data_len)
{
	int fd;
	int write_cnt;
	fd = open(pathname,O_RDWR | O_CREAT | O_APPEND,S_IRUSR | S_IWUSR | S_IXUSR);
	if(fd == -1)
	{
		printf("open file %s error\n",pathname);
		return -1;
	}
	write_cnt = write(fd,data,data_len);
	if(write_cnt != data_len)
	{
		printf("write data error,need %d, acture write %d\n",data_len,write_cnt);
		close(fd);
		return write_cnt;
	}
	close(fd);
	return write_cnt;
}

int fn_file_trunc(char *pathname)
{
	int fd;
	int write_cnt;
	fd = open(pathname,O_RDWR | O_TRUNC);
	if(fd == -1)
	{
		printf("open file %s error\n",pathname);
		return -1;
	}
	close(fd);
	return 0;
}


int fn_file_read(char *pathname,unsigned char *data,int data_len)
{
	int fd;
	int read_cnt;
	fd = open(pathname,O_RDWR);
	if(fd == -1)
	{
		printf("open file %s error\n",pathname);
		return -1;
	}
	read_cnt = read(fd,data,data_len);
	if(read_cnt != data_len)
	{
		printf("read data error,need %d, acture write %d\n",data_len,read_cnt);
		close(fd);
		return read_cnt;
	}
	close(fd);
	return read_cnt;
}


int fn_update(void)
{
	char pu8_cmd_string[40];
	fn_get_sdcard_exist();
	fn_get_sdcard_updateini_exist();
	fn_get_sdcard_s2e_exist();
	if(u8_sdcard_updateini_exist == 1)
	{
		fn_get_config();
	}
	fn_printf_config_information();
	
	printf("u8_update=%d,u8_sdcard_s2e_exist=%d,u8_del=%d\n",u8_update,u8_sdcard_s2e_exist,u8_del);
	if(u8_update == 1)
	{	
		if(u8_sdcard_s2e_exist == 1)
		{
			printf("update s2e code\n");
			system("cp /media/mmcblk0p1/s2e /s2e");
			if(u8_del == 1)
			{
				fn_file_trunc(M_S2WIFI_CONFIG_CFG_PATH);
				fn_file_write_to_end(M_S2WIFI_CONFIG_CFG_PATH,"updates=0\ndel=1\n",strlen("update=0\ndel=1\n"));
				printf("modify file %s\n",M_S2WIFI_CONFIG_CFG_PATH);
			}
		}
	}
	return 0;
}




