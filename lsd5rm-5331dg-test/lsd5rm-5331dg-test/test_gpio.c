#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include "am1808_gpio.h"
#include "debug_console.h"
#include "test_gpio.h"

char p8_str_buf[512];

unsigned int g_u16_gpio_num_cmp[] = 
{
	1*32+16,     // 1*32+17,  
	1*32+18,     // 1*32+19,   
	1*32+20,     // 1*32+21, 
	1*32+22,     // 1*32+23, 
	1*32+24,     // 1*32+25, 
	1*32+26,     // 1*32+27, 
	3*32+5,      // 3*32+6, 
	1*32+8,      // 1*32+9, 
	0*32+14,     // 0*32+15, 
	0*32+6,      // 0*32+7, 
	0*32+2,      // 0*32+3, 
	0*32+29,     // 3*32+0, 
	0*32+4,      // 0*32+5,         
	
	1*32+17,     //1*32+16,     
  	1*32+19,     //1*32+18,     
  	1*32+21,     //1*32+20,     
  	1*32+23,     //1*32+22,     
  	1*32+25,     //1*32+24,     
  	1*32+27,     //1*32+26,     
  	3*32+6,      //3*32+5,      
  	1*32+9,      //1*32+8,      
  	0*32+15,     //0*32+14,     
  	0*32+7,      //0*32+6,      
  	0*32+3,      //0*32+2,      
  	3*32+0,      //0*32+29,     
  	0*32+5,      //0*32+4,    

#if 0
	// BBBB1
	8*16+0,
	8*16+1,
	8*16+2,
	8*16+3,
	8*16+4,
	// AAAA1
	0*16+0,
	0*16+1,
	0*16+2,
	0*16+3,
	0*16+4,
#endif
};


unsigned int g_u16_gpio_num[] = 
{
	1*32+17,     //1*32+16,     
  	1*32+19,     //1*32+18,     
  	1*32+21,     //1*32+20,     
  	1*32+23,     //1*32+22,     
  	1*32+25,     //1*32+24,     
  	1*32+27,     //1*32+26,     
  	3*32+6,      //3*32+5,      
  	1*32+9,      //1*32+8,      
  	0*32+15,     //0*32+14,     
  	0*32+7,      //0*32+6,      
  	0*32+3,      //0*32+2,      
  	3*32+0,      //0*32+29,     
  	0*32+5,      //0*32+4,  

	1*32+16,     // 1*32+17,  
	1*32+18,     // 1*32+19,   
	1*32+20,     // 1*32+21, 
	1*32+22,     // 1*32+23, 
	1*32+24,     // 1*32+25, 
	1*32+26,     // 1*32+27, 
	3*32+5,      // 3*32+6, 
	1*32+8,      // 1*32+9, 
	0*32+14,     // 0*32+15, 
	0*32+6,      // 0*32+7, 
	0*32+2,      // 0*32+3, 
	0*32+29,     // 3*32+0, 
	0*32+4,      // 0*32+5, 
#if 0
	// AAAA1
	0*16+0,
	0*16+1,
	0*16+2,
	0*16+3,
	0*16+4,
	// BBBB1
	8*16+0,
	8*16+1,
	8*16+2,
	8*16+3,
	8*16+4,
#endif
};



#define M_GPIO_NUM_CNT (sizeof(g_u16_gpio_num)/sizeof(unsigned int))

unsigned int g_u16_gpio_connect[M_GPIO_NUM_CNT];
int g_u16_gpio_connect_cnt = 0;

