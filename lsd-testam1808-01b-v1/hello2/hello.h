#ifndef __HELLO_H__
#define __HELLO_H__

#include "grlib.h"
#include "widget.h"
#include "canvas.h"
#include "pushbutton.h"

#define M_SCREEN_FIRST_FACE              0
#define M_SCREEN_MENU			         1
#define M_SCREEN_LOCALDATADISPLAY		 2
#define M_SCREEN_MEASUREDATADISPLAY		 3
#define M_SCREEN_MEASUREDATADETIAL		 4
#define M_SCREEN_PARAMETERSETTING		 5
#define M_SCREEN_TERMINALMANAGER		 6

#define M_SCREEN_CMT_CHANNAL_SET		 7
#define M_SCREEN_AREA_METER_SET		     8
#define M_SCREEN_AMR_INFO    		     9
#define M_SCREEN_LAST_5_MESSAGE          10

#define M_MEASURE_DATA_POINT_NUM  6
extern unsigned char g_pu8_measure_data_point_time_hour[M_MEASURE_DATA_POINT_NUM];	   // 测量点--抄表时间--时
extern unsigned char g_pu8_measure_data_point_time_minute[M_MEASURE_DATA_POINT_NUM];  // 测量点--抄表时间--分
extern unsigned char g_pu8_measure_data_point_time[M_MEASURE_DATA_POINT_NUM][8]; // 测量点--抄表时间--字符型
extern double g_f64_measure_data_point_zyj[M_MEASURE_DATA_POINT_NUM];	  // 测量点--正向有功尖电量
extern double g_f64_measure_data_point_zyf[M_MEASURE_DATA_POINT_NUM];	  // 测量点--正向有功峰电量
extern double g_f64_measure_data_point_zyp[M_MEASURE_DATA_POINT_NUM];	  // 测量点--正向有功平电量
extern double g_f64_measure_data_point_zyg[M_MEASURE_DATA_POINT_NUM];	  // 测量点--正向有功谷电量
extern double g_f64_measure_data_point_fyj[M_MEASURE_DATA_POINT_NUM];	  // 测量点--反向有功尖电量
extern double g_f64_measure_data_point_fyf[M_MEASURE_DATA_POINT_NUM];	  // 测量点--反向有功峰电量
extern double g_f64_measure_data_point_fyp[M_MEASURE_DATA_POINT_NUM];	  // 测量点--反向有功平电量
extern double g_f64_measure_data_point_fyg[M_MEASURE_DATA_POINT_NUM];	  // 测量点--反向有功谷电量

