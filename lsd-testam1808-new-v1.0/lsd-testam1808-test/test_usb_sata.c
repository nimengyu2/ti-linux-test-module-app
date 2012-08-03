#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "am1808_gpio.h"
#include "debug_console.h"
#include "test_usb_sata.h"

int test_am1808_usb_sdb_flag = 0;

int fn_test_am1808_sata(void)
{
	char    buffer[BUFSIZ];
	FILE    *read_fp;
	FILE    *read_fp2;
	int        chars_read;
	int        ret;

	memset( buffer, 0, BUFSIZ );
	test_am1808_usb_sdb_flag = 1;
	if(test_am1808_usb_sdb_flag == 1)
	{
		read_fp = popen("ls /dev | grep sda", "r");
	}
	else
	{
		read_fp = popen("ls /dev | grep sdb", "r");
	}
	
	if (read_fp != NULL )
	{
		chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp);
		if (chars_read > 0)
		{
			if(test_am1808_usb_sdb_flag == 1)
			{
				read_fp2 = popen("fdisk -l /dev/sda | grep 320.0", "r");
			}
			else
			{
				read_fp2 = popen("fdisk -l /dev/sdb | grep 320.0", "r");
			}
			if ( read_fp2 != NULL )
			{
				chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp2);
				if (chars_read > 0)
				{
				}
				else
				{
					fn_test_uart_log_console_write("test_sata failed:sata not exsit,/dev dir device  is not sata\r\n");
					return -1;
				}
			}
			else
			{
				fn_test_uart_log_console_write("test_sata failed:sata not exsit,/dev dir device  is not sata\r\n");
				return -1;
			}

			if(test_am1808_usb_sdb_flag == 1)
			{
				ret = system("mount -t vfat /dev/sda1 /mnt/udisk");
				system("umount /dev/sda1");
			}
			else
			{
				ret = system("mount -t vfat /dev/sdb1 /mnt/udisk");
				system("umount /dev/sdb1");
			}

			if(ret != 0)
		    	{
				fn_test_uart_log_console_write("test_sata failed:mount sata dev error\r\n");
				return -1;
		    	}
			else
			{
		
			}

			fn_test_uart_log_console_write("test_sata ok\r\n");
			return 0;
			
		}
		else
		{	
			fn_test_uart_log_console_write("test_sata failed:no /dev dir device\r\n");
			return -1;
		}
		pclose(read_fp);
	}
	else
	{
		fn_test_uart_log_console_write("test_sata failed:open ls /dev and grep \r\n");
		return -1;
	}	
	
	return 0;
}


int fn_test_am1808_usb20(void)
{
	char    buffer[BUFSIZ];
	FILE    *read_fp;
	FILE    *read_fp2;
	int        chars_read;
	int        ret;

	memset( buffer, 0, BUFSIZ );
	read_fp = popen("ls /dev | grep sdb", "r");
	if ( read_fp != NULL )
	{
		chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp);
		if (chars_read > 0)
		{
			read_fp2 = popen("fdisk -l /dev/sdb | grep 320.0", "r");
			if ( read_fp2 != NULL )
			{
				chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp2);
				if (chars_read > 0)
				{
					fn_test_uart_log_console_write("test_usb20 failed:usb not exsit,/dev/sda is sata\r\n");
					return -1;
				}
				else
				{
					
				}
			}
			else
			{
				
			}
#if 0			
			//system("umount /dev/sdb1");
	    		ret = system("mount -t vfat /dev/sdb1 /mnt/udisk");
			system("umount /dev/sdb1");
			//printf("test_usb ret=%d\r\n",ret);

		    	
			if(ret != 0)
		    	{
				fn_test_uart_log_console_write("test_usb failed:mount /dev/sdb1 error\r\n");
				return -1;
		    	}
			else
			{
		
			}
#endif
			test_am1808_usb_sdb_flag = 1;
			fn_test_uart_log_console_write("test_usb20 ok\r\n");
			return 0;
		}
		else
		{
			fn_test_uart_log_console_write("test_usb20 failed:no /dev/sdb device\r\n");
			return -1;
		}
		pclose(read_fp);
	}
	else
	{
		fn_test_uart_log_console_write("test_usb20 failed:open ls /dev and grep \r\n");
		return -1;
	}

	return 0;	
}

