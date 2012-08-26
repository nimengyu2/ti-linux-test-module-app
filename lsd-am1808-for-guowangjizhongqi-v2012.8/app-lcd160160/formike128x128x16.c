//*****************************************************************************
//
// formike128x128x16.c - Display driver for the Formike Electronic
//                       KWH015C04-F01 color STN panel with an ST7637
//                       controller.
//
// Copyright (c) 2008-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 5727 of the EK-LM3S3748 Firmware Package.
//
//*****************************************************************************
// nmy modify 20110522 suit amr1808	lcd160160
// (1) In function fn_lcd160160_init() modify 
//     from   fn_lcd160160_write_cmd(0xc4);	 to  fn_lcd160160_write_cmd(0xc0);
//	   fn_lcd160160_write_cmd(0xc0);			//enable FLT and FLB, 18:SET LCD MAPPING CONTROL
// (2) In function fn_lcd160160_display_full_screen() modify from ... to follow function ...


//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
#include "grlib.h"
#include "formike128x128x16.h"

#define DEVICE_NAME "am1808-lcd160160" //设备名(/dev/lcd) 

#define MOTOR_MAGIC1 'D'
#define LCD_INIT 		_IOW(MOTOR_MAGIC1, 2,int)
#define LCD_WRITE_CMD	_IOW(MOTOR_MAGIC1, 3,int)
#define LCD_WRITE_DATA  _IOW(MOTOR_MAGIC1, 4,int)

#define DEVICE_NAME "am1808-gpio" //设备名(/dev/led) 
#define MOTOR_MAGIC 'g'
#define SET_GPIO_AD_RST 		_IOW(MOTOR_MAGIC, 2,int)    // GPIO7[13]
#define SET_GPIO_LCD_RST  		_IOW(MOTOR_MAGIC, 3,int)	// GPIO7[15]
#define SET_GPIO_WD_FD  		_IOW(MOTOR_MAGIC, 4,int)	// GPIO7[14]
#define SET_GPIO_GPRS_DTR  		_IOW(MOTOR_MAGIC, 6,int)	// GPIO7[9]
#define SET_GPIO_IRDA_CTL  		_IOW(MOTOR_MAGIC, 9,int)	// GPIO8[10]
#define SET_GPIO_BAT_CTL  		_IOW(MOTOR_MAGIC, 10,int)	// GPIO8[8]
#define SET_GPIO_LCD_BL  		_IOW(MOTOR_MAGIC, 11,int)	// GPIO7[11]

#define GET_GPIO_BTN_INT  		_IOW(MOTOR_MAGIC, 12,int)	// GPIO7[12]
#define GET_GPIO_AD_INT  		_IOW(MOTOR_MAGIC, 13,int)	// GPIO8[11]	
#define GET_GPIO_GPRS_DSR  		_IOW(MOTOR_MAGIC, 5,int)	// GPIO7[10]
#define GET_GPIO_GPRS_DCD  		_IOW(MOTOR_MAGIC, 7,int)	// GPIO7[8]
#define GET_GPIO_GPRS_RI  		_IOW(MOTOR_MAGIC, 8,int)	// GPIO8[9]

unsigned char g_u8_lcd160160_fresh_flag = 0;
int fd_lcd160160; 
int fd_am1808_gpio;
char g_u8_lcd_open_flag = 0;

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_lcd160160_write_data 
// 函数功能  :  
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
static void fn_lcd160160_write_data(unsigned char u8_data)
{
	   if(g_u8_lcd_open_flag)
  	 {
		ioctl(fd_lcd160160, LCD_WRITE_DATA,u8_data); 
  	 }
}

//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_lcd160160_write_cmd 
// 函数功能  :  写一个字节命令
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
static void fn_lcd160160_write_cmd(unsigned char u8_cmd)
{
	  if(g_u8_lcd_open_flag)
    {
       ioctl(fd_lcd160160, LCD_WRITE_CMD,u8_cmd); 
    }
}



// lcd 160*160 160行 54列，每列表示3个像素点，每个像素点表示4个bit，因此一个地址
// 需要写1.5个字节，因为半个字节不能写，因此只能一次性写3个字节，表示6个像素点
// 每个像素用4bit表示，根据最高为表示是否显示，如果最高为1，则该像素点显示，否则
// 这个像素点不显示，比如1000 1110 显示，比如0111 0101 不显示