//-----------------------------------------------------------------------------
// 总体背景
//-----------------------------------------------------------------------------
extern tCanvasWidget g_sBackground;
//-----------------------------------------------------------------------------
// 顶部信息栏
//-----------------------------------------------------------------------------
extern tCanvasWidget g_sHeading;
extern tCanvasWidget g_sHeadSignal;
extern tCanvasWidget g_sHeadCmtGscWay;
extern tPushButtonWidget g_sHeadWarningFlag;
extern tCanvasWidget g_sHeadStandard;
extern tCanvasWidget g_sHeadShowMesData;
extern tCanvasWidget g_sHeadCurrentTime;
//-----------------------------------------------------------------------------
// 主要显示区域
//-----------------------------------------------------------------------------
extern tCanvasWidget g_sMain;
extern tCanvasWidget g_sMainInfoGround;
extern tCanvasWidget g_sMainInfo;
//-----------------------------------------------------------------------------
// 第0屏 开始界面
//-----------------------------------------------------------------------------
extern tCanvasWidget g_sMainFirstFace;
extern tCanvasWidget g_sMainFirstFaceHead;
extern tCanvasWidget g_sMainFirstFaceDate;
extern tCanvasWidget g_sMainFirstFaceTime;
//-----------------------------------------------------------------------------
// 第一屏 主菜单
//-----------------------------------------------------------------------------
extern tCanvasWidget g_sMainMenu;
extern tCanvasWidget g_sMainMenuHead;
extern tCanvasWidget g_sMainMenuLocalDataDisplay;
extern tCanvasWidget g_sMainMenuMeasureDataDisplay;
extern tCanvasWidget g_sMainMenuParameterSetting;
extern tCanvasWidget g_sMainMenuTerminalManager;
//-----------------------------------------------------------------------------
// 第二屏 本地交采数据显示
//-----------------------------------------------------------------------------
extern tCanvasWidget g_sMainLocalDataDisplay;
extern tCanvasWidget g_sMainLocalDataDisplayHead;
// 该屏下的第一子屏	 1/2
extern tCanvasWidget g_sMainLocalDataDisplayFirst;
extern tCanvasWidget g_sMainLocalDataDisplayFirstNum;
extern tCanvasWidget g_sMainLocalDataDisplayFirstAVoltage;
extern tCanvasWidget g_sMainLocalDataDisplayFirstBVoltage;
extern tCanvasWidget g_sMainLocalDataDisplayFirstCVoltage;
extern tCanvasWidget g_sMainLocalDataDisplayFirstACurrent;
extern tCanvasWidget g_sMainLocalDataDisplayFirstBCurrent;
extern tCanvasWidget g_sMainLocalDataDisplayFirstCCurrent;
// 该屏下的第二子屏 2/2
extern tCanvasWidget g_sMainLocalDataDisplaySecond;
extern tCanvasWidget g_sMainLocalDataDisplaySecondNum;
extern tCanvasWidget g_sMainLocalDataDisplaySecondAVoltageAnagle;
extern tCanvasWidget g_sMainLocalDataDisplaySecondBVoltageAnagle;
extern tCanvasWidget g_sMainLocalDataDisplaySecondCVoltageAnagle;
//-----------------------------------------------------------------------------
// 第三屏 测量点数据显示
//-----------------------------------------------------------------------------
// 第三屏 测量点数据显示
extern tCanvasWidget g_sMainMeasureDataDisplay;
extern tCanvasWidget g_sMainMeasureDataDisplayHead;
extern tCanvasWidget g_sMainMeasureDataDisplayPoint1;
extern tCanvasWidget g_sMainMeasureDataDisplayPoint2;
extern tCanvasWidget g_sMainMeasureDataDisplayPoint3;
extern tCanvasWidget g_sMainMeasureDataDisplayPoint4;
extern tCanvasWidget g_sMainMeasureDataDisplayPoint5;
extern tCanvasWidget g_sMainMeasureDataDisplayPoint6;
extern tCanvasWidget g_sMainMeasureDataDisplayNum;

//-----------------------------------------------------------------------------
// 第四屏 测量点数据详细显示
//-----------------------------------------------------------------------------
extern tCanvasWidget g_sMainMeasureDataDetial;
extern tCanvasWidget g_sMainMeasureDataDetialHead;
extern tCanvasWidget g_sMainMeasureDataDetialCircleGround;
extern tCanvasWidget g_sMainMeasureDataDetialCircleConfirm;
extern tCanvasWidget g_sMainMeasureDataDetialCircleCancel;
extern tCanvasWidget g_sMainMeasureDataDetialFirst;
extern tCanvasWidget g_sMainMeasureDataDetialTimeInfo;
extern tCanvasWidget g_sMainMeasureDataDetialTime;
extern tCanvasWidget g_sMainMeasureDataDetialZYJ;
extern tCanvasWidget g_sMainMeasureDataDetialZYF;
extern tCanvasWidget g_sMainMeasureDataDetialZYP;
extern tCanvasWidget g_sMainMeasureDataDetialZYG;
extern tCanvasWidget g_sMainMeasureDataDetialFYJ;
extern tCanvasWidget g_sMainMeasureDataDetialFirstNum;
extern tCanvasWidget g_sMainMeasureDataDetialSecond;
extern tCanvasWidget g_sMainMeasureDataDetialFYF;
extern tCanvasWidget g_sMainMeasureDataDetialFYP;
extern tCanvasWidget g_sMainMeasureDataDetialFYG;
extern tCanvasWidget g_sMainMeasureDataDetialSecondNum;

//-----------------------------------------------------------------------------
// 第五屏 主菜单
//-----------------------------------------------------------------------------
// 第五屏 主菜单
extern tCanvasWidget g_sMainParaSet;
extern tCanvasWidget g_sMainParaSetHead;
extern tCanvasWidget g_sMainParaSetCmtChannelSet;
extern tCanvasWidget g_sMainParaSetAreaMeterSet;
extern tCanvasWidget g_sMainParaSetCentralMeterSet;
extern tCanvasWidget g_sMainParaSetTimeSet;
extern tCanvasWidget g_sMainParaSetPasswdSet;
extern tCanvasWidget g_sMainParaSetNumberSet;
extern tCanvasWidget g_sMainParaSetNum;

