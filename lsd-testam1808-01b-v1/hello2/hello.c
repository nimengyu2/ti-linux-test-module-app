#include "grlib.h"
#include "widget.h"
#include "canvas.h"
#include "pushbutton.h"
#include "formike128x128x16.h"
#include "gb_print.h"
#include "key.h"   
#include "hello.h" 

unsigned char g_u8_current_screen = M_SCREEN_FIRST_FACE;
//-----------------------------------------------------------------------------
// 第0屏 开始界面
//-----------------------------------------------------------------------------
unsigned long g_u32_year = 2045;	     // 当前日期--年
unsigned long g_u32_month = 10;			 // 当前日期--月
unsigned long g_u32_date = 20;			 // 当前日期--日
unsigned long g_u32_hour = 16;			 // 当前时间--时
unsigned long g_u32_minute = 15;		 // 当前时间--分
unsigned long g_u32_second = 1;			 // 当前时间--秒
unsigned char g_pu8_date[] = "yyyy-mm-dd"; // 当前日期--字符型
unsigned char g_pu8_time[] = "hh:mm:ss";   // 当前时间--字符型
//-----------------------------------------------------------------------------
// 第二屏 本地交采数据显示
//-----------------------------------------------------------------------------
double g_f64_local_data_a_voltage = 0.02;	     // A相电压
double g_f64_local_data_b_voltage = 221.15;		 // B相电压
double g_f64_local_data_c_voltage = -240.03;	 // C相电压
double g_f64_local_data_a_current = -1.23;		 // A相电流
double g_f64_local_data_b_current = -0.21;		 // B相电流
double g_f64_local_data_c_current = 0.02;		 // C相电流
double g_f64_local_data_a_voltage_angle = 2.10;	 // A相电压相角
double g_f64_local_data_b_voltage_angle = -40.8; // B相电压相角
double g_f64_local_data_c_voltage_angle = -30.6; // C相电压相角
//-----------------------------------------------------------------------------
// 第四屏 测量点数据详细显示
//-----------------------------------------------------------------------------
unsigned char g_pu8_measure_data_point_num[] = "测量点n";
unsigned char g_pu8_measure_data_point_time_hour[M_MEASURE_DATA_POINT_NUM] = {12,12,12,12,12,12};	   // 测量点--抄表时间--时
unsigned char g_pu8_measure_data_point_time_minute[M_MEASURE_DATA_POINT_NUM] = {12,10,9,8,7,6};  // 测量点--抄表时间--分
unsigned char g_pu8_measure_data_point_time[M_MEASURE_DATA_POINT_NUM][8] = {"hh:mm","hh:mm","hh:mm"
									,"hh:mm","hh:mm","hh:mm"}; // 测量点--抄表时间--字符型
double g_f64_measure_data_point_zyj[M_MEASURE_DATA_POINT_NUM] = {123456.78,
								159.66,15.06,1.456,0.123,0.015};	  // 测量点--正向有功尖电量
double g_f64_measure_data_point_zyf[M_MEASURE_DATA_POINT_NUM] = {123456.78,
								159.66,15.06,1.456,0.123,0.015};	  // 测量点--正向有功峰电量
double g_f64_measure_data_point_zyp[M_MEASURE_DATA_POINT_NUM] = {123456.78,
								159.66,15.06,1.456,0.123,0.015};	  // 测量点--正向有功平电量
double g_f64_measure_data_point_zyg[M_MEASURE_DATA_POINT_NUM] = {123456.78,
								159.66,15.06,1.456,0.123,0.015};	  // 测量点--正向有功谷电量
double g_f64_measure_data_point_fyj[M_MEASURE_DATA_POINT_NUM] = {123456.78,
								159.66,15.06,1.456,0.123,0.015};	  // 测量点--反向有功尖电量
double g_f64_measure_data_point_fyf[M_MEASURE_DATA_POINT_NUM] = {123456.78,
								159.66,15.06,1.456,0.123,0.015};	  // 测量点--反向有功峰电量
double g_f64_measure_data_point_fyp[M_MEASURE_DATA_POINT_NUM] = {123456.78,
								159.66,15.06,1.456,0.123,0.015};	  // 测量点--反向有功平电量
double g_f64_measure_data_point_fyg[M_MEASURE_DATA_POINT_NUM] = {123456.78,
								159.66,15.06,1.456,0.123,0.015};	  // 测量点--反向有功谷电量
//-----------------------------------------------------------------------------
// 第六屏 通讯通道设置
//-----------------------------------------------------------------------------
unsigned long g_u32_cmt_channal_set_host_ip = (((unsigned long)192<<24)|(168<<16)|(14<<8)|20);
unsigned long g_u32_cmt_channal_set_local_ip = (((unsigned long)192<<24)|(168<<16)|(14<<8)|21);
unsigned long g_u32_cmt_channal_set_term_ip = (((unsigned long)192<<24)|(168<<16)|(14<<8)|22);
double g_f64_cmt_channal_set_port_num = 5001;
unsigned char g_pu8_cmt_channal_set_apn[10] = "CMNET";
unsigned char g_pu8_cmt_channal_set_username[10] = "yuge";
unsigned char g_pu8_cmt_channal_set_passwd[10] = "000000";
double g_f64_cmt_channal_set_period = 0.3; // 单位ms 

//-----------------------------------------------------------------------------
// 第七屏 台区电表参数查看
//-----------------------------------------------------------------------------
double g_f64_area_meter_set_port1_interval = 0.3; // 单位ms 
double g_f64_area_meter_set_port2_interval = 0.5; // 单位ms 
double g_f64_area_meter_set_port3_interval = 0.4; // 单位ms 

//-----------------------------------------------------------------------------
// 第九屏 集中器版本
//-----------------------------------------------------------------------------
unsigned char g_pu8_amr_info_soft_version[20] = "10.03.26.09";

//void OnButtonPress(tWidget *pWidget);
//void OnSwUpdateButtonPress(tWidget *pWidget);

// WIDGET_ROOT
//      |
// g_sBackground--g_sHeading 
//                        |--> g_sHeadSignal --> g_sHeadCmtGscWay --> 
//                             g_sHeadWarningFlag --> g_sHeadStandard --> 
//                             g_sHeadShowMesData --> g_sHeadCurrentTime
//              --g_sMain
//                 |--> g_sMainFirstFace
//                       |--> g_sMainFirstFaceHead --> g_sMainFirstFaceDate --> g_sMainFirstFaceTime 
//                 |--> g_sMainMenu 
//                       |-->  g_sMainMenuHead --> g_sMainMenuLocalDataDisplay -->
//                             g_sMainMenuMeasureDataDisplay --> g_sMainMenuParameterSetting -->
//                             g_sMainMenuTerminalManager
//                 |--g_sMainLocalDataDisplay
//                       |--> g_sMainLocalDataDisplayHead |--> g_sMainLocalDataDisplayFirst 
//                                                                |--> g_sMainLocalDataDisplayFirstAVoltage --> 
//                                                                     g_sMainLocalDataDisplayFirstBVoltage -->
//                                                                     g_sMainLocalDataDisplayFirstCVoltage -->
//                                                                     g_sMainLocalDataDisplayFirstACurrent --> 
//                                                                     g_sMainLocalDataDisplayFirstBCurrent --> 
//                                                                     g_sMainLocalDataDisplayFirstCCurrent 
//                                                                        |--> g_sMainLocalDataDisplayFirstNum   
//                                                         |--> g_sMainLocalDataDisplaySecond 
//                                                                |--> g_sMainLocalDataDisplayFirstAVoltageAnagle --> 
//                                                                     g_sMainLocalDataDisplayFirstBVoltageAnagle -->
//                                                                     g_sMainLocalDataDisplayFirstCVoltageAnagle
//                                                                        |--> g_sMainLocalDataDisplaySecondNum
//                 |-->g_sMainMeasureDataDisplay
//                       |--> g_sMainMeasureDataDisplayHead --> g_sMainMeasureDataDisplayPoint1 -->
//                                                              g_sMainMeasureDataDisplayPoint2 --> 
//                                                              g_sMainMeasureDataDisplayPoint3 --> 
//                                                              g_sMainMeasureDataDisplayPoint4 --> 
//                                                              g_sMainMeasureDataDisplayPoint5 --> 
//                                                              g_sMainMeasureDataDisplayPoint6
//                                                                |--> g_sMainMeasureDataDisplayNum 
//                 |-->g_sMainParaSet
//                       |--> g_sMainParaSetHead --> g_sMainParaSetCmtChannelSet -->
//                                                   g_sMainParaSetAreaMeterSet --> 
//                                                   g_sMainParaSetCentralMeterSet --> 
//                                                   g_sMainParaSetTimeSet --> 
//                                                   g_sMainParaSetPasswdSet --> 
//                                                   g_sMainParaSetNumberSet 
//                                                     |--> g_sMainParaSetNum 
//                |-->g_sMainMeasureDataDetial
//                     |--> g_sMainMeasureDataDetialHead |--> g_sMainMeasureDataDetialFirst
//                                                               |--> g_sMainMeasureDataDetialTimeInfo -->
//                                                                    g_sMainMeasureDataDetialTime -->
//                                                                    g_sMainMeasureDataDetialZYJ -->
//                                                                    g_sMainMeasureDataDetialZYF -->
//                                                                    g_sMainMeasureDataDetialZYP -->
//                                                                    g_sMainMeasureDataDetialZYG -->
//                                                                    g_sMainMeasureDataDetialFYJ 
//                                                                      |-->g_sMainMeasureDataDetialFirstNum 
//                                                       |--> g_sMainMeasureDataDetialSecond 
//                                                                |--> g_sMainMeasureDataDetialFYF -->
//                                                                     g_sMainMeasureDataDetialFYP -->
//                                                                     g_sMainMeasureDataDetialFYG 
//                                                                         |-->g_sMainMeasureDataDetialSecondNum
                                                             