// lcd 160*160的全屏数据缓冲区
// y轴是160行，x轴是每两个点使用1个字节
unsigned char g_pu8_lcd160160_full_data[160][(160/2) + 1];
unsigned char g_pu8_lcd1160160_row_data_tmp[81];
unsigned char pu8_lcd_buf_a[160][160];
unsigned char pu8_lcd_buf_b[160][160];
unsigned char pu8_lcd_buf_c[160][81];
//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_lcd160160_display_full_screen 
// 函数功能  :  LCD 160*160全屏显示
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void fn_lcd160160_display_full_screen(void)
{
#if 0    
	short s8_i;
	unsigned char u8_j;
	unsigned char u8_tmph,u8_tmpl;
	unsigned char* pu8_data;
	pu8_data = (unsigned char*)g_pu8_lcd160160_full_data;

	#if 1
	fn_lcd160160_write_cmd(0x60);			//row address LSB
	fn_lcd160160_write_cmd(0x70);			//row address MSB
	fn_lcd160160_write_cmd(0x05);			//culomn address LSB
	fn_lcd160160_write_cmd(0x12);			//culomn address MSB
	#endif
	#if 0
	fn_lcd160160_write_cmd(0x6f);			//row address LSB
	fn_lcd160160_write_cmd(0x79);			//row address MSB
	fn_lcd160160_write_cmd(0x05);			//culomn address LSB
	fn_lcd160160_write_cmd(0x12);			//culomn address MSB
	#endif


	for(u8_j = 0;u8_j < 160; u8_j++)
	{

		for(s8_i = 80; s8_i >= 1 ; s8_i--)
		{
		   u8_tmpl = (g_pu8_lcd160160_full_data[u8_j][s8_i-1] >> 4)&0x0f;
		   u8_tmph = (g_pu8_lcd160160_full_data[u8_j][s8_i-1] << 4)&0xf0;
		   g_pu8_lcd1160160_row_data_tmp[s8_i] = u8_tmph + u8_tmpl; 
		}
		g_pu8_lcd160160_full_data[u8_j][s8_i] = 0x00;
		pu8_data = &g_pu8_lcd1160160_row_data_tmp[80];
		
		for(s8_i = 0; s8_i < 27; s8_i++)
		{
			fn_lcd160160_write_data(*pu8_data--);
			fn_lcd160160_write_data(*pu8_data--);
			fn_lcd160160_write_data(*pu8_data--);	
		}
	}
#endif 
#if 1
	unsigned char u8_i;
	unsigned char u8_j;
	unsigned char* pu8_data;
	pu8_data = (unsigned char*)g_pu8_lcd160160_full_data;

	// Éè¶šÆðÊŒµØÖ·
	#if 1
	fn_lcd160160_write_cmd(0x60);			//row address LSB
	fn_lcd160160_write_cmd(0x70);			//row address MSB
	fn_lcd160160_write_cmd(0x05);			//culomn address LSB
	fn_lcd160160_write_cmd(0x12);			//culomn address MSB
	#endif
	#if 0
	fn_lcd160160_write_cmd(0x6f);			//row address LSB
	fn_lcd160160_write_cmd(0x79);			//row address MSB
	fn_lcd160160_write_cmd(0x05);			//culomn address LSB
	fn_lcd160160_write_cmd(0x12);			//culomn address MSB
	#endif
	for(u8_i = 0;u8_i < 160; u8_i++)
	{
		for(u8_j = 0; u8_j < 80; u8_j++)
		{
			if((g_pu8_lcd160160_full_data[u8_i][u8_j] & 0xf0) != 0)
				pu8_lcd_buf_a[u8_i][2*u8_j+0] = 0xff;
			else
				pu8_lcd_buf_a[u8_i][2*u8_j+0] = 0x00;

			if((g_pu8_lcd160160_full_data[u8_i][u8_j] & 0x0f) != 0)
				pu8_lcd_buf_a[u8_i][2*u8_j+1] = 0xff;
			else
				pu8_lcd_buf_a[u8_i][2*u8_j+1] = 0x00;		
		}
	}

	for(u8_i = 0;u8_i < 160; u8_i++)
	{
		for(u8_j = 0; u8_j < 160; u8_j++)
		{
			pu8_lcd_buf_b[159-u8_j][u8_i] = pu8_lcd_buf_a[u8_i][u8_j];
		}
	}

	for(u8_i = 0;u8_i < 160; u8_i++)
	{
		for(u8_j = 0; u8_j < 80; u8_j++)
		{
			pu8_lcd_buf_c[u8_i][u8_j] = ((pu8_lcd_buf_b[u8_i][2*u8_j + 0] << 4)&0xf0) + 
										((pu8_lcd_buf_b[u8_i][2*u8_j + 1] << 0)&0x0f);
		}
		pu8_lcd_buf_c[u8_i][u8_j] = 0x00;
	}

	pu8_data = (unsigned char*)pu8_lcd_buf_c;
	for(u8_j = 0;u8_j < 160; u8_j++)
	{
		for(u8_i = 0; u8_i < 27; u8_i++)
		{
			fn_lcd160160_write_data(*pu8_data++);
			fn_lcd160160_write_data(*pu8_data++);
			fn_lcd160160_write_data(*pu8_data++);	
		}
	}
#endif
}


