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
#include "mtd_ops.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>


unsigned char g_pu8_flush_mtd_buf_wifi_mode_conf[64];
unsigned char g_pu8_flush_mtd_buf_login_cfg[1024];
unsigned char g_pu8_flush_mtd_buf_s2wifi_cfg[4096];

#define M_MTD_64KB_BLOCK_NUM  8
#define M_MTD_FILE_LENGTH    (4*1024)
#define M_MTD_NOW_FILE_NUM    3   // 第一个文件 是记录NUM=0x%08x  第二个文件是
unsigned char pu8_block_write_buf[M_MTD_FILE_LENGTH*M_MTD_NOW_FILE_NUM];
unsigned char pu8_block_read_buf[M_MTD_FILE_LENGTH*M_MTD_NOW_FILE_NUM];

unsigned long pu8_block_cnt[M_MTD_64KB_BLOCK_NUM];
unsigned long block_cnt_max;
unsigned long block_cnt_max_index;
unsigned long block_cnt_now;
unsigned long block_cnt_now_index;



unsigned char g_u8_log_open_flag = 0;

// TODO: Add your code here
FILE *log_file;
unsigned char debug_buf[16*1024];
void debug_open(void)
{
    //openlog("S2WIFI_Msg", LOG_CONS | LOG_PID, 0);
    if(g_u8_log_open_flag == 0)
    {
	g_u8_log_open_flag = 1;
	log_file = fopen("/wifi/log","a+");
    	if (log_file < 0) 
    	{ 
           //perror("open device error"); 
          //return 0;
    	} 
    }  
}

void debug_out(const char *format, ...)
{
    va_list va_arg_p;

    // Start the varargs processing.
    va_start(va_arg_p, format);

//#ifdef DEBUG_TO_TERMINAL
    //vfdebug_out(stderr, format, va_arg_p);
//#endif

//#ifdef DEBUG_TO_SYSLOG
    //syslog(LOG_DEBUG, format, va_arg_p);
//#endif
    //vfprintf(log_file, format, va_arg_p);
    vsnprintf((char *)debug_buf, sizeof(debug_buf), format, va_arg_p);
    fprintf(log_file,debug_buf);

    // End the varargs processing.
    va_end(va_arg_p);
}

void debug_close(void)
{
    //closelog();
    if(g_u8_log_open_flag == 1)
    {
	g_u8_log_open_flag = 0;
	fclose(log_file);
    }  
}


int fn_get_max_cnt_block(void)
{
	unsigned long i;
	unsigned long block;
	FILE * fd;
	char read_buf[64];
	unsigned long temp;
	int ret;
	fd = fopen(M_CFG_MTD_DEVICE_PATH,"r");
	if (fd < 0) 
	{ 
	   	debug_out("fn_get_new_block:open device error"); 
	   	return -1;
	} 
	
	block_cnt_max = 0;
	block_cnt_max_index = 0;
	
	for(i = 0;i < M_MTD_64KB_BLOCK_NUM;i++)
	{
		block = i * 0x10000;		
		ret = fseek(fd, block, SEEK_SET); 
		if(ret != 0)
		{
			debug_out("fn_get_new_block %d:fseek error\n",i);
			return -1;
		}
		else
		{
			debug_out("fn_get_new_block %d:fseek ok\n",i);
		
		}
		ret = fread(read_buf,1,sizeof(read_buf),fd);
		if(ret != sizeof(read_buf))
		{
			debug_out("fn_get_new_block %d:fread from %d error\n",block,i);
			return -1;
		}
		else
		{
			debug_out("fn_get_new_block %d:read ok\n",i);
			ret = sscanf(read_buf,"NUM=0x%08x",&temp);
			if(ret != 1)
			{
				debug_out("fn_get_new_block %d:read num failed\n",i);
				pu8_block_cnt[i] = 0;
			}
			else
			{
				debug_out("fn_get_new_block %d:read num ok\n",i);
				pu8_block_cnt[i] = temp;
			}
			if(pu8_block_cnt[i] > block_cnt_max)
			{
				debug_out("fn_get_new_block %d: block_cnt_max=0x%08x,temp=0x%08x\n",i,block_cnt_max,temp);
				block_cnt_max = pu8_block_cnt[i];
				block_cnt_max_index = i;
				
			}
		}
	}
	fclose(fd);
	debug_out("fn_get_new_block:block_cnt_max=%08x,block_cnt_max_index=%d\n",block_cnt_max,block_cnt_max_index);
	return 0;
}
int fn_get_block_data_to_buf(unsigned long block,unsigned char *read_buf,unsigned int cnt)
{
	FILE * fd;
	//unsigned long block;
	int i;
	unsigned char *p;
	int ret;

	fd = fopen(M_CFG_MTD_DEVICE_PATH,"r+");
	if (fd < 0) 
	{ 
	   	debug_out("fn_get_block_to_buf:open device error"); 
	   	return -1;
	} 

	block = block * 0x10000;

	ret = fseek(fd, block, SEEK_SET); 
	if(ret != 0)
	{
		debug_out("fn_get_block_to_buf:fseek error\n");
		return -1;
	}
	else
	{
		debug_out("fn_get_block_to_buf:fseek ok\n");
		
	}

	ret = fread(read_buf,1,cnt,fd);
	if(ret != cnt)
	{
		debug_out("fn_get_block_to_buf:fread from %d error\n,block");
		return -1;
	}
	else
	{
		debug_out("fn_get_block_to_buf:fread from addr=0x%08x ok,cnt=%d,data is \n%s\n",block,cnt,read_buf);
		p = read_buf;
		debug_out("Hex is \n");
		for(i = 0; i < strlen(read_buf); i++)
		{
			debug_out("0x%02x ",*p);
			p++;
			if(*p == 0xff)
			{
				debug_out("\n read hex is not ascii\n");				
				break;
			}   
		}
		debug_out("\n\n");
	}
	fclose(fd);
    	return 0;
}






