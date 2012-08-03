#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>


#define MOTOR_MAGIC 'L'
#define SET_LED2 _IOW(MOTOR_MAGIC, 2,int)
#define SET_LED3 _IOW(MOTOR_MAGIC, 3,int)
#define SET_LED4 _IOW(MOTOR_MAGIC, 4,int)
#define SET_LED5 _IOW(MOTOR_MAGIC, 5,int)

#define LED_DEVICE  "/dev/leds"
#define LED_ON   0
#define LED_OFF  1

int main(int argc, char **argv)
{ 
    int fd; 
    int i = 0;

    fd = open(LED_DEVICE, 0); 
    if (fd < 0) 
    { 
        printf("can't open /dev/leds!\n");
        exit(1);
    } 

    // init the LEDs
    ioctl(fd, SET_LED2, LED_OFF);
    ioctl(fd, SET_LED3, LED_OFF);
    ioctl(fd, SET_LED4, LED_OFF);
    ioctl(fd, SET_LED5, LED_OFF);

    while(1)
    {
        switch(i++)
        {
            case 0: ioctl(fd, SET_LED3, LED_ON);
                ioctl(fd, SET_LED4, LED_OFF);
                break;

            case 1: ioctl(fd, SET_LED2, LED_ON);
                ioctl(fd, SET_LED3, LED_OFF);
                break;

            case 2: ioctl(fd, SET_LED5, LED_ON);
                ioctl(fd, SET_LED2, LED_OFF);
                break;

            case 3: ioctl(fd, SET_LED4, LED_ON);
                ioctl(fd, SET_LED5, LED_OFF);
                break;

            default: break;
        }
        
        i %= 4;
        usleep(100000); // 100ms
    }

    close(fd); 
    return 0;
} 

