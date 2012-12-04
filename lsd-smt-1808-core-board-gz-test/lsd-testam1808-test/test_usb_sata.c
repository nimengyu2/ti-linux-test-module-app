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


char scsi_sd[5];
int usb_test_state = 0;

// 重复输入单行 返回获取的sda1的号
// usb_num = 1 usb2.0 otg
// usb_num = 2 usb1.1 host
int fn_get_usb_sdnum(char usb_num,char *buf,char *scsi_sd_str)
{
	int ret;
	char look_for_buf[20];
	int num_tmp;
	static char scsi_sd_ascii;
	static int scsi_num = 0;
	static char scsi_sd_ascii1;
	static char scsi_sd_ascii2;
	static int scsi_sub_num;
	static int scsi_num1;
	char tmp_buf[100];
	if(usb_test_state == 0)
	{
		if(usb_num == 1)
		{
			ret = sscanf(buf,"scsi%d : usb-storage 1-1:1.%d",&scsi_num,&scsi_num1);
		}
		else if(usb_num == 2)
		{
			ret = sscanf(buf,"scsi%d : usb-storage 2-1:1.%d",&scsi_num,&scsi_num1);
		}
		if(ret == 2)
		{
			printf("OK:get scsi%d\n",scsi_num);
			usb_test_state = 1; 
			return 1;
		}
		else
		{
			//printf("ERROR:sscanf scsi: usb-storage  failed,ret=%d,scsi_num=%d,scsi_num1=%d\n",
			//					ret,scsi_num,scsi_num1);
			return -1;
		}
	}
	else if(usb_test_state == 1)
	{
		ret = sscanf(buf,"sd %d:0:0:0: [sd%c]",&num_tmp,&scsi_sd_ascii);
		if(ret == 2)
		{
			printf("OK:get sd %d:0:0:0: [sd%c]\n",num_tmp,scsi_sd_ascii);
			if(num_tmp == scsi_num)
			{
				printf("OK:get scsi_num is same\n");
				usb_test_state = 2; 
				return 2;
			}
			else
			{
				//printf("ERROR:scsi_num is not same\n");
				return -1;
			}
		}
		else
		{
			//printf("ERROR:sscanf sd  :0:0:0: [ ] failed\n");
			return -1;
		}
	}
	else if(usb_test_state == 2)
	{
		// 注意，这里必须是 sd%c: sd%c %d 中间串一个空格，否则，scsi_sd_ascii1 scsi_sd_ascii2
		// 无法获取，实际字符串是 sda: sda1
		ret = sscanf(buf," sd%c: sd%c %d",&scsi_sd_ascii1,&scsi_sd_ascii2,&scsi_sub_num);
		if(ret == 3)
		{
			printf("OK: sd%c: sd%c%d\n",scsi_sd_ascii1,scsi_sd_ascii2,scsi_sub_num);
			sprintf(scsi_sd_str, "sd%c%d",scsi_sd_ascii1,scsi_sub_num);
			usb_test_state = 3;
			return 3;
		}
		else
		{
			//printf("ERROR:sscanf  sd : sd  failed\n");
			return -1;
		}		
		
	}
	else
	{
		usb_test_state = 0;
	}
}

int fn_test_am1808_usb(char usb_num)
{
	int ret;
	char buf[255];
	char *ret_buf;
	char cmd_buf[100];
	
	if(usb_num == 1)
	{
		printf("\n\n--------------------\n");		
		printf("start test usb2.0\n");
	}
	else
	{
		usb_num = 2;
		printf("\n\n--------------------\n");	
		printf("start test usb1.1\n");
	}

	 // 加载rtutil3070ap模式模块
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

	usb_test_state = 0;
    	while(1)
    	{
		ret_buf = fgets(buf,sizeof(buf),fp);
        	if(ret_buf == NULL)
        	{
            		break;
        	}
        	ret = fn_get_usb_sdnum(usb_num,buf,scsi_sd);
        	if(ret == 3)
        	{
			printf("OK:get usb scsi=%s\n",scsi_sd);
            		break;
        	}
	}
	if(usb_test_state != 3)
	{
		//printf("ERROR: not found usb scsi sd\n");
		if(usb_num == 1)
			fn_test_uart_log_console_write("ERROR: usb2.0 not found usb scsi sd\r\n");
		else
			fn_test_uart_log_console_write("ERROR: usb1.1 not found usb scsi sd\r\n");
		return -1;
	}

	sprintf(cmd_buf,"mount -t vfat /dev/%s /mnt/udisk",scsi_sd);	
	ret = system(cmd_buf);
	
	if(ret != 0)
    	{		
		if(usb_num == 1)
			fn_test_uart_log_console_write("ERROR: usb2.0 cant mount\r\n");
		else
			fn_test_uart_log_console_write("ERROR: usb1.1 cant mount\r\n");
		return -1;
    	}
	else
	{
		sprintf(cmd_buf,"umount /dev/%s",scsi_sd);	
		system(cmd_buf);
		//printf("OK:test_usb ok\r\n");
		if(usb_num == 1)
			fn_test_uart_log_console_write("OK: usb2.0 test_usb ok\r\n");
		else
			fn_test_uart_log_console_write("OK: usb1.1 test_usb ok\r\n");
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
	read_fp = popen("ls /dev | grep mtd5", "r");
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
			fn_test_uart_log_console_write("test_nand failed:no /dev/mtdblock5 device\r\n");
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





