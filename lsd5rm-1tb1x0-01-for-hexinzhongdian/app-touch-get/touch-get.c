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


int current_x,current_y,current_p;
int pen_up;
int x,y,pressure;
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
			//printf("type=0x%04x,code=0x%04x,value=0x%04x\n", ev_key.type,ev_key.code,ev_key.value);
			switch (ev_key.type)
			{
				case EV_KEY:
					switch (ev_key.code) {
					case BTN_TOUCH:
						if (ev_key.value == 0)
							pen_up = 1;
						break;
					}
				break;	
				case EV_SYN:
					if (pen_up) {
							printf("touch up,x=%d,y=%d,p=%d\n\n",x,y,pressure);
							x = 0;
							y = 0;
							pressure = 0;
							pen_up = 0;
					} else {
							x = current_x;
							y = current_y;
							pressure = current_p;
							printf("touch down,x=%d,y=%d,p=%d\n",x,y,pressure);
					}
				break;
				case EV_ABS:
					switch (ev_key.code) {
					case ABS_X:
						current_x = ev_key.value;
						break;
					case ABS_Y:
						current_y = ev_key.value;
						break;
					case ABS_PRESSURE:
						current_p = ev_key.value;
						break;
					}
				break;
			}
		}	
 	}
 	close(fd); 
 	return 0; 
}
