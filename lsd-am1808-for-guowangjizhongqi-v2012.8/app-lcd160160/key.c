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
#include "key.h"
#include "hello.h" 
#include "grlib.h"
#include "am1808_gpio.h"

//#define KEY_UP  0
//#define KEY_DOWN  5
//#define KEY_LEFT  2
//#define KEY_RIGHT  1
//#define KEY_OK  4
//#define KEY_CANCEL  3


// systick的全局计数变量 
unsigned long g_u32_systick_count = 0;
unsigned char g_u8_key_flag = 0;
unsigned char g_u8_key_num = 0;

// 主菜单下标
unsigned char g_u8_main_menu_index = 0;
// 本地数据显示下标
unsigned char g_u8_main_local_data_index = 0;
// 测量点显示下标
unsigned char g_u8_main_measure_data_index = 0;
// 参数设定管理下标
unsigned char g_u8_main_para_set_index = 0;
// 终端管理下标
unsigned char g_u8_main_term_set_index = 0;

// 测量点数据详细显示下标
unsigned char g_u8_main_measure_data_detial_index = 0;
// 循显选择项下标
unsigned char g_u8_main_measure_data_detial_circle_display_index = 0;
// 循显选择标志
unsigned char g_u8_main_measure_data_detial_circle_display_select_flag = 0;
// 循显标志
unsigned char g_u8_main_measure_data_detial_circle_display_flag = 0;

// 通道设置下标
unsigned char g_u8_main_cmt_channal_set_index = 0;
// 循显选择项下标
unsigned char g_u8_main_cmt_channal_set_circle_display_index = 0;
// 循显选择标志
unsigned char g_u8_main_cmt_channal_set_circle_display_select_flag = 0;
// 循显标志
unsigned char g_u8_main_cmt_channal_set_circle_display_flag = 0;

// 台区电表参数查看
unsigned char g_u8_main_area_meter_set_index = 0;

int  fd_buttons;  
unsigned char u8_old_buttons[6] = {1,1,1,1,1,1}; 
unsigned char u8_current_buttons[6] = {1,1,1,1,1,1};
unsigned char u8_up_buttions[6] = { 
				 KEY_UP,
				 KEY_DOWN,
				 KEY_LEFT,
				 KEY_RIGHT,
				 KEY_OK,
				 KEY_CANCEL};

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_systick_interrupt 
// 函数功能  :  systick中断服务函数
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
#if 0
void fn_systick_interrupt(void)
{
    unsigned long u32_i;  
      unsigned long u8_key_change_cnt;
       
        // 使用 read 函数读取一组按键值（6 个）  
        if (read(fd_buttons, u8_current_buttons, sizeof u8_current_buttons) != sizeof u8_current_buttons) 
       { 
            perror("read buttons:"); 
            exit(1); 
        } 

       // 逐个分析读取到的按键值  
        for (u32_i = 0, u8_key_change_cnt = 0; u32_i < sizeof(u8_old_buttons)/ sizeof(u8_old_buttons[0]); u32_i++) 
	{ 
             if (u8_old_buttons[u32_i] != u8_current_buttons[u32_i]) 
             { 
                 u8_old_buttons[u32_i] = u8_current_buttons[u32_i]; 
                 // 打印按键值，并标明按键按下/抬起的状态  
                // printf("%skey %d is %s", u8_key_change_cnt? ", ": "", u32_i, u8_old_buttons[u32_i] == '0' ? "down" : "up"); 
                 u8_key_change_cnt++; 
		 if(u8_current_buttons[u32_i] == '0')
		{ 
                      g_u8_key_flag = 1;
		      g_u8_key_num = u32_i;
                }
             } 
        } 
        if (u8_key_change_cnt) { 
            // printf("\n"); 
        }  
}
#endif




