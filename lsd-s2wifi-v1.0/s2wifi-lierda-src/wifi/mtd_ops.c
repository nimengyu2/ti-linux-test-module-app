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
#include "debug.h"

#ifdef MTD_SPI_FLASH


#include <stdio.h>
#include <unistd.h>
#include "mtd_ops.h"

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
		#if (M_MTD_OPS_DEBUG > 0)	   	
		printf("fn_get_new_block:open device error"); 
		#endif
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
			#if (M_MTD_OPS_DEBUG > 0)			
			printf("fn_get_new_block %d:fseek error\n",i);
			#endif
			return -1;
		}
		else
		{
			#if (M_MTD_OPS_DEBUG > 0)			
			printf("fn_get_new_block %d:fseek ok\n",i);
			#endif
		
		}
		ret = fread(read_buf,1,sizeof(read_buf),fd);
		if(ret != sizeof(read_buf))
		{
			#if (M_MTD_OPS_DEBUG > 0)			
			printf("fn_get_new_block %d:fread from %d error\n",block,i);
			#endif
			return -1;
		}
		else
		{
			#if (M_MTD_OPS_DEBUG > 0)
			printf("fn_get_new_block %d:read ok\n",i);
			#endif
			ret = sscanf(read_buf,"NUM=0x%08x",&temp);
			if(ret != 1)
			{
				#if (M_MTD_OPS_DEBUG > 0)				
				printf("fn_get_new_block %d:read num failed\n",i);
				#endif
				pu8_block_cnt[i] = 0;
			}
			else
			{
				#if (M_MTD_OPS_DEBUG > 0)				
				printf("fn_get_new_block %d:read num ok\n",i);
				#endif
				pu8_block_cnt[i] = temp;
			}
			if(pu8_block_cnt[i] > block_cnt_max)
			{
				#if (M_MTD_OPS_DEBUG > 0)
				printf("fn_get_new_block %d: block_cnt_max=0x%08x,temp=0x%08x\n",i,block_cnt_max,temp);
				#endif
				block_cnt_max = pu8_block_cnt[i];
				block_cnt_max_index = i;
				
			}
		}
	}
	fclose(fd);
	#if (M_MTD_OPS_DEBUG > 0)
	printf("fn_get_new_block:block_cnt_max=%08x,block_cnt_max_index=%d\n",block_cnt_max,block_cnt_max_index);
	#endif
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
		#if (M_MTD_OPS_DEBUG > 0)	   	
		printf("fn_get_block_to_buf:open device error"); 
		#endif
	   	return -1;
	} 

	block = block * 0x10000;

	ret = fseek(fd, block, SEEK_SET); 
	if(ret != 0)
	{
		#if (M_MTD_OPS_DEBUG > 0)
		printf("fn_get_block_to_buf:fseek error\n");
		#endif
		return -1;
	}
	else
	{
		#if (M_MTD_OPS_DEBUG > 0)
		printf("fn_get_block_to_buf:fseek ok\n");
		#endif
		
	}

	ret = fread(read_buf,1,cnt,fd);
	if(ret != cnt)
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_get_block_to_buf:fread from %d error\n,block");
		#endif
		return -1;
	}
	else
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_get_block_to_buf:fread from addr=0x%08x ok,cnt=%d,data is \n%s\n",block,cnt,read_buf);
		p = read_buf;
		printf("Hex is \n");
		for(i = 0; i < strlen(read_buf); i++)
		{
			printf("0x%02x ",*p);
			p++;
			if(*p == 0xff)
			{
				printf("\n read hex is not ascii\n");				
				break;
			}   
		}
		printf("\n\n");
		#endif
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
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_read: get max cnt block failed\n");
		#endif
		return -1;
	}
	else
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_read: get max cnt block ok\n");
		#endif
	}
	
	fd = fopen(M_CFG_MTD_DEVICE_PATH,"r+");
	if (fd < 0) 
	{ 
		#if (M_MTD_OPS_DEBUG > 0)	   	
		printf("fn_mtd_read:open device error"); 
		#endif
	   	return -1;
	} 

	block_and_file_pos = block_cnt_max_index * 0x10000 + file*M_MTD_FILE_LENGTH;
	#if (M_MTD_OPS_DEBUG > 0)
	printf("fn_mtd_read:block_and_file_pos=0x%08x\n",block_and_file_pos); 
	#endif

	ret = fseek(fd, block_and_file_pos, SEEK_SET); 
	if(ret != 0)
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_read:fseek error\n");
		#endif
		return -1;
	}
	else
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_read:fseek ok\n");	
		#endif
	}

	ret = fread(read_buf,1,cnt,fd);
	if(ret != cnt)
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_read:fread from %d error\n,block");
		#endif
		return -1;
	}
	else
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_read:fread from addr=0x%08x ok,cnt=%d,data is \n%s\n",block_and_file_pos,cnt,read_buf);
		p = read_buf;
		printf("Hex is \n");
		for(i = 0; i < strlen(read_buf); i++)
		{
			printf("0x%02x ",*p);
			p++;
			if(*p == 0xff)
			{
				printf("\n read hex is not ascii\n");				
				break;
			}   
		}
		printf("\n\n");
		#endif
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
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_write: get max cnt block failed\n");
		#endif
		return -1;
	}
	else
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_write: get max cnt block ok\n");
		#endif
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
	#if (M_MTD_OPS_DEBUG > 0)
	printf("fn_mtd_write: block_cnt_now=0x%08x,block_cnt_now_index=%d\n",block_cnt_now,block_cnt_now_index);
	#endif

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
		#if (M_MTD_OPS_DEBUG > 0)           	
		printf("fn_mtd_write:open device error"); 
		#endif
          	return -1;
        } 
	
	//printf("fn_mtd_write:block=%d\n",block);
	//block = block * 0x10000;
	//printf("fn_mtd_write:block=%d\n",block);
	

	ret = fseek(fd, block, SEEK_SET); 
	if(ret != 0)
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_write:fseek error\n");
		#endif
		return -1;
	}
	else
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_write:fseek ok\n");
		#endif
	}

	//printf("fn_mtd_write:block=%d\n",block);
	ret = fwrite(pu8_block_write_buf,1,sizeof(pu8_block_write_buf),fd);
	if(ret != sizeof(pu8_block_write_buf))
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_write:fwrite error\n");
		#endif
		return -1;
	}
	else
	{
		#if (M_MTD_OPS_DEBUG > 0)		
		printf("fn_mtd_write:fwrite data to block addr=0x%08x ok,cnt=%d,data is \n%s\n\n",block+file*M_MTD_FILE_LENGTH,cnt,write_buf);
		p = write_buf;
		printf("Hex is \n");
		for(i = 0; i < strlen(write_buf); i++)
		{
			printf("0x%02x ",*p);
			p++;
			if(*p == 0xff)
			{
				printf("\nwrite hex is not ascii\n");				
				break;
			}   
		}
		printf("\n\n");
		#endif
	}
	// 备注，如果这里使用close(fd)，一旦本程序没有正常结束，则会导致
	// 没有成功将数据写进去flash中，因此必须使用fclose(fd)
	fflush(fd);
	fclose(fd);
    	return 0;
}

#endif