//              -->g_sMainInfoGround
//                       |--> g_sMainInfo

//-----------------------------------------------------------------------------
// 总体背景
//-----------------------------------------------------------------------------
//总体背景
Canvas(g_sBackground, WIDGET_ROOT, 0, &g_sHeading,
       &g_sFormike128x128x16, 0, 0, 160, 160,
       CANVAS_STYLE_FILL | PB_STYLE_OUTLINE, ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);

//-----------------------------------------------------------------------------
// 顶部信息栏
//-----------------------------------------------------------------------------
// 顶部的信息栏，保持不变
Canvas(g_sHeading, &g_sBackground, &g_sMain, &g_sHeadSignal,
       &g_sFormike128x128x16, 0, 0, 160, 24,
       (CANVAS_STYLE_FILL | PB_STYLE_OUTLINE),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);

// 信号强度
Canvas(g_sHeadSignal, &g_sHeading, &g_sHeadCmtGscWay, 0,
       &g_sFormike128x128x16, 1, 2, 20, 20,
       (CANVAS_STYLE_IMG ),
       ClrWhite, ClrBlack, ClrBlack, &g_sFontMsyh16, 0, g_pucsignal20x20, 0);

// 通信方式选择 G S C 
Canvas(g_sHeadCmtGscWay, &g_sHeading, &g_sHeadWarningFlag, 0,
       &g_sFormike128x128x16, 22, 2, 20, 20,
       (CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_TOP),
       ClrWhite, ClrBlack, ClrBlack, &g_sFontCmss26, "G", 0, 0);

// 异常警告
CircularButton(g_sHeadWarningFlag,&g_sHeading, &g_sHeadStandard, 0,
               &g_sFormike128x128x16, 52, 11, 9,
               PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT | CANVAS_STYLE_TEXT_LEFT,
               ClrWhite, ClrWhite, ClrBlack, ClrBlack,
               &g_sFontCmss16b, "!", 0, 0, 0, 0, 0);
// 标准
Canvas(g_sHeadStandard, &g_sHeading, &g_sHeadShowMesData, 0,
       &g_sFormike128x128x16, 66, 2, 28, 20,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_TOP),
       ClrWhite, ClrBlack, ClrBlack, &g_sFontCmss18, "Std", 0, 0);

// 轮显示第几组数据
Canvas(g_sHeadShowMesData, &g_sHeading, &g_sHeadCurrentTime, 0,
       &g_sFormike128x128x16, 96, 2, 20, 20,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
       ClrWhite, ClrBlack, ClrBlack, &g_sFontCmss18b, "01", 0, 0);

// 当前时间
Canvas(g_sHeadCurrentTime, &g_sHeading, 0, 0,
       &g_sFormike128x128x16, 122, 2, 37, 20,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_BOTTOM),
       ClrWhite, ClrBlack, ClrBlack, &g_sFontCmss12b, "12:00", 0, 0);

//-----------------------------------------------------------------------------
// 主要显示区域
//-----------------------------------------------------------------------------
// 主要显示区域
Canvas(g_sMain, &g_sBackground, &g_sMainInfoGround, &g_sMainFirstFace,
       &g_sFormike128x128x16, 1, 25, 158, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);

// 正在抄表状态的显示背景
Canvas(g_sMainInfoGround, &g_sBackground, 0,&g_sMainInfo,
       &g_sFormike128x128x16, 0, 142, 160, 18,
       (CANVAS_STYLE_OUTLINE | CANVAS_STYLE_FILL),
       ClrWhite, 0, ClrBlack, 0, 0, 0, OnPaintMainCharacter);

// 正在抄表状态的显示状态
Canvas(g_sMainInfo, &g_sMainInfoGround, 0,0,
       &g_sFormike128x128x16, 1, 143, 158, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "终端正在抄表..", 0, OnPaintMainCharacter);


//-----------------------------------------------------------------------------
// 第0屏 开始界面
//-----------------------------------------------------------------------------
// 第0屏 开始界面
Canvas(g_sMainFirstFace, &g_sMain, 0, &g_sMainFirstFaceHead,
       &g_sFormike128x128x16, 1, 25, 158, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);

Canvas(g_sMainFirstFaceHead, &g_sMainFirstFace, &g_sMainFirstFaceDate,0,
       &g_sFormike128x128x16, 1+8*3, 25, 134, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "低压集抄集中器", 0, OnPaintMainCharacter);

Canvas(g_sMainFirstFaceDate, &g_sMainFirstFace, &g_sMainFirstFaceTime,0,
       &g_sFormike128x128x16, 1+8*5, 25+16+1+16+1, 5*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, (const char*)g_pu8_date, 0, OnPaintMainFirstFaceDate);

Canvas(g_sMainFirstFaceTime, &g_sMainFirstFace, 0,0,
       &g_sFormike128x128x16, 1+8*6, 25+16+1+16+1+16+1, 4*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, (const char*)g_pu8_time, 0, OnPaintMainFirstFaceTime);

//-----------------------------------------------------------------------------
// 第一屏 主菜单
//-----------------------------------------------------------------------------
// 第一屏 主菜单
Canvas(g_sMainMenu, &g_sMain, 0, &g_sMainMenuHead,
       &g_sFormike128x128x16, 1, 25, 158, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);

Canvas(g_sMainMenuHead, &g_sMainMenu, &g_sMainMenuLocalDataDisplay,0,
       &g_sFormike128x128x16, 1+8*6, 25, 16*3, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "主菜单", 0, OnPaintMainCharacter);

Canvas(g_sMainMenuLocalDataDisplay, &g_sMainMenu, &g_sMainMenuMeasureDataDisplay,0,
       &g_sFormike128x128x16, 1+16, 42, 8*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN | CANVAS_STYLE_REVERSE_COLOR),
       ClrWhite, 0, ClrBlack, 0, "本地交采数据显示", 0, OnPaintMainCharacter);

Canvas(g_sMainMenuMeasureDataDisplay, &g_sMainMenu, &g_sMainMenuParameterSetting,0,
       &g_sFormike128x128x16, 1+24, 59, 7*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "测量点数据显示", 0, OnPaintMainCharacter);