void fn_systick_interrupt(void)
{
	int i;
	for(i = 0;i < 6;i++)
	{
		u8_old_buttons[i] = u8_current_buttons[i] ;
	}
	u8_current_buttons[0] = fn_get_am1808_gpio_value(KEY_UP);
	u8_current_buttons[1] = fn_get_am1808_gpio_value(KEY_DOWN);
	u8_current_buttons[2] = fn_get_am1808_gpio_value(KEY_LEFT);
	u8_current_buttons[3] = fn_get_am1808_gpio_value(KEY_RIGHT);
	u8_current_buttons[4] = fn_get_am1808_gpio_value(KEY_OK);
	u8_current_buttons[5] = fn_get_am1808_gpio_value(KEY_CANCEL);

	for(i = 0;i < 6;i++)
	{
		if((u8_current_buttons[i] == 1) && (u8_old_buttons[i] == 0))
		{	
			g_u8_key_flag = 1;
		        g_u8_key_num = u8_up_buttions[i];
			break;
		}
	}
#if 0
	printf("cur[0]=%d,[1]=%d,[2]=%d,[3]=%d,[4]=%d,[5]=%d\n",
		u8_current_buttons[0],u8_current_buttons[1],u8_current_buttons[2],
		u8_current_buttons[3],u8_current_buttons[4],u8_current_buttons[5]);
	printf("old[0]=%d,[1]=%d,[2]=%d,[3]=%d,[4]=%d,[5]=%d\n",
		u8_old_buttons[0],u8_old_buttons[1],u8_old_buttons[2],
		u8_old_buttons[3],u8_old_buttons[4],u8_old_buttons[5]);
#endif
}


