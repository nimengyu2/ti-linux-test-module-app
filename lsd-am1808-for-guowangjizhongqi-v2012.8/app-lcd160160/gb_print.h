#ifndef __GB_PRINTF_H__
#define __GB_PRINTF_H__

//---------------------------------------------------------------------------//
//
//! \addtogroup gb_print_api
//! @{
//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
// c++编译器约束
//
//---------------------------------------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------//
//
// 添加硬件驱动头文件
//
//---------------------------------------------------------------------------//
#include <string.h>

//---------------------------------------------------------------------------//
//
// 添加用户自定义头文件
//
//---------------------------------------------------------------------------//
#include "font_gb_st16.h"
					 
//---------------------------------------------------------------------------//
//
// 宏定义
//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
// 函数声明
//
//---------------------------------------------------------------------------//
extern void GBPrint(signed long s32_x,
                    signed long s32_y,
			        const signed char s8_index[2],
			        const font_gb16  font_gb_st16[],
					unsigned long ulForeground,
				    unsigned long ulBackground);
extern void GBStringPrint(unsigned long u32_x,
                   unsigned long u32_y,
			       unsigned char *pu8_index,
				   unsigned long ulForeground,
				   unsigned long ulBackground
				   );
extern void put_hanzi_p(unsigned char add_x,unsigned char add_y,unsigned long color,char *hanzi_str) ;

//---------------------------------------------------------------------------//
//
// c++编译器约束
//
//---------------------------------------------------------------------------//
#ifdef __cpluscplus
           }
#endif

//---------------------------------------------------------------------------//
//
// Close the Doxygen group.
//! @}
//
//---------------------------------------------------------------------------//
#endif // __GB_PRINTF_H__
