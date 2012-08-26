//---------------------------------------------------------------------------//
//
//! @file gb_print.c
//! @brief 向屏幕打印字符 
//
//! @author Toby Zhang
//! @date 2010.07.11
//! @version v1.0
//! @note
//
//! 本文件使用图形库中的部分函数，并用用户自定义生成的字库，向屏幕打印字符\n
//!
//! ARM内核   : ARMv7M Cortex-M3\n
//! 使用芯片  : LM3S1968\n
//! 指令集    : Thumb2\n
//! 开发环境  : KEIL\n
//! 版权所有 (C)2005-2010 利尔达科技有限公司\n
//! \n
//! 修改日志  :\n
//! 2010.07.11    创建初试.Toby Zhang\n
//
//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//! \addtogroup gb_print_api
//! @{
//
//---------------------------------------------------------------------------//

#include "gb_print.h"
#include "grlib.h"
#include "formike128x128x16.h"


//---------------------------------------------------------------------------//
//
//! @brief 向屏幕打印一个中文字符 
//! @author Toby Zhang
//! @note
//! 本函数向屏幕打印一个中文字符，16*16点阵，遵循GB-2312标准 
// 
//! @param s32_x 屏幕上的x轴坐标 
//! @param s32_y 屏幕上的y轴坐标 
//! @param u8_index[2] 中文字符编码
//! @param font_gb_st16[] 用户自定义字库集 
//
//! @return void 
//
//  @deprecated
//! @since 2010.07.11
//! @see 无 
//
//---------------------------------------------------------------------------//
void GBPrintAscii(signed long s32_x,
                  signed long s32_y,
			      signed char s8_index,
				  unsigned long ulForeground,
			      unsigned long ulBackground)
{
    unsigned long i,j, k;
    unsigned char u8_data_buff; 
	const unsigned char *pu8_ascii_data;
	unsigned long u32_tmp = (s8_index - 32)*16;
	pu8_ascii_data = &g_x_font_asciii[u32_tmp];
    
	// 找到字符的字模数据，读出数据，向屏幕打印 
    for(j=0; j< 16; )
    {
        // 取出汉字左半部分字模数据
        u8_data_buff = *(pu8_ascii_data + j);

        // 循环汉字8列像素写入 
        for(k=0; k<8; k++)
		{
		    // 根据数据位打印不同的颜色
			if((u8_data_buff & 0x80) == 0x00)
			{
				// 为0，画背景色
			    DpyPixelDraw(&g_sFormike128x128x16, 
				             s32_x + k,
							 s32_y + j,
							 ulBackground); 
			}
			else
			{
				// 为1，画前景色
			    DpyPixelDraw(&g_sFormike128x128x16,
                             s32_x + k,
							 s32_y + j,
							 ulForeground);
			}
			u8_data_buff = u8_data_buff<<1;
		}
		
		// 字模索引指向后一个数据
        j++;                 
    }			

}