//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  如果处于AP模式,则返回1,客户端模式返回0
//          错误返回-1
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_mtd_read(unsigned long file,unsigned char *read_buf,unsigned int cnt)
{
	FILE * fd;
	unsigned long block_and_file_pos;
	int i;
	unsigned char *p;
	int ret;
	ret = fn_get_max_cnt_block();
	if(ret == -1)
	{
		debug_out("fn_mtd_read: get max cnt block failed\n");
		return -1;
	}
	else
	{
		debug_out("fn_mtd_read: get max cnt block ok\n");
	}
	
	fd = fopen(M_CFG_MTD_DEVICE_PATH,"r+");
	if (fd < 0) 
	{ 
	   	debug_out("fn_mtd_read:open device error"); 
	   	return -1;
	} 

	block_and_file_pos = block_cnt_max_index * 0x10000 + file*M_MTD_FILE_LENGTH;
	debug_out("fn_mtd_read:block_and_file_pos=0x%08x\n",block_and_file_pos); 

	ret = fseek(fd, block_and_file_pos, SEEK_SET); 
	if(ret != 0)
	{
		debug_out("fn_mtd_read:fseek error\n");
		return -1;
	}
	else
	{
		debug_out("fn_mtd_read:fseek ok\n");
		
	}

	ret = fread(read_buf,1,cnt,fd);
	if(ret != cnt)
	{
		debug_out("fn_mtd_read:fread from %d error\n,block");
		return -1;
	}
	else
	{
		debug_out("fn_mtd_read:fread from addr=0x%08x ok,cnt=%d,data is \n%s\n",block_and_file_pos,cnt,read_buf);
		p = read_buf;
		debug_out("Hex is \n");
		for(i = 0; i < strlen(read_buf); i++)
		{
			debug_out("0x%02x ",*p);
			p++;
			if(*p == 0xff)
			{
				debug_out("\n read hex is not ascii\n");				
				break;
			}   
		}
		debug_out("\n\n");
	}
	fclose(fd);
    	return 0;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :
// 函数功能  :  如果处于AP模式,则返回1,客户端模式返回0
//          错误返回-1
//
// 输入参数  :  无
//
// 返回参数  :  无
//
//-----------------------------------------------------------------------------
int fn_mtd_write(unsigned long  file,unsigned char* write_buf,unsigned int cnt)
{
	FILE * fd;
	unsigned long block;
	int i;
	unsigned char *p;
	char erase_cmd[64];
	int ret;

	ret = fn_get_max_cnt_block();
	if(ret == -1)
	{
		debug_out("fn_mtd_write: get max cnt block failed\n");
		return -1;
	}
	else
	{
		debug_out("fn_mtd_write: get max cnt block ok\n");
	}
	if(block_cnt_max_index >= (M_MTD_64KB_BLOCK_NUM - 1))
	{
		block_cnt_now_index = 0;
	}
	else
	{
		block_cnt_now_index = block_cnt_max_index + 1;
	}
	block_cnt_now = block_cnt_max + 1;
	debug_out("fn_mtd_write: block_cnt_now=0x%08x,block_cnt_now_index=%d\n",block_cnt_now,block_cnt_now_index);

	// 获取前一个block的数据		
	fn_get_block_data_to_buf(block_cnt_max_index,pu8_block_write_buf,sizeof(pu8_block_write_buf));
	
	snprintf(pu8_block_write_buf,64,"NUM=0x%08x",block_cnt_now);
	// 复制要写的数据	
	memcpy(&pu8_block_write_buf[file*M_MTD_FILE_LENGTH],write_buf,cnt);

	// 擦除新的block	
	block = block_cnt_now_index * 0x10000;

	snprintf(erase_cmd,sizeof(erase_cmd),"/usr/sbin/flash_erase %s 0x%08x 1\n",M_CFG_MTD_DEVICE_PATH,block);
	system(erase_cmd);

	fd = fopen(M_CFG_MTD_DEVICE_PATH,"r+");
	if (fd < 0) 
	{ 
           	debug_out("fn_mtd_write:open device error"); 
          	return -1;
        } 
	
	//printf("fn_mtd_write:block=%d\n",block);
	//block = block * 0x10000;
	//printf("fn_mtd_write:block=%d\n",block);
	

	ret = fseek(fd, block, SEEK_SET); 
	if(ret != 0)
	{
		debug_out("fn_mtd_write:fseek error\n");
		return -1;
	}
	else
	{
		debug_out("fn_mtd_write:fseek ok\n");
	}

	//printf("fn_mtd_write:block=%d\n",block);
	ret = fwrite(pu8_block_write_buf,1,sizeof(pu8_block_write_buf),fd);
	if(ret != sizeof(pu8_block_write_buf))
	{
		debug_out("fn_mtd_write:fwrite error\n");
		return -1;
	}
	else
	{
		debug_out("fn_mtd_write:fwrite data to block addr=0x%08x ok,cnt=%d,data is \n%s\n\n",block+file*M_MTD_FILE_LENGTH,cnt,write_buf);
		p = write_buf;
		debug_out("Hex is \n");
		for(i = 0; i < strlen(write_buf); i++)
		{
			debug_out("0x%02x ",*p);
			p++;
			if(*p == 0xff)
			{
				debug_out("\nwrite hex is not ascii\n");				
				break;
			}   
		}
		debug_out("\n\n");
	}
	// 备注，如果这里使用close(fd)，一旦本程序没有正常结束，则会导致
	// 没有成功将数据写进去flash中，因此必须使用fclose(fd)
	fflush(fd);
	fclose(fd);
    	return 0;
}


#define S2WIFI_CFG_FILE          "/var/www/cgi-bin/s2wifi.cfg"

char fn_write_s2wifi_config_to_mtd(void)
{
	FILE *fp_login_cfg, *fp_s2wifi_cfg;
	int ret;
	//unsigned char pu8_para_buf[1024];	
	
#if 0
	if(g_u8_log_open_flag == 0)
	{
		debug_open();
		g_u8_log_open_flag  = 1;
	}	
#endif
	fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "r+");
	if(fp_s2wifi_cfg == NULL)
	{
		return(-1);
	}
	ret = fread(g_pu8_flush_mtd_buf_s2wifi_cfg,1,sizeof(g_pu8_flush_mtd_buf_s2wifi_cfg),fp_s2wifi_cfg);
	if(ret <= 0)
	{
		debug_out("read_config s2wifi:fread from read_config error\n");
		return(-1);
	}	
	else
	{
		debug_out("read_config s2wifi:fread from ramdisk read_config ok\n");
		fn_mtd_write(M_CFG_BLOCK_S2WIFI_CFG ,g_pu8_flush_mtd_buf_s2wifi_cfg,sizeof(g_pu8_flush_mtd_buf_s2wifi_cfg));
		debug_out("read_config s2wifi:write to mtd \n");
	}
	fclose(fp_s2wifi_cfg);	
	return 0;
}


char fn_cat_s2wifi_cfg(void)
{
	FILE *fp_login_cfg, *fp_s2wifi_cfg;
	int ret;
	//unsigned char pu8_para_buf[1024];	
	
#if 0
	if(g_u8_log_open_flag == 0)
	{
		debug_open();
		g_u8_log_open_flag  = 1;
	}	
#endif
	fp_s2wifi_cfg = fopen(S2WIFI_CFG_FILE, "r+");
	if(fp_s2wifi_cfg == NULL)
	{
		return(-1);
	}
	ret = fread(g_pu8_flush_mtd_buf_s2wifi_cfg,1,sizeof(g_pu8_flush_mtd_buf_s2wifi_cfg),fp_s2wifi_cfg);
	if(ret <= 0)
	{
		debug_out("fn_cat_s2wifi_cfg:fread from read_config error\n");
		return(-1);
	}	
	else
	{
		debug_out("fn_cat_s2wifi_cfg:fread from ramdisk read_config ok\n");
		debug_out("%s\n\n",g_pu8_flush_mtd_buf_s2wifi_cfg);		
	}
	fclose(fp_s2wifi_cfg);	
	return 0;
}


