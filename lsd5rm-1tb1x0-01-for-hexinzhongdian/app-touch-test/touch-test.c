//compiler: gcc 4.3.2   
#include <stdio.h>   
#include <stdlib.h>   
#include <unistd.h>   
#include <fcntl.h>   
#include <sys/select.h>   
#include <sys/types.h>   
#include <sys/stat.h>   
#include <linux/input.h>   
#define DEV_ENENT  "/dev/event0"   
#define EVENT_STACK_SIZE 16   
struct input_event event_stack[EVENT_STACK_SIZE];  
int main()  
{  
 int iFd = -1;  
 int iSize = 0;  
 int i;  
 int retval;  
 fd_set rfds;  
  
 iFd = open(DEV_ENENT, O_RDONLY | O_NONBLOCK);  
 if(iFd < 0)  
 {  
  perror(DEV_ENENT);  
  return -1;  
 }  
 while(1)  
 {  
    
  FD_ZERO(&rfds);  
  FD_SET(iFd, &rfds);  
  retval = select(iFd + 1, &rfds, NULL, NULL, NULL);  
  if (retval == -1)  
  {  
   perror("select()");  
   continue;  
  }  
  else if (retval)  
   printf("Data is available now.\n");  
  else  
   continue;  
  iSize = read(iFd, event_stack, sizeof(event_stack));  
  if(iSize >= 0)  
  {  
   for(i = 0; i < iSize/sizeof(event_stack[0]); i++)  
    printf("type = %d\n  \  
     code = %d\n  \  
     value = %d\n",  
     event_stack[i].type,  
     event_stack[i].code,  
     event_stack[i].value);  
  }  
  else  
  {  
   perror(DEV_ENENT);  
  }  
 }  
 close(iFd);  
 return 0;  
}  