//---------------------------------------------------------------------------//
//
//! @brief 向屏幕打印一个中文字符 
//! @author Toby Zhang
//! @note
//! 本函数向屏幕打印一个中文字符，16*16点阵，遵循GB-2312标准 
// 
//! @param s32_x 屏幕上的x轴坐标 
//! @param s32_y 屏幕上的y轴坐标 
//! @param u8_index[2] 中文字符编码
//! @param font_gb_st16[] 用户自定义字库集 
//
//! @return void 
//
//  @deprecated
//! @since 2010.07.11
//! @see 无 
//
//---------------------------------------------------------------------------//
void GBPrintCharacter(unsigned long u32_x,
             unsigned long u32_y,
			 unsigned short u16_index,
			 unsigned long ulForeground,
		     unsigned long ulBackground)
{
    unsigned long u32_i,u32_j, u32_k;
    unsigned char u8_data_buff; 
	unsigned short u16_index_tmp;
	
	// 向用户自定义的字库中搜索需要打印的字符的字模
    for(u32_i = 0; u32_i < 200; u32_i++)
    {
	    // u16_index_tmp = *((unsigned short*)(g_x_font_gb_st16[u32_i].pu8_index));
            u16_index_tmp = *(g_x_font_gb_st16[u32_i].pu8_index + 1);
            u16_index_tmp = u16_index_tmp << 8;
            u16_index_tmp +=   *(g_x_font_gb_st16[u32_i].pu8_index + 0);
	    if(u16_index_tmp == 0)
		  break;
	    if(u16_index == u16_index_tmp)
	    {
		    // 找到字符的字模数据，读出数据，向屏幕打印 
            for(u32_j = 0; u32_j < 32; )
            {
                // 取出汉字左半部分字模数据
                u8_data_buff = g_x_font_gb_st16[u32_i].u8_mask[u32_j];

                // 循环汉字8列像素写入 
                for(u32_k = 0; u32_k < 8; u32_k++)
				{
				    // 根据数据位打印不同的颜色
					if((u8_data_buff & 0x80) == 0x00)
					{
						// 为0，画背景色
					    DpyPixelDraw(&g_sFormike128x128x16, 
						             u32_x + u32_k,
									 u32_y + u32_j / 2,
									 ulBackground); 
					}
					else
					{
						// 为1，画前景色
					    DpyPixelDraw(&g_sFormike128x128x16,
                                     u32_x + u32_k,
									 u32_y + u32_j / 2,
									 ulForeground);
					}
					u8_data_buff = u8_data_buff<<1;
				}
				
                // 字模索引指向后一个数据
                u32_j++;

                // 取出汉字右半部分字模数据
                u8_data_buff = g_x_font_gb_st16[u32_i].u8_mask[u32_j];

                // 循环汉字8列像素写入 
                for(u32_k=8; u32_k<16; u32_k++)
				{
				    // 根据数据位打印不同的颜色
					if((u8_data_buff & 0x80) == 0x00)
					{
						// 为0，画背景色 
					    DpyPixelDraw(&g_sFormike128x128x16,
						              u32_x + u32_k,
									  u32_y + u32_j / 2,
									  ulBackground); 
					}
					else
					{
						// 为1，画前景色 
					    DpyPixelDraw(&g_sFormike128x128x16,
                                     u32_x + u32_k,
									 u32_y + u32_j / 2,
									 ulForeground);
					}
					u8_data_buff = u8_data_buff<<1;
				}

                // 字模索引指向后一个数据
                u32_j++;                 
            }			

			// 完成字符的打印，跳出循环
			break;   
		}
	}
}

//---------------------------------------------------------------------------//
//
//! @brief 打印一个中文字符串 
//! @author Toby Zhang
//! @note
//! 向屏幕打印一个中文字符串。 
// 
//! @param s32_x 屏幕上的x轴坐标 
//! @param s32_y 屏幕上的y轴坐标 
//! @param u8_index[] 中文字符编码字符串 
//! @param font_gb_st16[] 用户自定义字库集
//
//! @return void
//
//  @deprecated
//! @since 2010.07.11
//! @see 参考GBStringPrint(). 
//
//---------------------------------------------------------------------------//
void GBStringPrint(unsigned long u32_x,
                   unsigned long u32_y,
			       unsigned char *pu8_index,
				   unsigned long ulForeground,
				   unsigned long ulBackground
				   )
{
    unsigned long u8_i;
    unsigned long u32_string_length = 0;
	unsigned short u16_index;
	 
    // 得到字符串的长度
	u32_string_length = strlen((const char*)pu8_index);
	
	// 分别打印字符串，每个字符之间空一个像素点
	for(u8_i=0; u8_i<u32_string_length; )
	{
        if(*pu8_index <= 0x7F)
		{
		    GBPrintAscii(u32_x,
		                 u32_y,
						 *pu8_index,
						 ulForeground,
					     ulBackground);
			
			// 修正中文字符指针指向下一个字符
	        pu8_index += 1;  
			u32_x += 8;
			u8_i++;
		}
		else
		{
		    //u16_index = *((unsigned short*)pu8_index);
		   u16_index = *(pu8_index + 1);
                   u16_index  = u16_index << 8;
                   u16_index += *(pu8_index + 0);

			// 向屏幕打印中文字符
	        GBPrintCharacter(u32_x,
		            u32_y,
	                u16_index,
					ulForeground,
					ulBackground);
	        
	        // 修正中文字符指针指向下一个字符
	        pu8_index += 2;  
			u8_i = u8_i + 2;
			u32_x += 16;
		}		
	}    
} 



//---------------------------------------------------------------------------//
//
// Close the Doxygen group.
//! @}
//
//---------------------------------------------------------------------------//