//-----------------------------------------------------------------------------
// 第六屏 通讯通道设置
//-----------------------------------------------------------------------------
extern tCanvasWidget g_sMainCmtChannalSet;
extern tCanvasWidget g_sMainCmtChannalSetHead;
extern tCanvasWidget g_sMainCmtChannalSetCircleGround;
extern tCanvasWidget g_sMainCmtChannalSetCircleConfirm;
extern tCanvasWidget g_sMainCmtChannalSetCircleCancel;
extern tCanvasWidget g_sMainCmtChannalSetFirst;
extern tCanvasWidget g_sMainCmtChannalSetHostIP;
extern tCanvasWidget g_sMainCmtChannalSetLocalIP;
extern tCanvasWidget g_sMainCmtChannalSetTermIP;
extern tCanvasWidget g_sMainCmtChannalSetPortNum;
extern tCanvasWidget g_sMainCmtChannalSetAPN;
extern tCanvasWidget g_sMainCmtChannalSetUserName;
extern tCanvasWidget g_sMainCmtChannalSetFirstNum;
extern tCanvasWidget g_sMainCmtChannalSetSecond;
extern tCanvasWidget g_sMainCmtChannalSetPasswd;
extern tCanvasWidget g_sMainCmtChannalSetPeriod;
extern tCanvasWidget g_sMainCmtChannalSetSecondNum;
//-----------------------------------------------------------------------------
// 第七屏 台区电表参数查看
//-----------------------------------------------------------------------------
// 第七屏 台区电表参数查看
extern tCanvasWidget g_sMainAreaMeterSet;
extern tCanvasWidget g_sMainAreaMeterSetHead;
extern tCanvasWidget g_sMainAreaMeterSetFirst;
extern tCanvasWidget g_sMainAreaMeterSetPort1Interval;
extern tCanvasWidget g_sMainAreaMeterSetPort2Interval;
extern tCanvasWidget g_sMainAreaMeterSetPort3Interval;
extern tCanvasWidget g_sMainAreaMeterSetFirstNum;
//-----------------------------------------------------------------------------
// 第八屏 终端管理与维护
//-----------------------------------------------------------------------------
// 第八屏 终端管理与维护
extern tCanvasWidget g_sMainTermSet;
extern tCanvasWidget g_sMainTermSetHead;
extern tCanvasWidget g_sMainTermSetAMRInfo;
extern tCanvasWidget g_sMainTermSetLast5Message;
extern tCanvasWidget g_sMainTermSetSystemReset;
extern tCanvasWidget g_sMainTermSetNum;

//-----------------------------------------------------------------------------
// 第九屏 集中器版本
//-----------------------------------------------------------------------------
// 第九屏 集中器版本
extern tCanvasWidget g_sMainAMRInfo;
extern tCanvasWidget g_sMainAMRInfoHead;
extern tCanvasWidget g_sMainAMRInfoVersion;
extern tCanvasWidget g_sMainAMRInfoSoftVersion;

//-----------------------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------------------
extern void OnPaintStringDisplay(tWidget *pWidget, tContext *pContext);
extern void OnPaintCmtChannalSetIPDisplay(tWidget *pWidget, tContext *pContext);
extern void OnPaintMainMeasureDataDetialPoint(tWidget *pWidget, tContext *pContext);
extern void OnPaintMainMeasureDataDetialTime(tWidget *pWidget, tContext *pContext);
extern void OnPaintMainFirstFaceDate(tWidget *pWidget, tContext *pContext);
extern void OnPaintMainFirstFaceTime(tWidget *pWidget, tContext *pContext);
extern void OnPaintMainCharacter(tWidget *pWidget, tContext *pContext);
extern void fn_set_cavas_fore_back_ground(tCanvasWidget *pst_canvas,unsigned long ulForeground,unsigned long ulBackground);
extern void fn_canvas_set_reverse_color(tCanvasWidget *pst_canvas);
extern void fn_canvas_clear_reverse_color(tCanvasWidget *pst_canvas);
extern void OnPaintMainLocalDataDisplay(tWidget *pWidget, tContext *pContext);
extern int fn_canvas_next_index(tCanvasWidget *pst_canvas,unsigned char *pu8_index);
extern int fn_canvas_prev_index(tCanvasWidget *pst_canvas,unsigned char *pu8_index);

extern unsigned char g_u8_current_screen;


#endif

