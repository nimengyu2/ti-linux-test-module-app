#include <stdio.h> 
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include "config.h"


int main(int argc, char **argv)
{ 
	fn_update();
	return 0;
} 