//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_lcd160160_clear_full_screen 
// 函数功能  :  LCD 160*160 全屏清除
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void fn_lcd160160_clear_full_screen(void)
{
    unsigned char u8_i;
    unsigned char u8_j; 
	for(u8_j = 0;u8_j < 160; u8_j++)
	{
	   	for(u8_i = 0; u8_i < 81; u8_i++)
		{
		   // 清除为0
		   g_pu8_lcd160160_full_data[u8_j][u8_i] = 0x00;
		} 
	}
}




//-----------------------------------------------------------------------------
//
// 函数名称  :  fn_lcd160160_fill_full_screen 
// 函数功能  :  LCD 160*160 全屏填充
//
// 输入参数  :  无
//
// 返回参数  :  无
// 
//-----------------------------------------------------------------------------
void fn_lcd160160_fill_full_screen(void)
{
    unsigned char u8_i;
    unsigned char u8_j; 
	for(u8_j = 0;u8_j < 160; u8_j++)
	{
	   	for(u8_i = 0; u8_i < 81; u8_i++)
		{
		    // 填充为0xff
			g_pu8_lcd160160_full_data[u8_j][u8_i] = 0xff;
		} 
	}
}

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the ST7637 display controller on the panel,
//! preparing it to display data.
//!
//! \return None.
//
//*****************************************************************************
void
Formike128x128x16Init(void)
{
   fd_lcd160160 = open("/dev/am1808-lcd160160", 0); 
   if (fd_lcd160160 < 0) 
   { 
      g_u8_lcd_open_flag = 0;
      perror("open device leds error"); 
      exit(1); 
   }   

   fd_am1808_gpio = open("/dev/am1808-gpio", 0); 
   if (fd_am1808_gpio < 0) 
   { 
       perror("open device am1808-gpio error"); 
       exit(1); 
   } 

   ioctl(fd_am1808_gpio, SET_GPIO_LCD_BL,1); 
   ioctl(fd_am1808_gpio, SET_GPIO_LCD_RST,0); 
   usleep(20000);
   ioctl(fd_am1808_gpio, SET_GPIO_LCD_RST,1); 
   usleep(100000);

   g_u8_lcd_open_flag = 1;
   ioctl(fd_lcd160160, LCD_INIT,0); 

   fn_lcd160160_fill_full_screen();
   fn_lcd160160_display_full_screen();
   //fn_delay_ms(200);

   fn_lcd160160_clear_full_screen();
   fn_lcd160160_display_full_screen();
   //fn_delay_ms(200);

}

//*****************************************************************************
//
//! Turns on the backlight.
//!
//! This function turns on the backlight on the display.
//!
//! \return None.
//
//*****************************************************************************
void
Formike128x128x16BacklightOn(void)
{
    //
    // Assert the signal that turns on the backlight.
    //
    //HWREG(LCD_BL_BASE + GPIO_O_DATA + (LCD_BL_PIN << 2)) = LCD_BL_PIN;
}

//*****************************************************************************
//
//! Turns off the backlight.
//!
//! This function turns off the backlight on the display.
//!
//! \return None.
//
//*****************************************************************************
void
Formike128x128x16BacklightOff(void)
{
    //
    // Deassert the signal that turns on the backlight.
    //
    //HWREG(LCD_BL_BASE + GPIO_O_DATA + (LCD_BL_PIN << 2)) = 0;
}

