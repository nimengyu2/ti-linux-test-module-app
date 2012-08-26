//*****************************************************************************
//
// formike128x128x16.h - Prototypes for the Formike Electronic KWH015C04-F01
//                       display driver.
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

#ifndef __FORMIKE128X128X16_H__
#define __FORMIKE128X128X16_H__

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void Formike128x128x16Init(void);
extern void Formike128x128x16BacklightOn(void);
extern void Formike128x128x16BacklightOff(void);
extern const tDisplay g_sFormike128x128x16;
extern void fn_lcd160160_display_full_screen(void);
extern void Formike128x128x16PixelDraw(void *pvDisplayData, long lX, long lY,
                           unsigned long ulValue);
extern void Formike128x128x16PixelDrawMultiple(void *pvDisplayData, long lX, long lY,
                                   long lX0, long lCount, long lBPP,
                                   const unsigned char *pucData,
                                   const unsigned char *pucPalette);
extern void Formike128x128x16RectFill(void *pvDisplayData, const tRectangle *pRect,
                          unsigned long ulValue);
extern void Formike128x128x16LineDrawH(void *pvDisplayData, long lX1, long lX2, long lY,
                           unsigned long ulValue);
extern void Formike128x128x16LineDrawV(void *pvDisplayData, long lX, long lY1, long lY2,
                           unsigned long ulValue);


extern unsigned char g_u8_lcd160160_fresh_flag;

#endif // __FORMIKE128X128X16_H__
