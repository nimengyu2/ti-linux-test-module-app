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
#include "debug.h"

//extern unsigned char u8_config_com_number;
//extern unsigned int u16_config_com_baudrate;
//extern unsigned char u8_config_com_databit;
//extern unsigned char u8_config_com_paritybit;
//extern unsigned char u8_config_com_stopbit;
//extern char pu8_socket_localip[15];
//extern char pu8_socket_serverip[15];
//extern unsigned char u8_socket_serverport;

unsigned char u8_config_com_number = 1;
unsigned int u16_config_com_baudrate = 115200;
unsigned char u8_config_com_databit = 8;
char u8_config_com_paritybit = 'N';
unsigned char u8_config_com_stopbit = 1;
char pu8_socket_localip[15] = "192.168.1.58";
char pu8_socket_serverip[15] = "192.168.1.101";
unsigned int u8_socket_serverport = 5000;

#if 1

int fn_get_parameter(char *buf)
{
    int ret;
    sscanf(buf,"com=%d",&u8_config_com_number);
    sscanf(buf,"baudrate=%d",&u16_config_com_baudrate);
    sscanf(buf,"databit=%d",&u8_config_com_databit);
    sscanf(buf,"paritybit=%c",&u8_config_com_paritybit);
    sscanf(buf,"stopbit=%d",&u8_config_com_stopbit);
    sscanf(buf,"localip=%s",pu8_socket_localip);
    sscanf(buf,"serverip=%s",pu8_socket_serverip);
    sscanf(buf,"serverport=%d",&u8_socket_serverport);
}


int fn_printf_config_information(void)
{
    int ret;
    printf("\nnow printf all config information start\n");
    printf("com=%d\n",u8_config_com_number);
    printf("baudrate=%d\n",u16_config_com_baudrate);
    printf("databit=%d\n",u8_config_com_databit);
    printf("paritybit=%c\n",u8_config_com_paritybit);
    printf("stopbit=%d\n",u8_config_com_stopbit);
    printf("localip=%s\n",pu8_socket_localip);
    printf("serverip=%s\n",pu8_socket_serverip);
    printf("serverport=%d\n",u8_socket_serverport);
    printf("now printf all config information end\n");
}
#endif



unsigned char u8_sdcard_exist = 0;
unsigned char u8_sdcard_configini_exist = 0;
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
		printf("OK:find sdcard,mmcblk0\n");
		return 0;
	}
	else
	{
		printf("ERROR:can't find sdcard,mmcblk0\n");	
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

    char buf[255];
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


int fn_get_sdcard_configini_exist(void)
{
	if(u8_sdcard_exist == 1)
	{
		FILE *fp;
	    	fp = fopen(M_S2WIFI_CONFIG_CFG_PATH,"r");
	    	if(fp == NULL)
	    	{
			u8_sdcard_configini_exist = 0;
			printf("ERROR:fopen /media/mmcblk0p1/config.ini failed\n");
			return -1;
	    	}
	    	else
	   	{			
			u8_sdcard_configini_exist = 1;
			printf("OK:fopen /media/mmcblk0p1/config.ini ok\n");
			return 0;
	    	}	
	}
	else
	{
		u8_sdcard_configini_exist = 0;
		printf("ERROR:no sdcard exist,no config.ini exist\n");
		return -1;
	}
}