//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void Formike128x128x16PixelDraw(void *pvDisplayData, long lX, long lY,
                           unsigned long ulValue)
{
   unsigned long u32_x,u32_y;
   u32_x = (unsigned long)lX;
   u32_y = (unsigned long)lY;
   if((u32_x & 0x01) == 0x01)
	{
	    if(ulValue == ClrBlack)
		{
		   g_pu8_lcd160160_full_data[u32_y][u32_x/2] |= 0x0f;
		}
		else if(ulValue == ClrWhite)
		{
		   g_pu8_lcd160160_full_data[u32_y][u32_x/2] &= 0xf0;
		}
	}
	else
	{
	    if(ulValue == ClrBlack)
		{
		   g_pu8_lcd160160_full_data[u32_y][u32_x/2] |= 0xf0;
		}
		else if(ulValue == ClrWhite)
		{
		   g_pu8_lcd160160_full_data[u32_y][u32_x/2] &= 0x0f;
		}
	}
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the first pixel.
//! \param lY is the Y coordinate of the first pixel.
//! \param lX0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param lCount is the number of pixels to draw.
//! \param lBPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param pucData is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pucPalette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
// lX0 应该是0-7 表示这个字节中的第几个像素点，从左到右，如果lX0=1，则表示
//     从左开始的第2个像素点，即这个字节bit7-bit0中的bit6
// pucData 中高位代表最左边的像素点
void
Formike128x128x16PixelDrawMultiple(void *pvDisplayData, long lX, long lY,
                                   long lX0, long lCount, long lBPP,
                                   const unsigned char *pucData,
                                   const unsigned char *pucPalette)
{
    unsigned long ulByte;
	unsigned long u32_x,u32_y;
	u32_x = (unsigned long)lX;
    u32_y = (unsigned long)lY;

    //
    // Determine how to interpret the pixel data based on the number of bits
    // per pixel.
    //
    switch(lBPP)
    {
        //
        // The pixel data is in 1 bit per pixel format.
        //
        case 1:
        {
            //
            // Loop while there are more pixels to draw.
            //
            while(lCount)
            {
                //
                // Get the next byte of image data.
                //
                ulByte = *pucData++;

                //
                // Loop through the pixels in this byte of image data.
                //
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    //
                    // Draw this pixel in the appropriate color.
                    //
                    lBPP = (ulByte >>(7 - lX0)) & 1;
                  	
					if((u32_x & 0x01) == 0x01)
					{
					    if(lBPP != 0)
						{
						   g_pu8_lcd160160_full_data[u32_y][u32_x/2] |= 0x0f;
						}
						else
						{
						   g_pu8_lcd160160_full_data[u32_y][u32_x/2] &= 0xf0;
						}
					}
					else
					{
					    if(lBPP != 0)
						{
						   g_pu8_lcd160160_full_data[u32_y][u32_x/2] |= 0xf0;
						}
						else
						{
						   g_pu8_lcd160160_full_data[u32_y][u32_x/2] &= 0x0f;
						}
					} 
					u32_x++;
                }

                //
                // Start at the beginning of the next byte of image data.
                //
                lX0 = 0;
            }

            //
            // The image data has been drawn.
            //
            break;
        }

        //
        // The pixel data is in 4 bit per pixel format.
        //
        case 4:
        {
           
            break;
        }

        //
        // The pixel data is in 8 bit per pixel format.
        //
        case 8:
        {
           
            break;
        }
    }
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.  For the ST7637
//! driver, the flush is a no operation.
//!
//! \return None.
//
//*****************************************************************************
static void
Formike128x128x16Flush(void *pvDisplayData)
{
    //
    // There is nothing to be done.
    //
	fn_lcd160160_display_full_screen();
}

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void
Formike128x128x16LineDrawH(void *pvDisplayData, long lX1, long lX2, long lY,
                           unsigned long ulValue)
{
     long u32_i;
	 for(u32_i = lX1; u32_i <= lX2; u32_i++)
	 {
		Formike128x128x16PixelDraw(pvDisplayData,u32_i,lY,ulValue);
	 }
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void
Formike128x128x16LineDrawV(void *pvDisplayData, long lX, long lY1, long lY2,
                           unsigned long ulValue)
{
     long u32_i;
	 for(u32_i = lY1; u32_i <= lY2; u32_i++)
	 {
	    Formike128x128x16PixelDraw(pvDisplayData,lX,u32_i,ulValue);
	 }
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both sXMin and
//! sXMax are drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
void
Formike128x128x16RectFill(void *pvDisplayData, const tRectangle *pRect,
                          unsigned long ulValue)
{
    //long lCount;

    long u32_i;
	for(u32_i = pRect->sYMin; u32_i <= pRect->sYMax; u32_i++)
	{
	   Formike128x128x16LineDrawH((void*)0,pRect->sXMin,pRect->sXMax,u32_i,ulValue);
	}
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ulValue is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static unsigned long
Formike128x128x16ColorTranslate(void *pvDisplayData, unsigned long ulValue)
{
    //
    // Translate from a 24-bit RGB color to a 5-6-5 RGB color.
    //
    return(ulValue);
}

//*****************************************************************************
//
//! The graphics library display structure that describes the driver for the
//! Formike Electronic KWH015C04-F01 color STN panel with an ST7637 controller.
//
//*****************************************************************************
const tDisplay g_sFormike128x128x16 =
{
    sizeof(tDisplay),
    0,
    160,
    160,
    Formike128x128x16PixelDraw,
    Formike128x128x16PixelDrawMultiple,
    Formike128x128x16LineDrawH,
    Formike128x128x16LineDrawV,
    Formike128x128x16RectFill,
    Formike128x128x16ColorTranslate,
    Formike128x128x16Flush
};

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
