#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <sys/select.h> 
#include <sys/time.h> 
#include <errno.h> 
#include <linux/input.h>

int main(void) 
{ 
 	int fd; 
 	int key_value,i=0,count;
 	struct input_event ev_key;
	int key_cnt,atob_cnt,btoa_cnt;
	key_cnt = 0;
	atob_cnt = 0;
	btoa_cnt = 0;
 	fd = open("/dev/event0", 666);//根据实际情况来设置文件，文件名根据/sys/class/input目录下的设备号来确定 
 	if (fd < 0) 
	{ 
  		perror("open device event"); 
  		exit(1); 
 	}
	for (;;) 
	{ 
  		count = read(fd,&ev_key,sizeof(struct input_event));
  		for(i=0; i<(int)count/sizeof(struct input_event); i++) 
		{
			if((ev_key.type == 1) && (ev_key.value == 0))
			{
				//printf("type=0x%04x,code=0x%04x,value=0x%04x\n", ev_key.type,ev_key.code,ev_key.value);
				printf("KEY%d down\n",ev_key.code-1);
			}
			
		}	
 	}
 	close(fd); 
 	return 0; 
}