Canvas(g_sMainMenuParameterSetting, &g_sMainMenu, &g_sMainMenuTerminalManager, 0,
       &g_sFormike128x128x16, 1+24, 76, 7*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "相关参数的查看", 0, OnPaintMainCharacter);

Canvas(g_sMainMenuTerminalManager, &g_sMainMenu, 0, 0,
       &g_sFormike128x128x16, 1+24, 93, 7*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "终端管理与维护", 0, OnPaintMainCharacter);

//-----------------------------------------------------------------------------
// 第二屏 本地交采数据显示
//-----------------------------------------------------------------------------
// 第二屏 本地交采数据显示
Canvas(g_sMainLocalDataDisplay, &g_sMain, 0, &g_sMainLocalDataDisplayHead,
       &g_sFormike128x128x16, 1, 25, 157, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);

// 菜单栏
Canvas(g_sMainLocalDataDisplayHead, &g_sMainLocalDataDisplay, 0, &g_sMainLocalDataDisplayFirst,
       &g_sFormike128x128x16, 1+8, 25, 8*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "本地交采数据显示", 0, OnPaintMainCharacter);

// 显示数据的第一屏
Canvas(g_sMainLocalDataDisplayFirst, &g_sMainLocalDataDisplayHead, 0, &g_sMainLocalDataDisplayFirstAVoltage,
		&g_sFormike128x128x16, 1, 42, 157, 117,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// A相电压
Canvas(g_sMainLocalDataDisplayFirstAVoltage, &g_sMainLocalDataDisplayFirst, &g_sMainLocalDataDisplayFirstBVoltage, 0,
       &g_sFormike128x128x16, 1, 42, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN | CANVAS_STYLE_REVERSE_COLOR),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_local_data_a_voltage, " A相电压:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// B相电压
Canvas(g_sMainLocalDataDisplayFirstBVoltage, &g_sMainLocalDataDisplayFirst, &g_sMainLocalDataDisplayFirstCVoltage, 0,
       &g_sFormike128x128x16, 1, 59, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_local_data_b_voltage, " B相电压:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// C相电压
Canvas(g_sMainLocalDataDisplayFirstCVoltage, &g_sMainLocalDataDisplayFirst, &g_sMainLocalDataDisplayFirstACurrent, 0,
       &g_sFormike128x128x16, 1, 76, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_local_data_c_voltage, " C相电压:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// A相电流
Canvas(g_sMainLocalDataDisplayFirstACurrent, &g_sMainLocalDataDisplayFirst, &g_sMainLocalDataDisplayFirstBCurrent, 0,
       &g_sFormike128x128x16, 1, 93, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_local_data_a_current, " A相电流:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// B相电流
Canvas(g_sMainLocalDataDisplayFirstBCurrent, &g_sMainLocalDataDisplayFirst, &g_sMainLocalDataDisplayFirstCCurrent, 0,
       &g_sFormike128x128x16, 1, 109, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_local_data_b_current, " B相电流:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// C相电流
Canvas(g_sMainLocalDataDisplayFirstCCurrent, &g_sMainLocalDataDisplayFirst, 0, &g_sMainLocalDataDisplayFirstNum,
       &g_sFormike128x128x16, 1, 126, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_local_data_c_current, " C相电流:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// 显示当前所在屏为第一屏
Canvas(g_sMainLocalDataDisplayFirstNum, &g_sMainLocalDataDisplayFirstCCurrent, 0,0 ,
       &g_sFormike128x128x16, 135, 25, 24, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "1/2", 
	   0, OnPaintMainCharacter);

// 显示数据的第二屏
Canvas(g_sMainLocalDataDisplaySecond, &g_sMainLocalDataDisplayHead, 0, &g_sMainLocalDataDisplaySecondAVoltageAnagle,
       &g_sFormike128x128x16, 1, 42, 157, 117,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// A相电压相角
Canvas(g_sMainLocalDataDisplaySecondAVoltageAnagle, &g_sMainLocalDataDisplaySecond, &g_sMainLocalDataDisplaySecondBVoltageAnagle, 0,
       &g_sFormike128x128x16, 1, 42, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_local_data_a_voltage_angle, " A相电压相角:", 
	   (const unsigned char*)1, OnPaintMainLocalDataDisplay);
// B相电压相角
Canvas(g_sMainLocalDataDisplaySecondBVoltageAnagle, &g_sMainLocalDataDisplaySecond, &g_sMainLocalDataDisplaySecondCVoltageAnagle, 0,
       &g_sFormike128x128x16, 1, 59, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_local_data_b_voltage_angle, " B相电压相角:", 
	   (const unsigned char*)1, OnPaintMainLocalDataDisplay);
// C相电压相角
Canvas(g_sMainLocalDataDisplaySecondCVoltageAnagle, &g_sMainLocalDataDisplaySecond, 0, &g_sMainLocalDataDisplaySecondNum,
       &g_sFormike128x128x16, 1, 76, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_local_data_c_voltage_angle, " C相电压相角:", 
	   (const unsigned char*)1, OnPaintMainLocalDataDisplay);
// 显示当前所在屏为第一屏
Canvas(g_sMainLocalDataDisplaySecondNum, &g_sMainLocalDataDisplaySecondCVoltageAnagle, 0,0 ,
       &g_sFormike128x128x16, 135, 25, 24, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN ),
       ClrWhite, 0, ClrBlack, 0, "2/2", 
	   0, OnPaintMainCharacter);

//-----------------------------------------------------------------------------
// 第三屏 测量点数据显示
//-----------------------------------------------------------------------------
// 第三屏 测量点数据显示
Canvas(g_sMainMeasureDataDisplay, &g_sMain, 0, &g_sMainMeasureDataDisplayHead,
       &g_sFormike128x128x16, 1, 25, 157, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// 菜单栏
Canvas(g_sMainMeasureDataDisplayHead, &g_sMainMeasureDataDisplay, &g_sMainMeasureDataDisplayPoint1, 0,
       &g_sFormike128x128x16, 1+8, 25, 7*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "测量点数据显示", 0, OnPaintMainCharacter);
// 测量点1
Canvas(g_sMainMeasureDataDisplayPoint1, &g_sMainMeasureDataDisplay, &g_sMainMeasureDataDisplayPoint2, 0,
       &g_sFormike128x128x16, 1+8*6, 42, 90, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN | CANVAS_STYLE_REVERSE_COLOR),
       ClrWhite, 0, ClrBlack, 0, "测量点1", 
	   0, OnPaintMainCharacter);
// 测量点2
Canvas(g_sMainMeasureDataDisplayPoint2, &g_sMainMeasureDataDisplay, &g_sMainMeasureDataDisplayPoint3, 0,
       &g_sFormike128x128x16, 1+8*6, 59, 90, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "测量点2", 
	   0, OnPaintMainCharacter);
// 测量点3
Canvas(g_sMainMeasureDataDisplayPoint3, &g_sMainMeasureDataDisplay, &g_sMainMeasureDataDisplayPoint4, 0,
       &g_sFormike128x128x16, 1+8*6, 76, 90, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "测量点3", 
	   0, OnPaintMainCharacter);
// 测量点4
Canvas(g_sMainMeasureDataDisplayPoint4, &g_sMainMeasureDataDisplay, &g_sMainMeasureDataDisplayPoint5, 0,
       &g_sFormike128x128x16, 1+8*6, 93, 90, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "测量点4", 
	   0, OnPaintMainCharacter);
// 测量点5
Canvas(g_sMainMeasureDataDisplayPoint5, &g_sMainMeasureDataDisplay, &g_sMainMeasureDataDisplayPoint6, 0,
       &g_sFormike128x128x16, 1+8*6, 109, 90, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "测量点5", 
	   0, OnPaintMainCharacter);
// 测量点6
Canvas(g_sMainMeasureDataDisplayPoint6, &g_sMainMeasureDataDisplay, 0, &g_sMainMeasureDataDisplayNum,
       &g_sFormike128x128x16, 1+8*6, 126, 90, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "测量点6", 
	   0, OnPaintMainCharacter);
// 显示当前所在屏为第一屏
Canvas(g_sMainMeasureDataDisplayNum, &g_sMainMeasureDataDisplayPoint6, 0,0 ,
       &g_sFormike128x128x16, 135, 25, 24, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN ),
       ClrWhite, 0, ClrBlack, 0, "1/1", 
	   0, OnPaintMainCharacter);

//-----------------------------------------------------------------------------
// 第四屏 测量点数据详细显示
//-----------------------------------------------------------------------------
// 第四屏 测量点数据详细显示
Canvas(g_sMainMeasureDataDetial, &g_sMain, 0, &g_sMainMeasureDataDetialHead,
       &g_sFormike128x128x16, 1, 25, 157, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);

// 测量点n
Canvas(g_sMainMeasureDataDetialHead, &g_sMainMeasureDataDetial, 0, &g_sMainMeasureDataDetialFirst,
       &g_sFormike128x128x16, 1+6*8, 25, 7*8, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, (const char*)g_pu8_measure_data_point_num, 0, OnPaintMainMeasureDataDetialPoint);

// 选择循环显示背景
Canvas(g_sMainMeasureDataDetialCircleGround, &g_sMainMeasureDataDetial, 0,&g_sMainMeasureDataDetialCircleConfirm,
       &g_sFormike128x128x16, 83, 80, 4*16+3*2, 16*2+3*3,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE),
       ClrWhite, 0, ClrBlack, 0, 0, 0, 0);
// 确认循环显示
Canvas(g_sMainMeasureDataDetialCircleConfirm, &g_sMainMeasureDataDetialCircleGround, &g_sMainMeasureDataDetialCircleCancel,0,
       &g_sFormike128x128x16, 83+3, 80+3, 4*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN | CANVAS_STYLE_REVERSE_COLOR),
       ClrWhite, 0, ClrBlack, 0, "确认循显", 0, OnPaintMainCharacter);
// 取消循环显示
Canvas(g_sMainMeasureDataDetialCircleCancel, &g_sMainMeasureDataDetialCircleGround, 0,0,
       &g_sFormike128x128x16, 83+3, 80+3+16+3, 4*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "取消循显", 0, OnPaintMainCharacter);

// 显示数据的第一屏
Canvas(g_sMainMeasureDataDetialFirst, &g_sMainMeasureDataDetialHead, 0, &g_sMainMeasureDataDetialTimeInfo,
		&g_sFormike128x128x16, 1, 42, 157, 117,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);

// 抄表时间提示中文
Canvas(g_sMainMeasureDataDetialTimeInfo, &g_sMainMeasureDataDetialFirst, &g_sMainMeasureDataDetialTime, 0,
       &g_sFormike128x128x16, 1, 42, 8*9, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "抄表时间:", 
	   0, OnPaintMainCharacter);

// 抄表时间
Canvas(g_sMainMeasureDataDetialTime, &g_sMainMeasureDataDetialFirst, &g_sMainMeasureDataDetialZYJ, 0,
       &g_sFormike128x128x16, 73, 42, 8*5, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN | CANVAS_STYLE_REVERSE_COLOR),
       ClrWhite, 0, ClrBlack, 0, 0, 
	   0, OnPaintMainMeasureDataDetialTime);

// 正向有功尖电量
Canvas(g_sMainMeasureDataDetialZYJ, &g_sMainMeasureDataDetialFirst, &g_sMainMeasureDataDetialZYF, 0,
       &g_sFormike128x128x16, 1, 59, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_measure_data_point_zyj[0], "正有功尖:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// 正向有功峰电量
Canvas(g_sMainMeasureDataDetialZYF, &g_sMainMeasureDataDetialFirst, &g_sMainMeasureDataDetialZYP, 0,
       &g_sFormike128x128x16, 1, 76, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_measure_data_point_zyf[0], "正有功峰:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// 正向有功平电量
Canvas(g_sMainMeasureDataDetialZYP, &g_sMainMeasureDataDetialFirst, &g_sMainMeasureDataDetialZYG, 0,
       &g_sFormike128x128x16, 1, 93, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_measure_data_point_zyp[0], "正有功平:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// 正向有功谷电量
Canvas(g_sMainMeasureDataDetialZYG, &g_sMainMeasureDataDetialFirst, &g_sMainMeasureDataDetialFYJ, 0,
       &g_sFormike128x128x16, 1, 109, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_measure_data_point_zyg[0], "正有功谷:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// 反向有功尖电量
Canvas(g_sMainMeasureDataDetialFYJ, &g_sMainMeasureDataDetialFirst, 0, &g_sMainMeasureDataDetialFirstNum,
       &g_sFormike128x128x16, 1, 126, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_measure_data_point_fyj[0], "反有功尖:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// 显示当前所在屏为第一屏
Canvas(g_sMainMeasureDataDetialFirstNum, &g_sMainMeasureDataDetialFYJ, 0,0 ,
       &g_sFormike128x128x16, 135, 25, 24, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "1/2", 
	   0, OnPaintMainCharacter);

// 显示数据的第二屏
Canvas(g_sMainMeasureDataDetialSecond, &g_sMainMeasureDataDetialHead, 0, &g_sMainMeasureDataDetialFYF,
       &g_sFormike128x128x16, 1, 42, 157, 117,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// 反向有功峰电量
Canvas(g_sMainMeasureDataDetialFYF, &g_sMainMeasureDataDetialSecond, &g_sMainMeasureDataDetialFYP, 0,
       &g_sFormike128x128x16, 1, 42, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_measure_data_point_fyf[0], "反有功峰:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// 反向有功平电量
Canvas(g_sMainMeasureDataDetialFYP, &g_sMainMeasureDataDetialSecond, &g_sMainMeasureDataDetialFYG, 0,
       &g_sFormike128x128x16, 1, 59, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_measure_data_point_fyp[0], "反有功平:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// 反向有功谷电量
Canvas(g_sMainMeasureDataDetialFYG, &g_sMainMeasureDataDetialSecond, 0, &g_sMainMeasureDataDetialSecondNum,
       &g_sFormike128x128x16, 1, 76, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_measure_data_point_fyg[0], "反有功谷:", 
	   (const unsigned char*)2, OnPaintMainLocalDataDisplay);
// 显示当前所在屏为第一屏
Canvas(g_sMainMeasureDataDetialSecondNum, &g_sMainMeasureDataDetialFYG, 0,0 ,
       &g_sFormike128x128x16, 135, 25, 24, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "2/2", 
	   0, OnPaintMainCharacter);


//-----------------------------------------------------------------------------
// 第五屏 参数设置与查看
//-----------------------------------------------------------------------------
// 第五屏 参数设置与查看
Canvas(g_sMainParaSet, &g_sMain, 0, &g_sMainParaSetHead,
       &g_sFormike128x128x16, 1, 25, 158, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// 参数设置与查看
Canvas(g_sMainParaSetHead, &g_sMainParaSet, &g_sMainParaSetCmtChannelSet,0,
       &g_sFormike128x128x16, 1+8*3, 25, 6*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "相关参数查看", 0, OnPaintMainCharacter);
// 通讯通道设置
Canvas(g_sMainParaSetCmtChannelSet, &g_sMainParaSet, &g_sMainParaSetAreaMeterSet,0,
       &g_sFormike128x128x16, 1+16*2, 42, 6*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN | CANVAS_STYLE_REVERSE_COLOR),
       ClrBlack, 0, ClrWhite, 0, "通讯通道查看", 0, OnPaintMainCharacter);
// 台区电表参数设置
Canvas(g_sMainParaSetAreaMeterSet, &g_sMainParaSet, &g_sMainParaSetCentralMeterSet,0,
       &g_sFormike128x128x16, 1+16*1, 59, 8*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "台区电表参数查看", 0, OnPaintMainCharacter);
// 集抄电表参数设置
Canvas(g_sMainParaSetCentralMeterSet, &g_sMainParaSet, &g_sMainParaSetTimeSet, 0,
       &g_sFormike128x128x16, 1+16*1, 76, 8*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "集抄电表参数查看", 0, OnPaintMainCharacter);
// 终端时间设置
Canvas(g_sMainParaSetTimeSet, &g_sMainParaSet, &g_sMainParaSetPasswdSet, 0,
       &g_sFormike128x128x16, 1+16*2, 93, 6*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "终端时间查看", 0, OnPaintMainCharacter);
// 界面密码设置
Canvas(g_sMainParaSetPasswdSet, &g_sMainParaSet, &g_sMainParaSetNumberSet, 0,
       &g_sFormike128x128x16, 1+16*2, 109, 6*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "界面密码查看", 0, OnPaintMainCharacter);
// 终端编号
Canvas(g_sMainParaSetNumberSet, &g_sMainParaSet, 0, &g_sMainParaSetNum,
       &g_sFormike128x128x16, 1+16*3, 126, 4*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "终端编号", 0, OnPaintMainCharacter);
// 显示当前所在屏为第一屏
Canvas(g_sMainParaSetNum, &g_sMainParaSetNumberSet, 0,0 ,
       &g_sFormike128x128x16, 135, 25, 24, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "1/1", 
	   0, OnPaintMainCharacter);

//-----------------------------------------------------------------------------
// 第六屏 通讯通道查看
//-----------------------------------------------------------------------------
// 第六屏 通讯通道查看
Canvas(g_sMainCmtChannalSet, &g_sMain, 0, &g_sMainCmtChannalSetHead,
       &g_sFormike128x128x16, 1, 25, 157, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// 通讯通道设置
Canvas(g_sMainCmtChannalSetHead, &g_sMainCmtChannalSet, 0, &g_sMainCmtChannalSetFirst,
       &g_sFormike128x128x16, 1+3*8, 25, 8*8, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0,"通讯通道查看", 0, OnPaintMainCharacter);
//// 选择循环显示背景
//Canvas(g_sMainCmtChannalSetCircleGround, &g_sMainCmtChannalSet, 0,&g_sMainCmtChannalSetCircleConfirm,
//       &g_sFormike128x128x16, 83, 80, 4*16+3*2, 16*2+3*3,
//       (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE),
//       ClrWhite, 0, ClrBlack, 0, 0, 0, 0);
//// 确认循环显示
//Canvas(g_sMainCmtChannalSetCircleConfirm, &g_sMainCmtChannalSetCircleGround, &g_sMainCmtChannalSetCircleCancel,0,
//       &g_sFormike128x128x16, 83+3, 80+3, 4*16, 16,
//       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN | CANVAS_STYLE_REVERSE_COLOR),
//       ClrWhite, 0, ClrBlack, 0, "确认循显", 0, OnPaintMainCharacter);
//// 取消循环显示
//Canvas(g_sMainCmtChannalSetCircleCancel, &g_sMainCmtChannalSetCircleGround, 0,0,
//       &g_sFormike128x128x16, 83+3, 80+3+16+3, 4*16, 16,
//       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
//       ClrWhite, 0, ClrBlack, 0, "取消循显", 0, OnPaintMainCharacter);
// 显示数据的第一屏
Canvas(g_sMainCmtChannalSetFirst, &g_sMainCmtChannalSetHead, 0, &g_sMainCmtChannalSetHostIP,
		&g_sFormike128x128x16, 1, 42, 157, 117,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// 主机IP地址
Canvas(g_sMainCmtChannalSetHostIP, &g_sMainCmtChannalSetFirst, &g_sMainCmtChannalSetLocalIP, 0,
       &g_sFormike128x128x16, 1, 42, 8*9, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN | CANVAS_STYLE_REVERSE_COLOR),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_u32_cmt_channal_set_host_ip, "主机IP:", 
	   0, OnPaintCmtChannalSetIPDisplay);
// 本机IP
Canvas(g_sMainCmtChannalSetLocalIP, &g_sMainCmtChannalSetFirst, &g_sMainCmtChannalSetTermIP, 0,
       &g_sFormike128x128x16, 1, 59, 8*5, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_u32_cmt_channal_set_local_ip, "本机IP:", 
	   0, OnPaintCmtChannalSetIPDisplay);
// 终端IP
Canvas(g_sMainCmtChannalSetTermIP, &g_sMainCmtChannalSetFirst, &g_sMainCmtChannalSetPortNum, 0,
       &g_sFormike128x128x16, 1, 76, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_u32_cmt_channal_set_term_ip, "终端IP:", 
	   0, OnPaintCmtChannalSetIPDisplay);
// 端口号
Canvas(g_sMainCmtChannalSetPortNum, &g_sMainCmtChannalSetFirst, &g_sMainCmtChannalSetAPN, 0,
       &g_sFormike128x128x16, 1, 93, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_cmt_channal_set_port_num, "端口号:", 
	   (const unsigned char*)0, OnPaintMainLocalDataDisplay);
// APN
Canvas(g_sMainCmtChannalSetAPN, &g_sMainCmtChannalSetFirst, &g_sMainCmtChannalSetUserName, 0,
       &g_sFormike128x128x16, 1, 109, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)g_pu8_cmt_channal_set_apn, "APN:", 
	   0, OnPaintStringDisplay);
// 用户名
Canvas(g_sMainCmtChannalSetUserName, &g_sMainCmtChannalSetFirst, 0, &g_sMainCmtChannalSetFirstNum,
       &g_sFormike128x128x16, 1, 126, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)g_pu8_cmt_channal_set_username, "用户名:", 
	   0, OnPaintStringDisplay);
// 显示当前所在屏为第一屏
Canvas(g_sMainCmtChannalSetFirstNum, &g_sMainCmtChannalSetUserName, 0,0 ,
       &g_sFormike128x128x16, 135, 25, 24, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "1/2", 
	   0, OnPaintMainCharacter);


// 显示数据的第二屏
Canvas(g_sMainCmtChannalSetSecond, &g_sMainCmtChannalSetHead, 0, &g_sMainCmtChannalSetPasswd,
       &g_sFormike128x128x16, 1, 42, 157, 117,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// 密码
Canvas(g_sMainCmtChannalSetPasswd, &g_sMainCmtChannalSetSecond, &g_sMainCmtChannalSetPeriod, 0,
       &g_sFormike128x128x16, 1, 42, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)g_pu8_cmt_channal_set_passwd, "密码:", 
	   0, OnPaintStringDisplay);
// 心跳周期
Canvas(g_sMainCmtChannalSetPeriod, &g_sMainCmtChannalSetSecond, 0, &g_sMainCmtChannalSetSecondNum,
       &g_sFormike128x128x16, 1, 59, 157, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_cmt_channal_set_period, "心跳周期:", 
	   (const unsigned char*)1, OnPaintMainLocalDataDisplay);
// 显示当前所在屏为第二屏
Canvas(g_sMainCmtChannalSetSecondNum, &g_sMainCmtChannalSetPeriod, 0,0 ,
       &g_sFormike128x128x16, 135, 25, 24, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "2/2", 
	   0, OnPaintMainCharacter);

//-----------------------------------------------------------------------------
// 第七屏 台区电表参数查看
//-----------------------------------------------------------------------------
// 第七屏 台区电表参数查看
Canvas(g_sMainAreaMeterSet, &g_sMain, 0, &g_sMainAreaMeterSetHead,
       &g_sFormike128x128x16, 1, 25, 157, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// 端口抄表间隔
Canvas(g_sMainAreaMeterSetHead, &g_sMainAreaMeterSet, 0, &g_sMainAreaMeterSetFirst,
       &g_sFormike128x128x16, 1+3*8, 25, 8*8, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0,"端口抄表间隔", 0, OnPaintMainCharacter);
// 显示数据的第一屏
Canvas(g_sMainAreaMeterSetFirst, &g_sMainAreaMeterSetHead, 0, &g_sMainAreaMeterSetPort1Interval,
		&g_sFormike128x128x16, 1, 42, 157, 117,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// 端口1间隔:
Canvas(g_sMainAreaMeterSetPort1Interval, &g_sMainAreaMeterSetFirst, &g_sMainAreaMeterSetPort2Interval, 0,
       &g_sFormike128x128x16, 1, 42, 8*9, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN | CANVAS_STYLE_REVERSE_COLOR),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_area_meter_set_port1_interval, "端口1间隔:", 
	   (const unsigned char*)1, OnPaintMainLocalDataDisplay);
// 端口2间隔:
Canvas(g_sMainAreaMeterSetPort2Interval, &g_sMainAreaMeterSetFirst, &g_sMainAreaMeterSetPort3Interval, 0,
       &g_sFormike128x128x16, 1, 59, 8*10, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_area_meter_set_port1_interval, "端口2间隔:", 
	   (const unsigned char*)1, OnPaintMainLocalDataDisplay);
// 端口3间隔:
Canvas(g_sMainAreaMeterSetPort3Interval, &g_sMainAreaMeterSetFirst, 0, &g_sMainAreaMeterSetFirstNum,
       &g_sFormike128x128x16, 1, 76,8*10 , 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)&g_f64_area_meter_set_port1_interval, "端口3间隔:", 
	   (const unsigned char*)1, OnPaintMainLocalDataDisplay);
// 显示当前所在屏为第一屏
Canvas(g_sMainAreaMeterSetFirstNum, &g_sMainAreaMeterSetPort3Interval, 0,0 ,
       &g_sFormike128x128x16, 127, 25, 24, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, " 1/1", 
	   0, OnPaintMainCharacter);

//-----------------------------------------------------------------------------
// 第八屏 终端管理与维护
//-----------------------------------------------------------------------------
// 第八屏 终端管理与维护
Canvas(g_sMainTermSet, &g_sMain, 0, &g_sMainTermSetHead,
       &g_sFormike128x128x16, 1, 25, 158, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// 终端管理与维护
Canvas(g_sMainTermSetHead, &g_sMainTermSet, 0,&g_sMainTermSetAMRInfo,
       &g_sFormike128x128x16, 1, 25, 7*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "终端管理与维护", 0, OnPaintMainCharacter);
// 集中器信息
Canvas(g_sMainTermSetAMRInfo, &g_sMainTermSetHead, &g_sMainTermSetLast5Message,0,
       &g_sFormike128x128x16, 1+8*5, 42, 5*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN | CANVAS_STYLE_REVERSE_COLOR),
       ClrBlack, 0, ClrWhite, 0, "集中器信息", 0, OnPaintMainCharacter);
// 最近5条中文消息
Canvas(g_sMainTermSetLast5Message, &g_sMainTermSetHead, &g_sMainTermSetSystemReset,0,
       &g_sFormike128x128x16, 1+20, 59, 8*15, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "最近5条中文消息", 0, OnPaintMainCharacter);
// 系统复位
Canvas(g_sMainTermSetSystemReset, &g_sMainTermSetHead, 0,&g_sMainTermSetNum,
       &g_sFormike128x128x16, 1+16*3, 76, 4*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "系统复位", 0, OnPaintMainCharacter);
// 显示当前所在屏为第一屏
Canvas(g_sMainTermSetNum, &g_sMainTermSetSystemReset, 0,0 ,
       &g_sFormike128x128x16, 135, 25, 24, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "1/1", 
	   0, OnPaintMainCharacter);

//-----------------------------------------------------------------------------
// 第九屏 集中器版本
//-----------------------------------------------------------------------------
// 第九屏 集中器版本
Canvas(g_sMainAMRInfo, &g_sMain, 0, &g_sMainAMRInfoHead,
       &g_sFormike128x128x16, 1, 25, 158, 134,
       (CANVAS_STYLE_FILL),
       ClrWhite, ClrBlack, ClrBlack, 0, 0, 0, 0);
// 终端管理与维护
Canvas(g_sMainAMRInfoHead, &g_sMainAMRInfo, 0,&g_sMainAMRInfoVersion,
       &g_sFormike128x128x16, 1+8*5, 25, 5*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "集中器信息", 0, OnPaintMainCharacter);
// 集中器信息
Canvas(g_sMainAMRInfoVersion, &g_sMainAMRInfoHead, &g_sMainAMRInfoSoftVersion,0,
       &g_sFormike128x128x16, 1+44, 42, 5*16, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, 0, "软件版本:", 0, OnPaintMainCharacter);
// 最近5条中文消息
Canvas(g_sMainAMRInfoSoftVersion, &g_sMainAMRInfoHead, 0,0,
       &g_sFormike128x128x16, 1+20, 59, 8*15, 16,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_APP_DRAWN),
       ClrWhite, 0, ClrBlack, (const tFont*)g_pu8_amr_info_soft_version, 
	   " ", 0, OnPaintStringDisplay);


//-----------------------------------------------------------------------------
//
// 函数名称  :  OnPaintCmtChannalSetIPDisplay 
// 函数功能  :  这个用于显示字符串
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void OnPaintStringDisplay(tWidget *pWidget, tContext *pContext)
{
   unsigned long u32_info_len = 0;
   //unsigned char pu8_ip_data[20];
   unsigned char *pu8_string_data;
   //unsigned char u8_value_data_index = 0;
   //unsigned long u32_i;
   //unsigned long u32_ip;
   short s16_xmin = (*pWidget).sPosition.sXMin;
   short s16_ymin = (*pWidget).sPosition.sYMin;
   //short s16_xmax = (*pWidget).sPosition.sXMax;
   //short s16_ymax = (*pWidget).sPosition.sYMax;   

   tCanvasWidget *pst_canvas = (tCanvasWidget *)pWidget;
   unsigned long ulForeground = (*pst_canvas).ulTextColor;
   unsigned long ulBackground = (*pst_canvas).ulFillColor;
   // 判断是否要反白显示
   if((pst_canvas->ulStyle) & CANVAS_STYLE_REVERSE_COLOR)
   {
       // 反白
	   ulForeground = ClrWhite;
	   ulBackground = ClrBlack; 
   }
   else
   {
       // 正常显示
	   ulForeground = ClrBlack;
	   ulBackground = ClrWhite; 
   }
   // 显示提示的中文信息
   u32_info_len = strlen(pst_canvas->pcText);    
   GBStringPrint(s16_xmin,s16_ymin,(unsigned char *)pst_canvas->pcText,ClrBlack,ClrWhite);

   // 借这个变量使用一下
   pu8_string_data = ((unsigned char *)pst_canvas->pFont); 

   // 输出数据字符串
   GBStringPrint(s16_xmin + 8*u32_info_len,s16_ymin,pu8_string_data,ulForeground,ulBackground);  
}


//-----------------------------------------------------------------------------
//
// 函数名称  :  OnPaintCmtChannalSetIPDisplay 
// 函数功能  :  这个用于显示IP地址
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void OnPaintCmtChannalSetIPDisplay(tWidget *pWidget, tContext *pContext)
{
   unsigned long u32_info_len = 0;
   unsigned char pu8_ip_data[20];
   //unsigned char u8_value_data_index = 0;
   //unsigned long u32_i;
   unsigned long u32_ip;
   short s16_xmin = (*pWidget).sPosition.sXMin;
   short s16_ymin = (*pWidget).sPosition.sYMin;
   //short s16_xmax = (*pWidget).sPosition.sXMax;
   //short s16_ymax = (*pWidget).sPosition.sYMax;   

   tCanvasWidget *pst_canvas = (tCanvasWidget *)pWidget;
   unsigned long ulForeground = (*pst_canvas).ulTextColor;
   unsigned long ulBackground = (*pst_canvas).ulFillColor;
   // 判断是否要反白显示
   if((pst_canvas->ulStyle) & CANVAS_STYLE_REVERSE_COLOR)
   {
       // 反白
	   ulForeground = ClrWhite;
	   ulBackground = ClrBlack; 
   }
   else
   {
       // 正常显示
	   ulForeground = ClrBlack;
	   ulBackground = ClrWhite; 
   }
   // 显示提示的中文信息
   u32_info_len = strlen(pst_canvas->pcText);    
   GBStringPrint(s16_xmin,s16_ymin,(unsigned char *)pst_canvas->pcText,ClrBlack,ClrWhite);

   // 借这个变量使用一下
   u32_ip = *((unsigned long *)pst_canvas->pFont); 
   pu8_ip_data[0] = '0' + (((u32_ip >> 24)&0xff)%1000)/100;
   pu8_ip_data[1] = '0' + (((u32_ip >> 24)&0xff)%100)/10;;
   pu8_ip_data[2] = '0' + (((u32_ip >> 24)&0xff)%10)/1;;

   pu8_ip_data[3] = '0' + (((u32_ip >> 16)&0xff)%1000)/100;
   pu8_ip_data[4] = '0' + (((u32_ip >> 16)&0xff)%100)/10;;
   pu8_ip_data[5] = '0' + (((u32_ip >> 16)&0xff)%10)/1;;

   pu8_ip_data[6] = '0' + (((u32_ip >> 8)&0xff)%1000)/100;
   pu8_ip_data[7] = '0' + (((u32_ip >> 8)&0xff)%100)/10;;
   pu8_ip_data[8] = '0' + (((u32_ip >> 8)&0xff)%10)/1;;

   pu8_ip_data[9] = '0' + (((u32_ip >> 0)&0xff)%1000)/100;
   pu8_ip_data[10] = '0' + (((u32_ip >> 0)&0xff)%100)/10;;
   pu8_ip_data[11] = '0' + (((u32_ip >> 0)&0xff)%10)/1;;

   // 最后一位为0表示结束
   pu8_ip_data[12] = 0x00;

   // 输出数据字符串
   GBStringPrint(s16_xmin + 8*u32_info_len,s16_ymin,pu8_ip_data,ulForeground,ulBackground);  
}



//-----------------------------------------------------------------------------
//
// 函数名称  :  OnPaintMainMeasureDataDetialPoint 
// 函数功能  :  这个用于显示测量点
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void OnPaintMainMeasureDataDetialPoint(tWidget *pWidget, tContext *pContext)
{	
   g_pu8_measure_data_point_num[6] = '0' + g_u8_main_measure_data_index + 1;
   OnPaintMainCharacter(pWidget, pContext);	 
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  OnPaintMainMeasureDataDetialTime 
// 函数功能  :  这个用于显示抄表时间
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void OnPaintMainMeasureDataDetialTime(tWidget *pWidget, tContext *pContext)
{	
	 //tCanvasWidget *pst_canvas = (tCanvasWidget *)pWidget;
     g_pu8_measure_data_point_time[g_u8_main_measure_data_index][0] = 
	 				'0' + ((g_pu8_measure_data_point_time_hour[g_u8_main_measure_data_index]%100)/10);  
     g_pu8_measure_data_point_time[g_u8_main_measure_data_index][1] = 
	 				'0' + ((g_pu8_measure_data_point_time_hour[g_u8_main_measure_data_index]%10)/1);  
     g_pu8_measure_data_point_time[g_u8_main_measure_data_index][3] = 
	 				'0' + ((g_pu8_measure_data_point_time_minute[g_u8_main_measure_data_index]%100)/10);  
     g_pu8_measure_data_point_time[g_u8_main_measure_data_index][4] = 
	 				'0' + ((g_pu8_measure_data_point_time_minute[g_u8_main_measure_data_index]%10)/1);  
	 OnPaintMainCharacter(pWidget,pContext);
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  OnPaintMainFirstFaceDate 
// 函数功能  :  这个用于显示日期
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void OnPaintMainFirstFaceDate(tWidget *pWidget, tContext *pContext)
{	 
	 if((g_u32_year < 10000) && (g_u32_month <= 12) && (g_u32_date <= 31))
	 {
	       g_pu8_date[0] = '0' + ((g_u32_year%10000)/1000);  
		   g_pu8_date[1] = '0' + ((g_u32_year%1000)/100);  
		   g_pu8_date[2] = '0' + ((g_u32_year%100)/10);  
		   g_pu8_date[3] = '0' + ((g_u32_year%10)/1);  

		   g_pu8_date[5] = '0' + ((g_u32_month%100)/10);  
		   g_pu8_date[6] = '0' + ((g_u32_month%10)/1);  

		   g_pu8_date[8] = '0' + ((g_u32_date%100)/10);  
		   g_pu8_date[9] = '0' + ((g_u32_date%10)/1);  

		   OnPaintMainCharacter(pWidget,pContext);
	 }
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  OnPaintMainFirstFaceTime 
// 函数功能  :  这个用于显示时间
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void OnPaintMainFirstFaceTime(tWidget *pWidget, tContext *pContext)
{	
	 if((g_u32_hour <= 24) && (g_u32_minute <= 59) && (g_u32_second <= 59))
	 {
		   g_pu8_time[0] = '0' + ((g_u32_hour%100)/10);  
		   g_pu8_time[1] = '0' + ((g_u32_hour%10)/1);  

		   g_pu8_time[3] = '0' + ((g_u32_minute%100)/10);  
		   g_pu8_time[4] = '0' + ((g_u32_minute%10)/1);  

		   g_pu8_time[6] = '0' + ((g_u32_second%100)/10);  
		   g_pu8_time[7] = '0' + ((g_u32_second%10)/1);  

		   OnPaintMainCharacter(pWidget,pContext);
	 }
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  OnPaintMainLocalDataDisplay 
// 函数功能  :  这个用于显示中英文信息+浮点数据的函数
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void OnPaintMainLocalDataDisplay(tWidget *pWidget, tContext *pContext)
{
   double f32_value;
   unsigned long u32_value;
   unsigned long u32_decimal_num = 0;
   unsigned long u32_info_len = 0;
   unsigned char pu8_value_data[20];
   unsigned char u8_value_data_index = 0;
   unsigned long u32_i;
   unsigned long u32_j = 1;
   unsigned long u32_k;
   //unsigned long s32_tmp;
   unsigned long u32_tmp;
   unsigned char u8_negative_flag = 0;
   short s16_xmin = (*pWidget).sPosition.sXMin;
   short s16_ymin = (*pWidget).sPosition.sYMin;
   //short s16_xmax = (*pWidget).sPosition.sXMax;
   //short s16_ymax = (*pWidget).sPosition.sYMax;   
   tCanvasWidget *pst_canvas = (tCanvasWidget *)pWidget;
   unsigned long ulForeground = (*pst_canvas).ulTextColor;
   unsigned long ulBackground = (*pst_canvas).ulFillColor;
   // 判断是否要反白显示
   if((pst_canvas->ulStyle) & CANVAS_STYLE_REVERSE_COLOR)
   {
       // 反白
	   ulForeground = ClrWhite;
	   ulBackground = ClrBlack; 
   }
   else
   {
       // 正常显示
	   ulForeground = ClrBlack;
	   ulBackground = ClrWhite; 
   }
   // 显示提示的中文信息
   u32_info_len = strlen(pst_canvas->pcText);    
    //printf("aa %s,\n",pst_canvas->pcText);
   GBStringPrint(s16_xmin,s16_ymin,(unsigned char *)pst_canvas->pcText,ClrBlack,ClrWhite);

   // 借这个变量使用一下
   u32_decimal_num = (unsigned long)pst_canvas->pucImage;   

   // 借这个变量使用一下
   f32_value = *((double *)pst_canvas->pFont); 
   
   // 根据需要添加负号
   if(f32_value < 0)
   {
     f32_value = 0 - f32_value;
	 pu8_value_data[u8_value_data_index] = '-'; 
	 u8_value_data_index++;
	 u8_negative_flag = 1;
   }
   
   // 判断是否有小于1，如果小于1，则需要显示0.多少
   // 这里是大于1的情况下
   if(f32_value > 1)
   {
       u32_value = (unsigned long)f32_value;

	   u32_value = u32_value%1000000;
	   // 逐个获取每一位
	   for(u32_i = 100000;u32_i >= 1; u32_i = u32_i/10)
	   {
	      	 // 获取这个位的值
			 u32_tmp = u32_value/u32_i;	 
			 // 删除这个位之后的剩余数据
			 u32_value = u32_value%u32_i;
			 if(u32_tmp != 0)
			 {
			    pu8_value_data[u8_value_data_index] = '0' + u32_tmp; 
				u8_value_data_index++;
			 }
			 else
			 {
			     // 开始的0的不显示
				 if(u8_value_data_index != u8_negative_flag)
				 {
				    pu8_value_data[u8_value_data_index] = '0' + u32_tmp; 
				    u8_value_data_index++;
				 }
			 }
	   }
   }
   // 小于1的情况
   else
   {
      // 在小数点前增加字符0
	  pu8_value_data[u8_value_data_index] = '0'; 
      u8_value_data_index++;
   }
   
   if(u32_decimal_num)
   {
	   // 小数点
	   pu8_value_data[u8_value_data_index] = '.'; 
	   u8_value_data_index++;
   }
   

   // 首先乘上几个10
   u32_j = 1;
   for(u32_i = 0; u32_i < u32_decimal_num; u32_i++)
   {
      // 获取小数点部分的最左边的一位
	  f32_value *= 10;
	  u32_j  *= 10;  
   }
   
   // 显示小数点部分数据
   u32_tmp  = (unsigned long)f32_value;
   u32_tmp = u32_tmp%u32_j;
   u32_j = u32_j/10;

   // 根据小数点部分位数显示 
   for(u32_i = 0; u32_i < u32_decimal_num; u32_i++)
   {
      // 获取小数点部分的最左边的一位
	  if(u32_j != 0)
	  {
	     u32_k = u32_tmp/u32_j; 
	  }
	  // 设定小数部分的数字
	  pu8_value_data[u8_value_data_index] = '0' + u32_k; 
	  u8_value_data_index++;
	  u32_tmp = u32_tmp%u32_j;
	  u32_j = u32_j/10; 
   }
   // 最后一位为0表示结束
   pu8_value_data[u8_value_data_index] = 0x00;

   // 输出数据字符串
   GBStringPrint(s16_xmin + 8*u32_info_len,s16_ymin,pu8_value_data,ulForeground,ulBackground);
}


//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_canvas_set_reverse_color 
// 函数功能  :  设定显示反白
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void fn_canvas_set_reverse_color(tCanvasWidget *pst_canvas)
{
   pst_canvas->ulStyle |= (CANVAS_STYLE_REVERSE_COLOR); 
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_canvas_clear_reverse_color 
// 函数功能  :  清除显示反白
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void fn_canvas_clear_reverse_color(tCanvasWidget *pst_canvas)
{
   pst_canvas->ulStyle &= ~(CANVAS_STYLE_REVERSE_COLOR); 
}

//-----------------------------------------------------------------------------


//
// 函数名称  :  fn_set_cavas_fore_back_ground 
// 函数功能  :  设定canvas前景色和背景色
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void fn_set_cavas_fore_back_ground(tCanvasWidget *pst_canvas,unsigned long ulForeground,unsigned long ulBackground)
{
   CanvasFillColorSet(pst_canvas,ulBackground);
   CanvasTextColorSet(pst_canvas,ulForeground);
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_canvas_next_index 
// 函数功能  :  设定canvas增加下标
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
int fn_canvas_next_index(tCanvasWidget *pst_canvas,unsigned char *pu8_index)
{
    unsigned char u8_i;
    tCanvasWidget *pst_canvas_tmp = pst_canvas;
	for(u8_i = 0; u8_i < *pu8_index; u8_i++)
	{
	   // 获取下一个canvas
	   pst_canvas_tmp = (tCanvasWidget *)((*pst_canvas_tmp).sBase.pNext); 
	   if(pst_canvas_tmp == 0)
	   {
	     return 0 ; 
	   }
	}

	// 如果已经到了最后一个canvas
    if((*pst_canvas_tmp).sBase.pNext == 0)
	{
	   *pu8_index = 0;	   
	   // 清除当前反白
	   fn_canvas_clear_reverse_color(pst_canvas_tmp);	   
	   pst_canvas_tmp = pst_canvas;
	   // 设置下一个反白
	   fn_canvas_set_reverse_color(pst_canvas_tmp);
	}
	// 没有到最后一个canvas 
	else
	{
	   *pu8_index = *pu8_index + 1;
	   // 清除当前反白
	   fn_canvas_clear_reverse_color(pst_canvas_tmp);	   
	   pst_canvas_tmp = (tCanvasWidget *)((*pst_canvas_tmp).sBase.pNext); 
	   // 设置下一个反白
	   fn_canvas_set_reverse_color(pst_canvas_tmp);
	}
	return 1;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_canvas_prev_index 
// 函数功能  :  设定canvas减少下标
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
int fn_canvas_prev_index(tCanvasWidget *pst_canvas,unsigned char *pu8_index)
{
    unsigned char u8_i;
    tCanvasWidget *pst_canvas_tmp = pst_canvas;
    // 如果当前在第一项的时候
	if(*pu8_index == 0)
	{
	    // 寻找最后一项
		for(u8_i = 0;u8_i < 50; u8_i++)
		{
		   // 获取下一个canvas		   
		   if((tCanvasWidget *)((*pst_canvas_tmp).sBase.pNext) == 0)
		   {
		      // 如果是最后一项，则他的下一项为0
			  break;
		   }
		   // 获取下一个项
		   pst_canvas_tmp = (tCanvasWidget *)((*pst_canvas_tmp).sBase.pNext); 
		   // 增加下标
		   *pu8_index = *pu8_index + 1;
		}

		// 设置最后一项反白
	    fn_canvas_set_reverse_color(pst_canvas_tmp);

		// 设置清除第一项反白
		pst_canvas_tmp = pst_canvas;
		// 清除当前反白
	    fn_canvas_clear_reverse_color(pst_canvas_tmp);
	}
	else
	{
		// 获取前一项
		*pu8_index = *pu8_index - 1;

		// 寻找到前一个项指针
		for(u8_i = 0; u8_i < *pu8_index; u8_i++)
		{
		   // 获取下一个canvas
		   pst_canvas_tmp = (tCanvasWidget *)((*pst_canvas_tmp).sBase.pNext); 
		   if(pst_canvas_tmp == 0)
		   {
		     return 0 ; 
		   }
		}
		
		// 设定前一项反白
	    fn_canvas_set_reverse_color(pst_canvas_tmp);	 
		
		// 设置清除当前项的反白
		pst_canvas_tmp = (tCanvasWidget *)((*pst_canvas_tmp).sBase.pNext); 
	    // 设置下一个反白
	    fn_canvas_clear_reverse_color(pst_canvas_tmp);
	}	
	return 1;
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  OnPaintMainCharacter 
// 函数功能  :  显示汉字
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void OnPaintMainCharacter(tWidget *pWidget, tContext *pContext)
{
   short s16_xmin = (*pWidget).sPosition.sXMin;
   short s16_ymin = (*pWidget).sPosition.sYMin;
   //short s16_xmax = (*pWidget).sPosition.sXMax;
   //short s16_ymax = (*pWidget).sPosition.sYMax;   
   tCanvasWidget *pst_canvas = (tCanvasWidget *)pWidget;
   unsigned long ulForeground = (*pst_canvas).ulTextColor;
   unsigned long ulBackground = (*pst_canvas).ulFillColor;
   if((pst_canvas->ulStyle) & CANVAS_STYLE_REVERSE_COLOR)
   {
       ulForeground = ClrWhite;
	   ulBackground = ClrBlack; 
   }
   else
   {
       ulForeground = ClrBlack;
	   ulBackground = ClrWhite; 
   }
   // printf("aa %s,\n",pst_canvas->pcText);
   GBStringPrint(s16_xmin,s16_ymin,(unsigned char *)pst_canvas->pcText,ulForeground,ulBackground);
}






// 主函数
int
main(void)
{

    // 初始化液晶显示
    Formike128x128x16Init();

    // 背光开启
    Formike128x128x16BacklightOn();

    // systick初始化函数
    //fn_systick_init();

    // 按键初始化
    fn_key_init();

    // 增加背景到根控件下
    WidgetAdd(WIDGET_ROOT, (tWidget *)&g_sBackground);

    // 发送绘制根控件命令
    WidgetPaint(WIDGET_ROOT);

    // 主循环
    while(1)
    {
        // 循环处理消息
        WidgetMessageQueueProcess();

		fn_key_process();
    }

}
