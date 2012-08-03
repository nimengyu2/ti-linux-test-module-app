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
#include "wifi_mode.h"
#include "mtd_ops.h"
#include "debug.h"
#include <string.h>

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
int fn_wifi_mode_read(void)
{
    FILE *fp;
    char buf[128];
    int ret;
    char wifi_mode[10];
#ifdef MTD_SPI_FLASH
    int mtd_error_flag = 0;

    ret = fn_mtd_read(M_CFG_BLOCK_WIFI_MODE_CONF,g_pu8_flush_mtd_buf_wifi_mode_conf,
		sizeof(g_pu8_flush_mtd_buf_wifi_mode_conf));
    if(ret == 0)
    {
	if(g_pu8_flush_mtd_buf_wifi_mode_conf[0] == '#')
	{
		printf("fn_wifi_mode_read: read mtd file ok\n");
		fp = fopen(M_WIFI_MODE_PATH,"w+");
		ret = fwrite(g_pu8_flush_mtd_buf_wifi_mode_conf,1,sizeof(g_pu8_flush_mtd_buf_wifi_mode_conf),fp);
		if(ret == sizeof(g_pu8_flush_mtd_buf_wifi_mode_conf))
		{
			printf("fn_wifi_mode_read: get mode from mtd and wirte to ramdisk ok\n");
		}
		else
		{
			printf("fn_wifi_mode_read: get mode from mtd and wirte to ramdisk failed\n");
		}
		// 注意如果使用close(fp)，则会发现写进去的数据不对，因此后来改成fclose(fp)，或者fflush即可
		fflush(fp);
		fclose(fp);
	}   
	else
	{
		mtd_error_flag = 1;
	}      
	
    }
    else   // 读取错误
    {
		mtd_error_flag = 1;
    }
#endif

    fp = fopen(M_WIFI_MODE_PATH,"r");

    fgets(buf,sizeof(buf),fp);
    fclose(fp);
    printf("fn_wifi_mode_read: read from %s is %s\n",M_WIFI_MODE_PATH,buf);
    sscanf(buf,"#wifi_mode=%s",wifi_mode);
    
    ret = strcmp(wifi_mode,"AP");
    if(ret == 0)
    {
	#ifdef MTD_SPI_FLASH	
	if(mtd_error_flag == 1)	
	{
		mtd_error_flag = 0;		
		fn_wifi_mode_write(M_WIFI_MODE_AP);  
	}
	#endif
	      
	#if ( WIFI_MODE_DEBUG >= 3)
        debug_out("wifi mode read is AP mode\n");
        #endif
        return 1;
    }
    ret = strcmp(wifi_mode,"STA");
    if(ret == 0)
    {
	#ifdef MTD_SPI_FLASH	
	if(mtd_error_flag == 1)	
	{
		mtd_error_flag = 0;		
		fn_wifi_mode_write(M_WIFI_MODE_STA);  
	}    
	#endif

	#if ( WIFI_MODE_DEBUG >= 3)
        debug_out("wifi mode read is STA mode\n");
        #endif
        return 0;
    }
    return -1;
}


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
int fn_wifi_mode_write (int wifi_mode)
{
    FILE *fp;
    fp = fopen(M_WIFI_MODE_PATH,"w+");
    if(wifi_mode == 0)
    {
        fprintf(fp,"#wifi_mode=STA\n");
	#ifdef MTD_SPI_FLASH	
	fn_mtd_write(M_CFG_BLOCK_WIFI_MODE_CONF,"#wifi_mode=STA\n",strlen("#wifi_mode=STA\n"));
	#endif
        #if ( WIFI_MODE_DEBUG >= 3)
        debug_out("wifi mode write STA mode\n");
        #endif
    }
    else if(wifi_mode == 1)
    {
        fprintf(fp,"#wifi_mode=AP\n");
	#ifdef MTD_SPI_FLASH
	fn_mtd_write(M_CFG_BLOCK_WIFI_MODE_CONF,"#wifi_mode=AP\n",strlen("#wifi_mode=STA\n"));
	#endif
        #if ( WIFI_MODE_DEBUG >= 3)
        debug_out("wifi mode write AP mode\n");
        #endif
    }
    fclose(fp);
    return 0;
}