//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_key_process 
// 函数功能  :  按键处理
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void fn_key_process(void)
{
  
   usleep(50000); // 50ms
   fn_systick_interrupt(); 
   // 如果有按键按下
   if(g_u8_key_flag == 1)
   {
       // 清除按键标志
	   g_u8_key_flag = 0;
	   // 判断是什么按键
	   switch (g_u8_key_num)
	   {
case KEY_UP:
		   // 如果处于main下主目录下
		   if(g_u8_current_screen == M_SCREEN_MENU)
		   { 
		       fn_canvas_prev_index(&g_sMainMenuLocalDataDisplay,&g_u8_main_menu_index);
		   }
		   // 如果出于本地交采数据显示的时候
		   else if(g_u8_current_screen == M_SCREEN_LOCALDATADISPLAY)
		   {
		        // 因为第一屏只能显示6行数据
				if(g_u8_main_local_data_index >= 6)
				{
				   // 判断当前是否在第二屏中
				   WidgetRemove((tWidget *)&g_sMainLocalDataDisplaySecond);
				   WidgetAdd((tWidget *)&g_sMainLocalDataDisplayHead, 
				          (tWidget *)&g_sMainLocalDataDisplayFirst); 
				}

				// 链接第一屏和第二屏的数据
				WidgetLink((tWidget *)&g_sMainLocalDataDisplayFirstCCurrent, 
				          (tWidget *)&g_sMainLocalDataDisplaySecondAVoltageAnagle);
				// 选择上一个索引，已经相应的反白处理
				fn_canvas_prev_index(&g_sMainLocalDataDisplayFirstAVoltage,&g_u8_main_local_data_index);	
				// 释放链接
				WidgetUnLink((tWidget *)&g_sMainLocalDataDisplayFirstCCurrent, 
				          (tWidget *)&g_sMainLocalDataDisplaySecondAVoltageAnagle);
						  	  		    
				// 如果索引出于第二屏，则显示第二屏的数据
				if(g_u8_main_local_data_index >= 6)
				{
				   WidgetAdd((tWidget *)&g_sMainLocalDataDisplayHead, 
				          (tWidget *)&g_sMainLocalDataDisplaySecond); 
				}
		   }
		   // 处于测量点数据显示
		   else if(g_u8_current_screen == M_SCREEN_MEASUREDATADISPLAY)
		   { 
		       fn_canvas_prev_index(&g_sMainMeasureDataDisplayPoint1,&g_u8_main_measure_data_index); 
		   }
		   // 处于参数设定
		   else if(g_u8_current_screen == M_SCREEN_PARAMETERSETTING)
		   { 
		       fn_canvas_prev_index(&g_sMainParaSetCmtChannelSet,&g_u8_main_para_set_index); 
		   }
		   // 处于终端管理
		   else if(g_u8_current_screen == M_SCREEN_TERMINALMANAGER)
		   { 
		       fn_canvas_prev_index(&g_sMainTermSetAMRInfo,&g_u8_main_term_set_index); 
		   }
		   // 处于测量点详细显示界面
		   else if(g_u8_current_screen == M_SCREEN_MEASUREDATADETIAL)
		   {
		        // 处于设定循显
				if(g_u8_main_measure_data_detial_circle_display_select_flag == 1)
				{
					// 选择上一个索引，已经相应的反白处理
					fn_canvas_prev_index(&g_sMainMeasureDataDetialCircleConfirm,
								&g_u8_main_measure_data_detial_circle_display_index);	
				}
				// 正常情况下上下翻
				else
				{
	                // 因为第一屏只能显示6行数据
					if(g_u8_main_measure_data_detial_index >= 6)
					{
					   // 判断当前是否在第二屏中
					   WidgetRemove((tWidget *)&g_sMainMeasureDataDetialSecond);
					   WidgetAdd((tWidget *)&g_sMainMeasureDataDetialHead, 

					          (tWidget *)&g_sMainMeasureDataDetialFirst); 
					}
	
					// 链接第一屏和第二屏的数据
					WidgetLink((tWidget *)&g_sMainMeasureDataDetialFYJ, 
					          (tWidget *)&g_sMainMeasureDataDetialFYF);
					// 选择上一个索引，已经相应的反白处理
					fn_canvas_prev_index(&g_sMainMeasureDataDetialTime,&g_u8_main_measure_data_detial_index);	
					// 释放链接
					WidgetUnLink((tWidget *)&g_sMainMeasureDataDetialFYJ, 
					          (tWidget *)&g_sMainMeasureDataDetialFYF);
							  	  		    
					// 如果索引出于第二屏，则显示第二屏的数据
					if(g_u8_main_measure_data_detial_index >= 6)
					{
					   WidgetAdd((tWidget *)&g_sMainMeasureDataDetialHead, 
					          (tWidget *)&g_sMainMeasureDataDetialSecond); 
					}
				}		
		   }
		   else if(g_u8_current_screen == M_SCREEN_CMT_CHANNAL_SET)
		   {
		      	// 因为第一屏只能显示6行数据
				if(g_u8_main_cmt_channal_set_index >= 6)
				{
				   // 判断当前是否在第二屏中
				   WidgetRemove((tWidget *)&g_sMainCmtChannalSetSecond);
				   WidgetAdd((tWidget *)&g_sMainCmtChannalSetHead, 
				          (tWidget *)&g_sMainCmtChannalSetFirst); 
				}

				// 链接第一屏和第二屏的数据
				WidgetLink((tWidget *)&g_sMainCmtChannalSetUserName, 
				          (tWidget *)&g_sMainCmtChannalSetPasswd);
				// 选择上一个索引，已经相应的反白处理
				fn_canvas_prev_index(&g_sMainCmtChannalSetHostIP,&g_u8_main_cmt_channal_set_index);	
				// 释放链接
				WidgetUnLink((tWidget *)&g_sMainCmtChannalSetUserName, 
				          (tWidget *)&g_sMainCmtChannalSetPasswd);
						  	  		    
				// 如果索引出于第二屏，则显示第二屏的数据
				if(g_u8_main_cmt_channal_set_index >= 6)
				{
				   WidgetAdd((tWidget *)&g_sMainCmtChannalSetHead, 
				          (tWidget *)&g_sMainCmtChannalSetSecond); 
				}
		   }
		   // 台区电表参数查看
		   else if(g_u8_current_screen == M_SCREEN_AREA_METER_SET)
		   { 
		       fn_canvas_prev_index(&g_sMainAreaMeterSetPort1Interval,&g_u8_main_area_meter_set_index); 
		   }

		   break;
case KEY_DOWN:
           // 如果处于main下主目录下
		   if(g_u8_current_screen == M_SCREEN_MENU)
		   { 
		       fn_canvas_next_index(&g_sMainMenuLocalDataDisplay,&g_u8_main_menu_index); 
		   }
           // 如果出于本地交采数据显示的时候
		   else if(g_u8_current_screen == M_SCREEN_LOCALDATADISPLAY)
		   {
		        // 因为第一屏只能显示6行数据
				if(g_u8_main_local_data_index >= 6)
				{
				   // 判断当前是否在第二屏中
				   WidgetRemove((tWidget *)&g_sMainLocalDataDisplaySecond);
				   WidgetAdd((tWidget *)&g_sMainLocalDataDisplayHead, 
				          (tWidget *)&g_sMainLocalDataDisplayFirst); 
				}

				// 链接第一屏和第二屏的数据
				WidgetLink((tWidget *)&g_sMainLocalDataDisplayFirstCCurrent, 
				          (tWidget *)&g_sMainLocalDataDisplaySecondAVoltageAnagle);
				// 选择上一个索引，已经相应的反白处理
				fn_canvas_next_index(&g_sMainLocalDataDisplayFirstAVoltage,&g_u8_main_local_data_index);	
				// 释放链接
				WidgetUnLink((tWidget *)&g_sMainLocalDataDisplayFirstCCurrent, 
				          (tWidget *)&g_sMainLocalDataDisplaySecondAVoltageAnagle);
						  	  		    
				// 如果索引出于第二屏，则显示第二屏的数据
				if(g_u8_main_local_data_index >= 6)
				{
				   WidgetAdd((tWidget *)&g_sMainLocalDataDisplayHead, 
				          (tWidget *)&g_sMainLocalDataDisplaySecond); 
				}
		   }
		   // 处于测量点数据显示
		   else if(g_u8_current_screen == M_SCREEN_MEASUREDATADISPLAY)
		   { 
		       fn_canvas_next_index(&g_sMainMeasureDataDisplayPoint1,&g_u8_main_measure_data_index); 
		   }
		   // 处于参数查看
		   else if(g_u8_current_screen == M_SCREEN_PARAMETERSETTING)
		   { 
		       fn_canvas_next_index(&g_sMainParaSetCmtChannelSet,&g_u8_main_para_set_index); 
		   }
		   // 处于终端管理
		   else if(g_u8_current_screen == M_SCREEN_TERMINALMANAGER)
		   { 
		       fn_canvas_next_index(&g_sMainTermSetAMRInfo,&g_u8_main_term_set_index); 
		   }
		   // 处于测量点详细显示界面
		   else if(g_u8_current_screen == M_SCREEN_MEASUREDATADETIAL)
		   {
		        // 处于设定循显
				if(g_u8_main_measure_data_detial_circle_display_select_flag == 1)
				{
					// 选择上一个索引，已经相应的反白处理
					fn_canvas_next_index(&g_sMainMeasureDataDetialCircleConfirm,
								&g_u8_main_measure_data_detial_circle_display_index);	
				}
				// 正常情况下上下翻
				else
				{
				    // 因为第一屏只能显示6行数据
					if(g_u8_main_measure_data_detial_index >= 6)
					{
					   // 判断当前是否在第二屏中
					   WidgetRemove((tWidget *)&g_sMainMeasureDataDetialSecond);
					   WidgetAdd((tWidget *)&g_sMainMeasureDataDetialHead, 
					          (tWidget *)&g_sMainMeasureDataDetialFirst); 
					}
	
					// 链接第一屏和第二屏的数据
					WidgetLink((tWidget *)&g_sMainMeasureDataDetialFYJ, 
					          (tWidget *)&g_sMainMeasureDataDetialFYF);
					// 选择上一个索引，已经相应的反白处理
					fn_canvas_next_index(&g_sMainMeasureDataDetialTime,&g_u8_main_measure_data_detial_index);	
					// 释放链接
					WidgetUnLink((tWidget *)&g_sMainMeasureDataDetialFYJ, 
					          (tWidget *)&g_sMainMeasureDataDetialFYF);
							  	  		    
					// 如果索引出于第二屏，则显示第二屏的数据
					if(g_u8_main_measure_data_detial_index >= 6)
					{
					   WidgetAdd((tWidget *)&g_sMainMeasureDataDetialHead, 
					          (tWidget *)&g_sMainMeasureDataDetialSecond); 
					}
				}
		   }
		   else if(g_u8_current_screen == M_SCREEN_CMT_CHANNAL_SET)
		   {
		      	// 因为第一屏只能显示6行数据
				if(g_u8_main_cmt_channal_set_index >= 6)
				{
				   // 判断当前是否在第二屏中
				   WidgetRemove((tWidget *)&g_sMainCmtChannalSetSecond);
				   WidgetAdd((tWidget *)&g_sMainCmtChannalSetHead, 
				          (tWidget *)&g_sMainCmtChannalSetFirst); 
				}

				// 链接第一屏和第二屏的数据
				WidgetLink((tWidget *)&g_sMainCmtChannalSetUserName, 
				          (tWidget *)&g_sMainCmtChannalSetPasswd);
				// 选择上一个索引，已经相应的反白处理
				fn_canvas_next_index(&g_sMainCmtChannalSetHostIP,&g_u8_main_cmt_channal_set_index);	
				// 释放链接
				WidgetUnLink((tWidget *)&g_sMainCmtChannalSetUserName, 
				          (tWidget *)&g_sMainCmtChannalSetPasswd);
						  	  		    
				// 如果索引出于第二屏，则显示第二屏的数据
				if(g_u8_main_cmt_channal_set_index >= 6)
				{
				   WidgetAdd((tWidget *)&g_sMainCmtChannalSetHead, 
				          (tWidget *)&g_sMainCmtChannalSetSecond); 
				}
		   }
		   // 台区电表参数查看
		   else if(g_u8_current_screen == M_SCREEN_AREA_METER_SET)
		   { 
		       fn_canvas_next_index(&g_sMainAreaMeterSetPort1Interval,&g_u8_main_area_meter_set_index); 
		   }

		   break;
case KEY_OK:
		   // 如果处于main下主目录下，此时按下ok键将会进入其中的子菜单中
		   if(g_u8_current_screen == M_SCREEN_MENU)
		   {
		      // 本地交采数据显示
			  if(g_u8_main_menu_index == 0)
			  {
			   	  g_u8_current_screen = M_SCREEN_LOCALDATADISPLAY;
				  WidgetRemove((tWidget *)&g_sMainMenu);
				  WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainLocalDataDisplay);
			  }
			  // 测量点数据显示
			  else if(g_u8_main_menu_index == 1)
			  {
			   	  g_u8_current_screen = M_SCREEN_MEASUREDATADISPLAY;
				  WidgetRemove((tWidget *)&g_sMainMenu);
				  WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainMeasureDataDisplay);
			  }
			  // 参数设置和查看
			  else if (g_u8_main_menu_index == 2)
			  {
			      g_u8_current_screen = M_SCREEN_PARAMETERSETTING;
				  WidgetRemove((tWidget *)&g_sMainMenu);
				  WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainParaSet);
			  }
			  // 终端管理与维护
			  else if (g_u8_main_menu_index == 3)
			  {
			      g_u8_current_screen = M_SCREEN_TERMINALMANAGER;
				  WidgetRemove((tWidget *)&g_sMainMenu);
				  WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainTermSet);
			  }
		   }
		   // 如果处于开始界面，此时按下ok键，则进入主菜单
		   else if(g_u8_current_screen == M_SCREEN_FIRST_FACE)
		   {
		      g_u8_current_screen = M_SCREEN_MENU;
			  WidgetRemove((tWidget *)&g_sMainFirstFace);
			  WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainMenu);
		   }
		   // 如果出于测量点数据显示界面，则进入需要显示的测量点详细显示界面
		   else if(g_u8_current_screen == M_SCREEN_MEASUREDATADISPLAY)
		   {
		      // 进入显示详情
			  g_u8_current_screen = M_SCREEN_MEASUREDATADETIAL;
			  WidgetRemove((tWidget *)&g_sMainMeasureDataDisplay);
			  WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainMeasureDataDetial);
			  // 设定显示的值
			  g_sMainMeasureDataDetialZYJ.pFont = (const tFont*)&g_f64_measure_data_point_zyj[g_u8_main_measure_data_index];
  			  g_sMainMeasureDataDetialZYF.pFont = (const tFont*)&g_f64_measure_data_point_zyf[g_u8_main_measure_data_index];
			  g_sMainMeasureDataDetialZYP.pFont = (const tFont*)&g_f64_measure_data_point_zyp[g_u8_main_measure_data_index];
			  g_sMainMeasureDataDetialZYG.pFont = (const tFont*)&g_f64_measure_data_point_zyg[g_u8_main_measure_data_index];
			  g_sMainMeasureDataDetialFYJ.pFont = (const tFont*)&g_f64_measure_data_point_fyj[g_u8_main_measure_data_index];
			  g_sMainMeasureDataDetialFYF.pFont = (const tFont*)&g_f64_measure_data_point_fyf[g_u8_main_measure_data_index];
			  g_sMainMeasureDataDetialFYP.pFont = (const tFont*)&g_f64_measure_data_point_fyp[g_u8_main_measure_data_index];
			  g_sMainMeasureDataDetialFYG.pFont = (const tFont*)&g_f64_measure_data_point_fyg[g_u8_main_measure_data_index];
			  g_sMainMeasureDataDetialTime.pcText = (const char*)(g_pu8_measure_data_point_time[g_u8_main_measure_data_index]);
		   }
		   // 处于循环显示
		   else if(g_u8_current_screen == M_SCREEN_MEASUREDATADETIAL)
		   {
		       if(g_u8_main_measure_data_detial_circle_display_select_flag == 0)
			   {
			       g_u8_main_measure_data_detial_circle_display_select_flag = 1;

				   // 链接第一屏和第二屏的数据
				   WidgetLink((tWidget *)&g_sMainMeasureDataDetialHead, 
					          (tWidget *)&g_sMainMeasureDataDetialCircleGround); 
			   }
			   else if(g_u8_main_measure_data_detial_circle_display_select_flag == 1)
			   {
			      g_u8_main_measure_data_detial_circle_display_flag = 
				             (g_u8_main_measure_data_detial_circle_display_index?0:1); 
				  // 链接第一屏和第二屏的数据
				  WidgetUnLink((tWidget *)&g_sMainMeasureDataDetialHead, 
					          (tWidget *)&g_sMainMeasureDataDetialCircleGround); 
				  g_u8_main_measure_data_detial_circle_display_select_flag = 0;
			   }
		   }
		   // 参数设置和查看
		   else if(g_u8_current_screen == M_SCREEN_PARAMETERSETTING)
		   {
		      // 通讯通道设置
			  if(g_u8_main_para_set_index == 0)
			  {
			     g_u8_current_screen = M_SCREEN_CMT_CHANNAL_SET;
				 WidgetRemove((tWidget *)&g_sMainParaSet);
				 WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainCmtChannalSet);
			  }
			  // 台区电表参数查看
			  if(g_u8_main_para_set_index == 1)
			  {
			     g_u8_current_screen = M_SCREEN_AREA_METER_SET;
				 WidgetRemove((tWidget *)&g_sMainParaSet);
				 WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainAreaMeterSet);
			  }
		   }
		   else if(g_u8_current_screen == M_SCREEN_TERMINALMANAGER)
		   {
		      if(g_u8_main_term_set_index == 0)
			  {
			      g_u8_current_screen = M_SCREEN_AMR_INFO;
				  WidgetRemove((tWidget *)&g_sMainTermSet);
				  WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainAMRInfo);
			  } 
		   }
		   break;
