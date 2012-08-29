#include <fcntl.h> 
#include <stdio.h> 
#include <sys/ioctl.h> 
  
int main() 
{ 
 int f; 
 f = open("/dev/tty0", O_RDWR); 
 write(f, "\033[9;0]", 8); 
 close(f); 
 return 0;
} 