int fn_test_am1808_gpio(void)
{
	int i;
	int j;
	int value_tmp = 0;

	// test single gpio output
#if 0
	fn_set_am1808_gpio_direct(g_u16_gpio_num[0],M_GPIO_OUTPUT);
	fn_set_am1808_gpio_value(g_u16_gpio_num[0],M_GPIO_OUTPUT_LOW);
	printf("wait for gpio output\r\n");
	while(1);
#endif
	
	#if (DEBUG_TEST_GPIO >= 1)
	sprintf(p8_str_buf, "start test am1808 gpio,gpio_num_cnt=%d\r\n",M_GPIO_NUM_CNT);
	fn_test_uart_log_console_write(p8_str_buf);
	
	#endif
	// 循环每个引脚
	for(i = 0; i < M_GPIO_NUM_CNT; i++)
	{
		#if (DEBUG_TEST_GPIO >= 3)
		sprintf(p8_str_buf, "test gpio_num, GPIO%d[%d]\r\n",g_u16_gpio_num[i]/32,g_u16_gpio_num[i]%32);
		fn_test_uart_log_console_write(p8_str_buf);
		
		#endif	
		// 设定需要测试的引脚为输出 其他设定为输入
		for(j = 0; j < M_GPIO_NUM_CNT; j++)
		{
			if(j == i)
			{
				fn_set_am1808_gpio_direct(g_u16_gpio_num[j],M_GPIO_OUTPUT);
			}
			else
			{
				fn_set_am1808_gpio_direct(g_u16_gpio_num[j],M_GPIO_INPUT);
			}	
		}

		// 测试其余的gpio口
		g_u16_gpio_connect_cnt = 0;
		for(j = 0; j < M_GPIO_NUM_CNT; j++)
		{
			if(j == i)
			{
				continue;
			}		
#if 1	
			fn_set_am1808_gpio_value(g_u16_gpio_num[i],M_GPIO_OUTPUT_HIGH);
			value_tmp = fn_get_am1808_gpio_value(g_u16_gpio_num[j]);
			if(value_tmp != M_GPIO_OUTPUT_HIGH)
			{
				continue;
			}
#endif
	
			fn_set_am1808_gpio_value(g_u16_gpio_num[i],M_GPIO_OUTPUT_LOW);
			value_tmp = fn_get_am1808_gpio_value(g_u16_gpio_num[j]);
			if(value_tmp != M_GPIO_OUTPUT_LOW)
			{
				continue;
			}
			g_u16_gpio_connect[g_u16_gpio_connect_cnt] = g_u16_gpio_num[j];
			g_u16_gpio_connect_cnt++;
			#if (DEBUG_TEST_GPIO >= 3)
			sprintf(p8_str_buf, "test GPIO%d[%d] is same with GPIO%d[%d]\r\n",
							g_u16_gpio_num[i]/32,g_u16_gpio_num[i]%32,
							g_u16_gpio_num[j]/32,g_u16_gpio_num[j]%32);
			fn_test_uart_log_console_write(p8_str_buf);
			
			#endif
		}
		#if (DEBUG_TEST_GPIO >= 3)
		sprintf(p8_str_buf, "test gpio g_u16_gpio_connect_cnt=%d\r\n",g_u16_gpio_connect_cnt);
		fn_test_uart_log_console_write(p8_str_buf);
		
		#endif

		#if (DEBUG_TEST_GPIO >= 1)
		if(g_u16_gpio_connect_cnt == 1)
		{
			if(g_u16_gpio_connect[0] == g_u16_gpio_num_cmp[i])
			{
				sprintf(p8_str_buf, "OK:GPIO%d[%d] <---> GPIO%d[%d]\r\n",
					g_u16_gpio_num[i]/32,g_u16_gpio_num[i]%32,
					g_u16_gpio_connect[0]/32,g_u16_gpio_connect[0]%32);
				fn_test_uart_log_console_write(p8_str_buf);
				
			}
			else
			{
				sprintf(p8_str_buf, "ERROR:GPIO%d[%d] <---> GPIO%d[%d]\r\n",
					g_u16_gpio_num[i]/32,g_u16_gpio_num[i]%32,
					g_u16_gpio_connect[0]/32,g_u16_gpio_connect[0]%32);
				fn_test_uart_log_console_write(p8_str_buf);
				
			}
		}
		else if(g_u16_gpio_connect_cnt == 0)
		{
			sprintf(p8_str_buf, "ERROR:GPIO%d[%d] no connect,pls to GPIO%d[%d]\r\n",
					g_u16_gpio_num[i]/32,g_u16_gpio_num[i]%32,
					g_u16_gpio_num_cmp[i]/32,g_u16_gpio_num_cmp[i]%32);
			fn_test_uart_log_console_write(p8_str_buf);
			
		}
		else
		{
			int k;	
			sprintf(p8_str_buf, "ERROR:GPIO%d[%d] <---> ",
					g_u16_gpio_num[i]/32,g_u16_gpio_num[i]%32);	
			fn_test_uart_log_console_write(p8_str_buf);
				
			for(k = 0; k < g_u16_gpio_connect_cnt;k++)
			{
				sprintf(p8_str_buf, " GPIO%d[%d] ",
					g_u16_gpio_connect[i]/32,g_u16_gpio_connect[i]%32);
				fn_test_uart_log_console_write(p8_str_buf);
				
			}
			fn_test_uart_log_console_write("\r\n");
		}
		//fn_test_uart_log_console_write("\r\n");
		#endif
		
	}
}