int fn_test_am1808_usb11(void)
{
	char    buffer[BUFSIZ];
	FILE    *read_fp;
	FILE    *read_fp2;
	int        chars_read;
	int        ret;

	memset( buffer, 0, BUFSIZ );
	read_fp = popen("ls /dev | grep sdc", "r");
	if ( read_fp != NULL )
	{
		chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp);
		if (chars_read > 0)
		{
			read_fp2 = popen("fdisk -l /dev/sdc | grep 320.0", "r");
			if ( read_fp2 != NULL )
			{
				chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp2);
				if (chars_read > 0)
				{
					fn_test_uart_log_console_write("test_usb11 failed:usb not exsit,/dev/sdc is sata\r\n");
					return -1;
				}
				else
				{
					
				}
			}
			else
			{
				
			}
			
			//system("umount /dev/sdb1");
	    		ret = system("mount -t vfat /dev/sdc1 /mnt/udisk");
			system("umount /dev/sdc1");
			//printf("test_usb ret=%d\r\n",ret);
#if 1
		    	
			if(ret != 0)
		    	{
				fn_test_uart_log_console_write("test_usb11 failed:mount /dev/sdc1 error\r\n");
				return -1;
		    	}
			else
			{
		
			}
#endif
			test_am1808_usb_sdb_flag = 1;
			fn_test_uart_log_console_write("test_usb11 ok\r\n");
			return 0;
		}
		else
		{
			fn_test_uart_log_console_write("test_usb11 failed:no /dev/sdc device\r\n");
			return -1;
		}
		pclose(read_fp);
	}
	else
	{
		fn_test_uart_log_console_write("test_usb11 failed:open ls /dev and grep \r\n");
		return -1;
	}

	return 0;	
}


int fn_test_am1808_mmc(void)
{
	char    buffer[BUFSIZ];
	FILE    *read_fp;
	FILE    *read_fp2;
	int        chars_read;
	int        ret;

	memset( buffer, 0, BUFSIZ );
	read_fp = popen("ls /dev | grep mmcblk0", "r");
	if ( read_fp != NULL )
	{
		chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp);
		if (chars_read > 0)
		{
			
			//system("umount /dev/sdb1");
	    		ret = system("mount -t vfat /dev/mmcblk0p1 /mnt/udisk");
			system("umount /dev/mmcblk0p1");
			//printf("test_usb ret=%d\r\n",ret);
#if 1
		    	
			if(ret != 0)
		    	{
				fn_test_uart_log_console_write("test_mmc failed:mount /dev/mmcblk0p1 error\r\n");
				return -1;
		    	}
			else
			{
		
			}
#endif
			test_am1808_usb_sdb_flag = 1;
			fn_test_uart_log_console_write("test_mmc ok\r\n");
			return 0;
		}
		else
		{
			fn_test_uart_log_console_write("test_mmc failed:no /dev/mmcblk0p1 device\r\n");
			return -1;
		}
		pclose(read_fp);
	}
	else
	{
		fn_test_uart_log_console_write("test_mmc failed:open ls /dev and grep \r\n");
		return -1;
	}

	return 0;	
}


int fn_test_am1808_nand(void)
{
	char    buffer[BUFSIZ];
	FILE    *read_fp;
	FILE    *read_fp2;
	int        chars_read;
	int        ret;

	memset( buffer, 0, BUFSIZ );
	read_fp = popen("ls /dev | grep mtd", "r");
	if ( read_fp != NULL )
	{
		chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp);
		if (chars_read > 0)
		{
#if 0			
			//system("umount /dev/sdb1");
	    		ret = system("mount -t jffs2 /dev/mtdblock3 /mnt/jffs2");
			system("umount /dev/mtdblock3");
			//printf("test_usb ret=%d\r\n",ret);

		    	
			if(ret != 0)
		    	{
				fn_test_uart_log_console_write("test_nand failed:mount /dev/mtdblock3 error\r\n");
				return -1;
		    	}
			else
			{
		
			}
#endif
			test_am1808_usb_sdb_flag = 1;
			fn_test_uart_log_console_write("test_nand ok\r\n");
			return 0;
		}
		else
		{
			fn_test_uart_log_console_write("test_nand failed:no /dev/mtdblock3 device\r\n");
			return -1;
		}
		pclose(read_fp);
	}
	else
	{
		fn_test_uart_log_console_write("test_nand failed:open ls /dev and grep \r\n");
		return -1;
	}

	return 0;	
}