case KEY_CANCEL:
		   // 从本地采样数据显示返回
		   if(g_u8_current_screen == M_SCREEN_LOCALDATADISPLAY)
		   {
                g_u8_current_screen = M_SCREEN_MENU; 
				WidgetRemove((tWidget *)&g_sMainLocalDataDisplay);
				WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainMenu);
		   }
		   // 从测量点数据显示返回
		   else if(g_u8_current_screen == M_SCREEN_MEASUREDATADISPLAY)
		   {
		        g_u8_current_screen = M_SCREEN_MENU;
				WidgetRemove((tWidget *)&g_sMainMeasureDataDisplay);
				WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainMenu);
		   }
		   // 从参数查看显示返回
		   else if(g_u8_current_screen == M_SCREEN_PARAMETERSETTING)
		   {
		        g_u8_current_screen = M_SCREEN_MENU;
				WidgetRemove((tWidget *)&g_sMainParaSet);
				WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainMenu);
		   }
		   // 从终端管理显示返回
		   else if(g_u8_current_screen == M_SCREEN_TERMINALMANAGER)
		   {
		        g_u8_current_screen = M_SCREEN_MENU;
				WidgetRemove((tWidget *)&g_sMainTermSet);
				WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainMenu);
		   }
		   // 从主菜单显示返回
		   else if(g_u8_current_screen == M_SCREEN_MENU)
		   {
		      g_u8_current_screen = M_SCREEN_FIRST_FACE;
			  WidgetRemove((tWidget *)&g_sMainMenu);
			  WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainFirstFace);
		   }
		   // 测量点数据详细显示
		   else if(g_u8_current_screen == M_SCREEN_MEASUREDATADETIAL)
		   {
		      // 如果处于选择循显标志的时候，按下取消键返回
			  if(g_u8_main_measure_data_detial_circle_display_select_flag == 1)
			  {
			      // 断开循显选择块
				  WidgetUnLink((tWidget *)&g_sMainMeasureDataDetialHead, 
					          (tWidget *)&g_sMainMeasureDataDetialCircleGround);
				  // 清除选择循环显示标志过程 
				  g_u8_main_measure_data_detial_circle_display_select_flag = 0;
			  }
			  else
			  {
				  // 测试点数据显示返回
				  g_u8_current_screen = M_SCREEN_MEASUREDATADISPLAY;
				  WidgetRemove((tWidget *)&g_sMainMeasureDataDetial);
				  WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainMeasureDataDisplay);
			  }  
		   }
		   // 通讯通道设置
		   else if(g_u8_current_screen == M_SCREEN_CMT_CHANNAL_SET)
		   {
		      g_u8_current_screen = M_SCREEN_PARAMETERSETTING;
			  WidgetRemove((tWidget *)&g_sMainCmtChannalSet);
			  WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainParaSet);
		   }
		   // 台区电表参数查看
		   else if(g_u8_current_screen == M_SCREEN_AREA_METER_SET)
		   { 
		        g_u8_current_screen = M_SCREEN_PARAMETERSETTING;
				WidgetRemove((tWidget *)&g_sMainAreaMeterSet);
				WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainParaSet);
		   }
		   else if(g_u8_current_screen == M_SCREEN_AMR_INFO)
		   {
		         g_u8_current_screen = M_SCREEN_TERMINALMANAGER;
				 WidgetRemove((tWidget *)&g_sMainAMRInfo);
				 WidgetAdd((tWidget *)&g_sMain, (tWidget *)&g_sMainTermSet); 
		   }
		   break;
		   default:
		   break;
	   }
	   WidgetPaint(WIDGET_ROOT);
   }    
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_systick_init 
// 函数功能  :  systick初始化
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void fn_systick_init(void)
{

}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_key_init 
// 函数功能  :  按键初始化
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void fn_key_init(void)
{
        // 打开按键设备/dev/buttons  
     fd_buttons = open("/dev/buttons", 0); 
     if (fd_buttons < 0) { 
         // 打开失败则退出  
         perror("open device buttons"); 
         exit(1); 
     } 
}
