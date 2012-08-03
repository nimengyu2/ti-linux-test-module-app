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

int main(void) 
{ 
    int fd;
    
    // 定义按键值变量，对于驱动函数中的button_values[]数组 
    char buttons[4] = {'0', '0', '0', '0'}; 

    // 打开按键设备/dev/buttons
    fd = open("/dev/buttons", 0); 
    if(fd < 0)
    { 
        // 打开失败则退出  
        perror("ERROR: Can not open /dev/buttons"); 
        exit(1); 
    } 

    // 永读按键并打印键值和状态  
    for( ; ; )
    { 
        char current_buttons[4]; 
        int count_of_changed_key; 
        int i; 
        
        // 使用read函数读取一组按键值（4个）  
        if (read(fd, current_buttons, sizeof(current_buttons)) != sizeof(current_buttons))
        { 
            perror("ERROR: Can not read button values"); 
            exit(1); 
        } 

        // 逐个分析读取到的按键值  
        for(i=0, count_of_changed_key=0; i<sizeof(buttons)/sizeof(buttons[0]); i++)
        { 
            if(buttons[i] != current_buttons[i])
            { 
                buttons[i] = current_buttons[i];

                // 打印按键值，并标明按键按下/抬起的状态  
                printf("%s S%d is %s", count_of_changed_key? ", ": "", i+2, buttons[i] == '0' ? "UP" : "DOWN"); 
                count_of_changed_key++; 
            } 
        } 

        if (count_of_changed_key)
        {
            printf("\n"); 
        }    
    } 

    // 关闭按键设备文件  
    close(fd); 
    printf("Close /dev/buttons\n");

    return 0; 
}

