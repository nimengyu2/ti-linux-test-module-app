#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <mtd/mtd-user.h>


// Ö÷º¯Êý
int main(int argc, char **argv) 
{ 

	int fd;
	int ret;
	int block;
	unsigned char mtd_device[64];
	char erase_cmd[256];
	
	if ((sscanf(argv[1], "%s", mtd_device) != 1))
        {
           printf("erase-mtd <device> <block> \n"); 
           exit(1); 
        }

	if ((sscanf(argv[2],"%d",&block) != 1))
        {
           printf("erase-mtd <device> <block> \n"); 
           exit(1); 
        }
	printf("mtd_device=%s,block=%d\n",mtd_device,block);
		
	block = block * 0x10000;

	snprintf(erase_cmd,sizeof(erase_cmd),"flash_erase %s 0x%08x 1\n",mtd_device,block);
	system(erase_cmd);
	
		
    	return 0;
} 

