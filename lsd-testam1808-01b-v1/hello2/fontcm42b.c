//*****************************************************************************
//
// fontcm42b.c - Font definition for the 42 point Cm bold font.
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
// This is part of revision 5727 of the Stellaris Graphics Library.
//
//*****************************************************************************

//*****************************************************************************
//
// This file is generated by ftrasterize; DO NOT EDIT BY HAND!
//
//*****************************************************************************

#include "grlib.h"

//*****************************************************************************
//
// Details of this font:
//     Style: cm
//     Size: 42 point
//     Bold: yes
//     Italic: no
//     Memory usage: 5284 bytes
//
//*****************************************************************************

//*****************************************************************************
//
// The compressed data for the 42 point Cm bold font.
//
//*****************************************************************************
static const unsigned char g_pucCm42bData[5083] =
{
      5,  17,   0,  93,  64,  36,  11, 240, 240,  84, 102,  71,
     71,  71,  71,  71,  85, 101, 101, 101, 101, 101, 115, 131,
    131, 131, 131, 131, 131, 240, 240, 179, 117,  87,  71,  71,
     85, 115,   0,  15, 112,  35,  21,   0,  10,  99, 115, 117,
     85,  86,  70,  87,  55,  71,  55,  86,  70,  86,  70, 146,
    130, 146, 130, 145, 145, 146, 130, 130, 130, 130, 130, 130,
    130,   0,  69,  32, 110,  34,   0,  18,  98, 114, 240, 130,
    114, 240, 115,  99, 240, 115,  99, 240, 114, 115, 240, 114,
    114, 240, 115,  99, 240, 115,  99, 240, 114, 115, 240, 114,
    114, 240, 115,  99, 240, 115,  99, 240, 114, 115, 191,  15,
     79,  15, 227,  99, 240, 114, 114, 240, 115,  99, 240, 115,
     99, 240, 115,  99, 240, 114, 114, 240, 115,  99, 239,  15,
     79,  15, 179, 114, 240, 115,  99, 240, 115,  99, 240, 114,
    114, 240, 115, 114, 240, 115,  99, 240, 115,  99, 240, 114,
    114, 240, 115, 114, 240, 115,  99, 240, 115,  99, 240, 114,
    114, 240, 130, 114,   0,  15,  76,  20, 240, 194, 240,  50,
    240,  50, 249, 156, 131,  34,  51,  99,  50,  66,  83,  66,
     52,  67,  66,  37,  67,  66,  37,  68,  50,  37,  68,  50,
     51,  86,  18, 185, 203, 156, 156, 156, 155, 185, 201, 178,
     37,  83,  50,  52,  69,  34,  67,  69,  34,  67,  69,  34,
     67,  68,  50,  67,  81,  82,  66,  98,  66,  51, 115,  34,
     35, 154, 198, 240,  18, 240,  50,   0,  23, 112, 115,  35,
     84, 240,  18, 183, 243, 148,  35, 211, 149,  52, 164, 148,
     70, 101, 164,  95, 165,  82,  54,  19, 165,  82, 147, 181,
     82, 131, 197,  82, 131, 197,  82, 115, 213,  82,  99, 229,
     82,  99, 244,  82,  83, 240,  20,  66,  83, 240,  37,  50,
     83, 240,  52,  34,  83, 240,  87,  68, 240, 240,  19, 240,
    240,  19, 103, 240,  52,  84,  35, 240,  35,  84,  66, 240,
     19, 100,  66, 244,  85,  82, 227, 101,  82, 211, 117,  82,
    196, 117,  82, 195, 133,  82, 179, 149,  82, 164, 149,  82,
    163, 180,  67, 147, 196,  66, 148, 212,  35, 147, 247, 162,
    240,  37,   0,  40,  32,  89,  32,   0,   9,  20, 240, 183,
    240, 148,  34, 240, 133,  50, 240, 116,  66, 240, 101,  66,
    240, 101,  66, 240, 101,  66, 240, 101,  65, 240, 117,  50,
    240, 117,  49, 240, 134,  18, 240, 151, 121, 150, 137, 149,
    195, 198, 178, 229, 162, 231, 146, 210,  21, 130, 211,  22,
     99, 196,  38,  82, 196,  69,  66, 197,  70,  35, 197,  86,
     18, 213, 103, 229, 102, 246, 102, 130,  85, 118, 114,  86,
     67,  22,  51, 138,  58, 182, 134,   0,  44, 112,  21,  11,
      0,   5,  99, 117,  86,  87,  71,  86,  86, 146, 146, 145,
    146, 130, 130, 130,   0,  36,  96,  47,  15, 240, 146, 194,
    195, 179, 179, 195, 180, 164, 180, 164, 180, 180, 165, 165,
    164, 180, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
    165, 165, 180, 181, 165, 165, 180, 180, 196, 180, 196, 180,
    196, 195, 211, 210, 226,  64,  46,  15, 242, 226, 211, 211,
    211, 196, 195, 196, 180, 196, 180, 180, 181, 165, 180, 181,
    165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
    164, 165, 165, 164, 180, 180, 164, 180, 179, 180, 179, 179,
    179, 194, 194, 208,  45,  20, 240, 194, 240,  36, 240,  20,
    240,  20, 178,  52,  50,  84,  50,  52,  69,  34,  37,  85,
     18,  21, 138, 212, 232, 172, 102,  18,  22,  68,  50,  52,
     67,  67,  51, 164, 240,  20, 240,  20, 240,  34,   0,  61,
     48,  64,  33,   0,  34,  83, 240, 243, 240, 243, 240, 243,
    240, 243, 240, 243, 240, 243, 240, 243, 240, 243, 240, 243,
    240, 243, 240, 243, 240, 243, 240, 243, 240,  47,  14,  79,
     14, 240,  35, 240, 243, 240, 243, 240, 243, 240, 243, 240,
    243, 240, 243, 240, 243, 240, 243, 240, 243, 240, 243, 240,
    243, 240, 243,   0,  31,  22,  11,   0,  36,   3, 117,  86,
     87,  71,  86,  86, 146, 146, 145, 146, 145, 146, 130, 145,
    240, 240, 176,  11,  15,   0,  41,  43,  75,  75,  75,   0,
     34,  32,  14,  11,   0,  36,   3, 117,  87,  71,  71,  85,
    115,   0,  15, 112,  91,  20, 240, 240,  66, 240,  35, 240,
     35, 240,  34, 240,  35, 240,  35, 240,  34, 240,  35, 240,
     35, 240,  34, 240,  35, 240,  35, 240,  34, 240,  35, 240,
     35, 240,  34, 240,  35, 240,  35, 240,  34, 240,  35, 240,
     35, 240,  34, 240,  35, 240,  35, 240,  34, 240,  35, 240,
     35, 240,  34, 240,  35, 240,  35, 240,  34, 240,  35, 240,
     35, 240,  34, 240,  35, 240,  35, 240,  34, 240,  35, 240,
     35, 240,  34, 240,  35, 240,  35, 240,  34, 240,  48,  59,
     21,   0,  13, 118, 217, 180,  52, 148,  84, 131, 116, 100,
    116, 100, 116, 100, 117,  69, 117,  69, 117,  69, 117,  69,
    117,  69, 117,  69, 117,  69, 117,  69, 117,  69, 117,  69,
    117,  69, 117,  69, 117,  84, 117,  84, 116, 100, 116, 100,
    116, 116,  84, 148,  52, 185, 214,   0,  30,  16,  37,  19,
      0,  12, 114, 240,  19, 214, 154, 148,  21, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 159,  79,   0,  26,  80,  54,
     21,   0,  13, 102, 218, 163,  70, 115, 102,  85,  86,  86,
     86,  70,  86,  70,  86,  70,  86,  84, 102, 246, 245, 246,
    245, 245, 240,  20, 240,  20, 240,  20, 240,  20, 240,  35,
     98, 147, 114, 131, 130, 115, 130, 126, 126, 111,  95,   1,
     95,   1,   0,  29,  64,  57,  21,   0,  13, 103, 218, 147,
     85, 130, 116, 117,  85, 102,  69, 102,  69, 101,  85, 116,
     84, 240,  21, 240,  20, 240,  20, 215, 232, 240,  53, 240,
     22, 240,  21, 240,  22,  84, 102,  70,  86,  70,  86,  70,
     86,  70,  86,  69, 101,  99, 102, 115,  70, 154, 214,   0,
     30,  16,  57,  22,   0,  15,  50, 240,  67, 240,  52, 240,
     37, 240,  37, 240,  22, 247, 247, 226,  21, 210,  37, 210,
     37, 194,  53, 178,  69, 178,  69, 162,  85, 146, 101, 146,
    101, 130, 117, 114, 133, 127,   3,  79,   3, 229, 240,  37,
    240,  37, 240,  37, 240,  37, 220, 172,   0,  30,  96,  54,
     21,   0,  13,  50, 161, 132,  99, 141, 140, 155, 170, 183,
    226, 240,  66, 240,  66, 240,  66,  37, 204, 148,  69, 131,
    100, 130, 117, 240,  21, 240,  22, 246,  99, 102,  84, 102,
     70,  86,  70,  86,  69, 101, 100, 101,  98, 117, 131,  69,
    170, 214,   0,  30,  16,  57,  21,   0,  14,  21, 233, 165,
     51, 149,  68, 132,  69, 117,  69, 117,  69, 101,  99, 117,
    240,  21, 246,  36, 150,  23, 119,  53, 103,  69,  86,  85,
     86,  86,  70,  86,  70,  86,  70,  86,  70,  86,  85,  86,
     85,  86,  85,  86, 100,  85, 117,  69, 133,  37, 170, 214,
      0,  30,  61,  21,   0,  13,  34, 240,  79,   1,  95,   1,
     95,  95,   1,  95,  98, 147, 114, 146, 130, 131, 130, 115,
    240,  50, 240,  50, 240,  51, 240,  51, 240,  36, 240,  35,
    240,  51, 240,  36, 240,  36, 240,  36, 240,  36, 240,  21,
    240,  21, 240,  21, 240,  21, 240,  21, 240,  21, 240,  35,
      0,  30,  64,  55,  22,   0,  14,  70, 234, 179,  84, 147,
    116, 131, 132, 115, 132, 100, 132, 101, 116, 102,  99, 120,
     67, 137,  19, 156, 187, 203, 188, 147,  26, 115,  72, 100,
    103,  68, 134,  68, 149,  68, 164,  68, 164,  68, 164,  84,
    147, 100, 132, 117,  84, 155, 215,   0,  31,  64,  56,  21,
      0,  13, 103, 217, 165,  52, 148,  69, 117,  85, 101,  85,
     86,  85,  86,  86,  70,  86,  70,  86,  70,  86,  70,  86,
     85,  86,  85,  86,  85,  71, 101,  55, 126, 149,  22, 246,
    245, 240,  21, 116,  85, 101,  84, 118,  53, 117,  68, 147,
     68, 185, 214,   0,  30,  32,  23,  11,   0,  19,  67, 117,
     87,  71,  71,  85, 115,   0,   7, 115, 117,  87,  71,  71,
     85, 115,   0,  15, 112,  31,  11,   0,  19,  67, 117,  87,
     71,  71,  85, 115,   0,   7, 115, 117,  87,  71,  71,  86,
    101, 146, 146, 145, 146, 145, 146, 130, 145, 240, 240, 176,
     36,  11,   0,  15,  51, 117,  87,  71,  71,  85, 115, 240,
    240, 179, 131, 131, 131, 131, 131, 131, 117, 101, 101, 101,
    101, 101, 101,  87,  71,  71,  71,  71,  85, 115, 240, 208,
     17,  32,   0,  68,  15,  13,  79,  13,   0,  28,  79,  13,
     79,  13,   0,  64,  64,  49,  19,   0,  26, 100, 230, 214,
    199, 214, 214, 228,   0,   9,  18, 240,  34, 240,  34, 240,
     34, 240,  34, 240,  34, 240,  19, 240,  18, 240,  19, 243,
    244, 228, 244, 229,  99,  85,  85,  69,  85,  69, 100,  84,
     99, 108, 152,   0,   8,  16,  48,  19,   0,   7,  87, 171,
    115,  85,  84, 101,  69,  85,  69,  85,  69,  85,  83, 101,
    213, 228, 228, 228, 243, 243, 240,  19, 240,  18, 240,  34,
    240,  34, 240,  34, 240,  34,   0,   9,  20, 230, 214, 215,
    198, 214, 228,   0,  27,  48, 104,  32,   0,  13,  55, 240,
    125, 240,  37, 117, 227, 196, 195, 243, 162, 117,  98, 147,
     88,  98, 130,  84,  51,  83,  98,  84,  98,  82,  98,  69,
    101,  34,  98,  69, 101,  35,  66,  69, 117,  50,  66,  69,
    117,  50,  66,  69, 117,  50,  66,  69, 117,  50,  66,  69,
    117,  50,  66,  69, 117,  50,  66,  69, 117,  50,  66,  69,
    117,  50,  82,  69, 101,  50,  82,  69, 101,  50,  82,  84,
    101,  49, 114,  84,  55,  34, 115,  88,  23, 146, 117,  68,
    179, 240, 243, 240,  20, 165, 151, 223,   1, 240,  59,   0,
     45,  48,  76,  33,   0,  18,  34, 240, 244, 240, 228, 240,
    213, 240, 214, 240, 198, 240, 183, 240, 184, 240, 168, 240,
    153, 240, 146,  38, 240, 130,  38, 240, 114,  55, 240,  98,
     70, 240,  98,  70, 240,  82, 102, 240,  66, 102, 240,  51,
    102, 240,  50, 134, 240,  47,   1, 240,  31,   2, 240,  18,
    166, 242, 166, 227, 166, 226, 198, 210, 198, 195, 199, 122,
    109,  74, 109,   0,  45, 112,  65,  30,   0,  15,  15,   5,
    175,   7, 198, 118, 182, 134, 166, 150, 150, 150, 150, 150,
    150, 150, 150, 150, 150, 149, 166, 134, 166, 117, 207,   1,
    239,   2, 214, 133, 182, 149, 166, 150, 150, 166, 134, 166,
    134, 166, 134, 166, 134, 166, 134, 166, 134, 166, 134, 150,
    150, 150, 150, 134, 111,   8, 127,   6,   0,  42,  48,  70,
     29,   0,  15, 104,  82, 204,  35, 166, 103, 149, 150, 133,
    181, 117, 212, 101, 243, 101, 243,  86, 243,  85, 240,  34,
     70, 240,  34,  70, 240, 134, 240, 134, 240, 134, 240, 134,
    240, 134, 240, 134, 240, 134, 240,  34,  85, 240,  34,  86,
    240,  18,  86, 240,  18, 101, 242, 133, 226, 134, 194, 166,
    163, 198,  99, 253, 240,  72,   0,  41,  32,  64,  33,   0,
     16,  79,   5, 223,   7, 246, 119, 214, 165, 198, 181, 182,
    182, 166, 197, 166, 198, 150, 213, 150, 213, 150, 214, 134,
    214, 134, 214, 134, 214, 134, 214, 134, 214, 134, 214, 134,
    214, 134, 214, 134, 213, 150, 213, 150, 198, 150, 197, 166,
    197, 166, 181, 182, 165, 198, 134, 159,   7, 191,   5,   0,
     47,  75,  29,   0,  14,  79,   8, 111,   8, 166, 133, 166,
    163, 166, 163, 166, 178, 166, 178, 166, 194, 150,  82,  82,
    150,  82,  82, 150,  82, 240,  22,  82, 240,  22,  67, 240,
     29, 240,  29, 240,  22,  52, 240,  22,  82, 240,  22,  82,
     98, 134,  82,  98, 134,  82,  98, 134, 210, 134, 194, 150,
    194, 150, 194, 150, 179, 150, 164, 150, 134,  95,   8, 111,
      8,   0,  40,  80,  63,  28,   0,  14,  15,   7, 111,   8,
    150, 118, 150, 148, 150, 163, 150, 178, 150, 178, 150, 178,
    150,  82,  66, 150,  82,  82, 134,  82, 246,  82, 246,  67,
    253, 253, 246,  52, 246,  67, 246,  82, 246,  82, 246,  82,
    246, 240, 118, 240, 118, 240, 118, 240, 118, 240, 118, 240,
    118, 240,  63, 223,   0,  40,  16,  71,  33,   0,  17, 119,
     98, 253,  35, 214, 104, 197, 166, 181, 197, 165, 228, 149,
    240,  19, 149, 240,  19, 134, 240,  19, 133, 240,  50, 118,
    240,  50, 118, 240, 198, 240, 198, 240, 198, 240, 198, 240,
    198, 240, 198, 143,  85, 143,  85, 230, 134, 214, 149, 214,
    149, 214, 165, 198, 181, 182, 197, 151, 215,  88, 252,  51,
    240,  55,  98,   0,  46,  32,  65,  35,   0,  17,  78,  62,
     78,  62, 134, 182, 198, 182, 198, 182, 198, 182, 198, 182,
    198, 182, 198, 182, 198, 182, 198, 182, 198, 182, 198, 182,
    207,   8, 207,   8, 198, 182, 198, 182, 198, 182, 198, 182,
    198, 182, 198, 182, 198, 182, 198, 182, 198, 182, 198, 182,
    198, 182, 198, 182, 142,  62,  78,  62,   0,  48,  80,  36,
     18,   0,   9,  14,  78, 134, 198, 198, 198, 198, 198, 198,
    198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
    198, 198, 198, 198, 198, 198, 142,  78,   0,  25,  32,  60,
     24,   0,  12,  95, 159, 246, 240,  54, 240,  54, 240,  54,
    240,  54, 240,  54, 240,  54, 240,  54, 240,  54, 240,  54,
    240,  54, 240,  54, 240,  54, 240,  54, 240,  54, 240,  54,
    240,  54, 240,  54, 240,  54, 132, 102, 118,  86, 118,  86,
    118,  86, 117,  86, 147,  86, 188, 231,   0,  34,  80,  79,
     34,   0,  17,  14, 105,  94, 105, 150, 196, 198, 194, 230,
    178, 246, 162, 240,  22, 146, 240,  38, 130, 240,  54, 114,
    240,  70,  98, 240,  86,  82, 240, 102,  67, 240, 102,  38,
    240,  86,  23, 240,  95, 240,  72,  22, 240,  71,  54, 240,
     54,  71, 240,  38,  86, 240,  38, 102, 240,  22, 103, 246,
    118, 246, 134, 230, 135, 214, 150, 214, 166, 198, 167, 126,
     61,  78,  61,   0,  47,  32,  63,  26,   0,  13,  15, 191,
    246, 240,  86, 240,  86, 240,  86, 240,  86, 240,  86, 240,
     86, 240,  86, 240,  86, 240,  86, 240,  86, 240,  86, 240,
     86, 240,  86, 240,  86, 240,  86, 162, 134, 162, 134, 162,
    134, 162, 134, 147, 134, 146, 150, 146, 150, 131, 150, 116,
    150,  86,  95,   6,  95,   6,   0,  36,  48, 110,  41,   0,
     20,  74, 240,  42,  75, 251, 135, 247, 199, 241,  21, 194,
     21, 210,  21, 194,  21, 210,  21, 194,  22, 193,  37, 194,
     37, 178,  37, 194,  37, 177,  53, 194,  53, 146,  53, 194,
     53, 146,  53, 194,  53, 145,  69, 194,  69, 114,  69, 194,
     69, 114,  69, 194,  70,  82,  85, 194,  85,  82,  85, 194,
     85,  81, 101, 194, 101,  50, 101, 194, 101,  50, 101, 194,
    102,  33, 117, 194, 117,  18, 117, 194, 117,  18, 117, 194,
    134, 133, 194, 134, 133, 194, 133, 149, 194, 148, 149, 194,
    148, 149, 138,  83, 109,  74,  98, 109,   0,  56, 112,  81,
     34,   0,  17,  10, 170,  75, 154, 136, 194, 200, 194, 201,
    178, 194,  23, 162, 194,  38, 162, 194,  39, 146, 194,  55,
    130, 194,  71, 114, 194,  86, 114, 194,  87,  98, 194, 103,
     82, 194, 119,  66, 194, 134,  66, 194, 135,  50, 194, 151,
     34, 194, 167,  18, 194, 182,  18, 194, 185, 194, 200, 194,
    215, 194, 215, 194, 230, 194, 245, 194, 240,  20, 194, 240,
     20, 138, 211, 138, 226,   0,  47,  96,  65,  31,   0,  16,
    104, 240,  93, 240,  22,  86, 213, 149, 181, 181, 165, 182,
    133, 213, 118, 214, 101, 245, 101, 245,  86, 246,  70, 246,
     70, 246,  70, 246,  70, 246,  70, 246,  70, 246,  70, 246,
     70, 246,  85, 245, 102, 229, 102, 214, 117, 213, 134, 182,
    149, 181, 181, 149, 214,  86, 240,  29, 240,  88,   0,  44,
     48,  62,  29,   0,  14,  79,   4, 175,   6, 198, 118, 166,
    134, 150, 149, 150, 150, 134, 150, 134, 150, 134, 150, 134,
    150, 134, 150, 134, 149, 150, 134, 150, 118, 175,   2, 207,
    230, 240, 134, 240, 134, 240, 134, 240, 134, 240, 134, 240,
    134, 240, 134, 240, 134, 240, 134, 240, 134, 240,  78, 254,
      0,  41,  96,  88,  31,   0,  16, 104, 240,  93, 240,  37,
     86, 214, 133, 182, 165, 165, 197, 133, 213, 133, 214, 102,
    229, 101, 245,  86, 246,  70, 246,  70, 246,  70, 246,  70,
    246,  70, 246,  70, 246,  70, 246,  70, 246,  85, 245, 101,
    245, 102, 214, 117,  84,  69, 149,  54,  52, 165,  35,  50,
     20, 197,  18,  85, 232,  68, 240,  45,  97, 235,  81, 240,
    100,  66, 240, 121, 240, 121, 240, 121, 240, 120, 240, 151,
    240, 150, 240, 180,   0,  12,  64,  75,  34,   0,  17,  15,
      3, 240,  31,   6, 240,  38, 118, 246, 134, 230, 149, 230,
    150, 214, 150, 214, 150, 214, 150, 214, 150, 214, 149, 230,
    134, 230, 117, 240,  31,   2, 240,  47,   1, 240,  54,  86,
    240,  38, 102, 240,  22, 118, 246, 118, 246, 118, 246, 118,
    246, 118, 246, 119, 230, 119, 230, 119,  66, 134, 134,  66,
    134, 135,  35,  78,  90,  94, 134,   0,  47,  64,  59,  24,
      0,  12, 102,  82, 154,  35, 117,  87, 101, 133, 100, 164,
     85, 164,  85, 179,  85, 179,  86, 163,  86, 163,  89, 240,
     28, 207, 175, 175, 175, 204, 240,  25, 240,  39,  66, 198,
     66, 213,  67, 197,  67, 197,  67, 196,  84, 180,  85, 148,
    104,  85,  99,  43, 130,  87,   0,  34,  32,  74,  30,   0,
     15,  31,   9, 111,   9, 100,  86,  84,  99, 102,  99,  83,
    118, 115,  67, 118, 115,  66, 134, 130,  66, 134, 130,  66,
    134, 130,  66, 134, 130,  66, 134, 130, 230, 240, 150, 240,
    150, 240, 150, 240, 150, 240, 150, 240, 150, 240, 150, 240,
    150, 240, 150, 240, 150, 240, 150, 240, 150, 240, 150, 240,
    150, 240, 150, 240,  63,   3, 207,   3,   0,  42,  32,  66,
     34,   0,  17,  14, 106,  78, 106, 134, 226, 198, 226, 198,
    226, 198, 226, 198, 226, 198, 226, 198, 226, 198, 226, 198,
    226, 198, 226, 198, 226, 198, 226, 198, 226, 198, 226, 198,
    226, 198, 226, 198, 226, 198, 226, 198, 226, 198, 226, 213,
    211, 214, 194, 245, 179, 246, 147, 240,  53, 100, 240,  93,
    240, 151,   0,  48,  80,  77,  34,   0,  17,  13, 137,  77,
    137, 134, 226, 198, 211, 198, 210, 230, 194, 230, 179, 231,
    162, 240,  22, 162, 240,  22, 147, 240,  38, 130, 240,  54,
    115, 240,  55,  98, 240,  86,  98, 240,  86,  83, 240,  87,
     66, 240, 118,  51, 240, 118,  50, 240, 150,  34, 240, 150,
     19, 240, 153, 240, 184, 240, 184, 240, 198, 240, 214, 240,
    213, 240, 244, 240, 244, 240, 240,  18,   0,  49, 110,  45,
      0,  22,  77,  44,  89,  77,  44,  89, 118, 150, 194, 182,
    134, 194, 182, 150, 162, 198, 150, 162, 214, 134, 162, 214,
    150, 130, 230, 135, 130, 246, 119, 130, 246, 120,  98, 240,
     22,  98,  22,  98, 240,  38,  82,  23,  82, 240,  38,  67,
     38,  66, 240,  54,  66,  54,  66, 240,  70,  50,  55,  50,
    240,  70,  35,  70,  34, 240,  86,  34,  86,  34, 240, 102,
     18,  87,  18, 240, 105, 104, 240, 120, 120, 240, 135, 120,
    240, 135, 134, 240, 150, 150, 240, 165, 150, 240, 164, 180,
    240, 180, 180, 240, 195, 195, 240, 194, 210,   0,  63, 112,
     81,  34,   0,  17,  29,  59, 125,  59, 182, 147, 240,  23,
    130, 240,  54, 115, 240,  55,  83, 240,  87,  66, 240, 118,
     51, 240, 119,  34, 240, 150,  19, 240, 153, 240, 183, 240,
    214, 240, 215, 240, 214, 240, 215, 240, 185, 240, 162,  22,
    240, 147,  23, 240, 115,  54, 240, 114,  86, 240,  83,  87,
    240,  66, 118, 240,  51, 119, 240,  19, 150, 240,  18, 182,
    227, 183, 139,  94,  75,  94,   0,  47,  32,  72,  34,   0,
     17,  13, 137,  77, 137, 134, 227, 183, 195, 214, 194, 231,
    163, 247, 146, 240,  23, 131, 240,  39, 114, 240,  55,  99,
    240,  71,  67, 240, 102,  66, 240, 119,  35, 240, 135,  18,
    240, 154, 240, 168, 240, 184, 240, 198, 240, 214, 240, 214,
    240, 214, 240, 214, 240, 214, 240, 214, 240, 214, 240, 214,
    240, 214, 240, 158, 240,  94,   0,  48,  32,  66,  25,   0,
     12,  95,   5,  95,   5,  86, 118, 100, 135,  99, 150, 115,
    135, 114, 150, 130, 135, 130, 134, 146, 118, 240,  55, 240,
     54, 240,  55, 240,  54, 240,  55, 240,  54, 240,  55, 240,
     54, 114, 150, 130, 135, 130, 134, 146, 119, 146, 118, 147,
    103, 147, 102, 148,  86, 149,  86, 119,  79,   6,  79,   6,
      0,  34, 112,  46,  11, 183,  71,  67, 131, 131, 131, 131,
    131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
    131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
    131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 135,  71,
     64,  34,  21,   0,  11,   2, 130, 130, 130, 130, 130, 130,
    130, 145, 145, 161, 145, 146, 130, 150,  70,  87,  55,  71,
     55,  71,  55,  86,  70,  85,  85, 115, 115,   0,  69,  46,
     11, 183,  71, 131, 131, 131, 131, 131, 131, 131, 131, 131,
    131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
    131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
    131, 131, 131, 131, 131, 131,  71,  71,  64,  14,  14,   0,
      7,  65, 196, 150, 120,  84,  36,  65, 114,   0,  60,  14,
     11,   0,   5,  99, 117,  87,  71,  71,  85, 115,   0,  46,
     16,  21,  11,   0,   6,   2, 130, 130, 130, 145, 161, 146,
    150,  87,  71,  71,  86,  85, 115,   0,  36,  64,  43,  24,
      0,  42,  72, 237, 180,  70, 150,  69, 150,  85, 148, 101,
    162, 117, 240,  69, 234, 181,  53, 149,  85, 133, 101, 117,
    117, 117, 117, 117, 117, 117, 102, 133,  66,  23, 105,  39,
    134,  70,   0,  33,  64,  66,  25,   0,  12,  72, 240,  40,
    240,  85, 240,  85, 240,  85, 240,  85, 240,  85, 240,  85,
    240,  85, 240,  85, 240,  85,  38, 197,  25, 167,  69, 150,
    100, 149, 132, 133, 132, 133, 133, 117, 133, 117, 133, 117,
    133, 117, 133, 117, 133, 117, 133, 117, 132, 133, 132, 134,
    100, 148,  18,  68, 163,  41, 179,  69,   0,  35,  80,  36,
     20,   0,  35, 103, 187, 132,  84, 100,  85,  85,  85,  84,
    101,  69, 129, 101, 245, 245, 245, 245, 245, 240,  20, 146,
     85, 130, 101,  98, 133,  67, 154, 198,   0,  28,  64,  64,
     26,   0,  14,  56, 240,  56, 240, 101, 240, 101, 240, 101,
    240, 101, 240, 101, 240, 101, 240, 101, 240, 101, 214,  37,
    185,  21, 164,  87, 148, 118, 133, 133, 132, 149, 117, 149,
    117, 149, 117, 149, 117, 149, 117, 149, 117, 149, 117, 149,
    132, 149, 133, 133, 148, 118, 149,  90, 137,  24, 165,  56,
      0,  36,  32,  41,  22,   0,  39,  39, 219, 164,  69, 132,
    101, 100, 117, 100, 133,  69, 133,  69, 133,  79,   3,  79,
      3,  69, 240,  37, 240,  37, 240,  52, 178,  85, 147, 100,
    131, 133,  83, 171, 215,   0,  31,  48,  42,  19,   0,   8,
     21, 200, 149,  36, 133,  21, 117,  37, 117,  37, 117,  65,
    149, 229, 229, 229, 202, 154, 181, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 187, 139,   0,
     27,  16,  58,  23,   0,  40, 118,  68, 122,  21, 100,  68,
     35,  84, 100,  19,  84, 101, 132, 101, 132, 101, 132, 101,
    132, 100, 164,  69, 172, 177,  38, 210, 240,  98, 240,  99,
    240,  93, 191, 143,   1, 127,   1,  99, 165,  68, 180,  67,
    196,  68, 180,  83, 164, 101, 117, 126, 201,   0,   9,  96,
     66,  27,   0,  13,  72, 240,  72, 240, 117, 240, 117, 240,
    117, 240, 117, 240, 117, 240, 117, 240, 117, 240, 117, 240,
    117,  69, 213,  41, 181,  18,  53, 183,  85, 166, 101, 166,
    101, 165, 117, 165, 117, 165, 117, 165, 117, 165, 117, 165,
    117, 165, 117, 165, 117, 165, 117, 165, 117, 165, 117, 123,
     27,  75,  27,   0,  37,  80,  35,  14,   0,   7,  52, 149,
    150, 119, 134, 133, 163,   0,   6,  40, 104, 149, 149, 149,
    149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
    106,  74,   0,  19,  96,  48,  18,   0,  10,   4, 214, 199,
    168, 168, 182, 212,   0,   8,  25, 153, 213, 213, 213, 213,
    213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
    213, 213,  83,  85,  69,  69,  69,  69,  69,  53,  84,  53,
    122, 166,   0,   7, 112,  63,  25,   0,  12,  72, 240,  40,
    240,  85, 240,  85, 240,  85, 240,  85, 240,  85, 240,  85,
    240,  85, 240,  85, 240,  85,  72, 133,  72, 133,  99, 181,
     83, 197,  67, 213,  51, 229,  35, 245,  20, 250, 251, 230,
     21, 213,  37, 213,  53, 197,  69, 181,  69, 181,  85, 165,
    101, 107,  25,  75,  25,   0,  34, 112,  36,  15,   0,   7,
     72, 120, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
    165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
    165, 165, 165, 123,  75,   0,  21,  16,  67,  38,   0,  66,
     72,  69, 101, 168,  41,  56, 136,  18,  53,  34,  52, 181,
     17,  87,  69, 166, 102,  85, 166, 101, 101, 165, 117, 101,
    165, 117, 101, 165, 117, 101, 165, 117, 101, 165, 117, 101,
    165, 117, 101, 165, 117, 101, 165, 117, 101, 165, 117, 101,
    165, 117, 101, 165, 117, 101, 123,  27,  26,  75,  27,  26,
      0,  52,  96,  47,  27,   0,  47,  40,  69, 168,  41, 136,
     18,  53, 181,  17,  85, 166, 101, 166, 101, 165, 117, 165,
    117, 165, 117, 165, 117, 165, 117, 165, 117, 165, 117, 165,
    117, 165, 117, 165, 117, 165, 117, 123,  27,  75,  27,   0,
     37,  80,  41,  22,   0,  39,  38, 234, 180,  68, 148, 100,
    116, 132, 100, 132,  85, 133,  69, 133,  69, 133,  69, 133,
     69, 133,  69, 133,  69, 133,  84, 132, 100, 132, 116, 100,
    133,  69, 170, 230,   0,  31,  64,  58,  25,   0,  43, 104,
     38, 152,  25, 122,  69, 150, 101, 133, 117, 133, 132, 133,
    133, 117, 133, 117, 133, 117, 133, 117, 133, 117, 133, 117,
    133, 117, 117, 133, 117, 134, 100, 151,  68, 174, 181,  37,
    213, 240,  85, 240,  85, 240,  85, 240,  85, 240,  85, 240,
     43, 235,   0,  11,  16,  61,  26,   0,  46,  38,  82, 185,
     51, 165,  66,  20, 149, 102, 133, 118, 133, 133, 117, 149,
    117, 149, 117, 149, 117, 149, 117, 149, 117, 149, 117, 149,
    133, 133, 133, 133, 149, 102, 165,  56, 185,  21, 213,  53,
    240, 101, 240, 101, 240, 101, 240, 101, 240, 101, 240, 101,
    240,  59, 251,   0,  10,  32,  33,  20,   0,  35,   8,  36,
    104,  22,  88,  17,  21, 118,  37, 118,  37, 117,  67, 133,
    245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 203, 155,
      0,  28,  80,  36,  18,   0,  32,   9, 114,  84,  98, 130,
     83, 130,  83, 130,  84, 232, 186, 154, 154, 184, 228,  66,
    147,  66, 147,  67, 131,  68, 115,  69,  83,  92,  98,  53,
      0,  25,  96,  42,  18,   0,  14,  34, 240,  18, 240,  18,
    243, 243, 243, 228, 213, 173,  93, 133, 213, 213, 213, 213,
    213, 213, 213, 213, 213,  66, 117,  66, 117,  66, 117,  66,
    117,  66, 133,  34, 168, 181,   0,  25,  80,  45,  27,   0,
     47,  40,  72, 120,  72, 165, 117, 165, 117, 165, 117, 165,
    117, 165, 117, 165, 117, 165, 117, 165, 117, 165, 117, 165,
    117, 165, 117, 165, 117, 165, 102, 165, 102, 181,  74, 153,
     24, 167,  40,   0,  37,  80,  46,  24,   0,  42,  10,  55,
     74,  55, 117, 114, 165, 114, 165, 114, 181,  82, 197,  82,
    213,  51, 213,  50, 229,  50, 245,  18, 240,  21,  18, 240,
     24, 240,  38, 240,  54, 240,  68, 240,  84, 240,  84, 240,
     98,   0,  34,  80,  66,  32,   0,  56,   9,  25,  39,  73,
     25,  39, 101,  84, 130, 149,  84,  98, 165,  84,  98, 180,
     84,  98, 181,  69,  66, 197,  54,  66, 212,  54,  66, 213,
     34,  20,  34, 229,  18,  36,  34, 244,  18,  36,  34, 247,
     54, 240,  22,  70, 240,  37,  70, 240,  37,  84, 240,  52,
    100, 240,  67, 100, 240,  66, 130,   0,  45,  64,  46,  24,
      0,  42,   9,  56,  73,  56, 117,  98, 196,  82, 213,  51,
    229,  19, 240,  23, 240,  38, 240,  69, 240,  85, 240,  69,
    240,  55, 240,  19,  21, 242,  37, 226,  69, 195,  85, 163,
    116, 120,  57,  72,  57,   0,  33,  64,  65,  25,   0,  43,
    106,  56,  74,  56, 117, 130, 165, 114, 197,  98, 197,  83,
    213,  66, 229,  66, 229,  50, 240,  21,  34, 240,  21,  19,
    240,  39, 240,  55, 240,  54, 240,  85, 240,  85, 240,  99,
    240, 115, 240, 115, 240, 114, 240,  18,  67, 229,  50, 245,
     50, 245,  34, 240,  20,  35, 240,  39, 240,  68,   0,  11,
     96,  39,  20,   0,  35,  31,  95,  84,  85,  99,  86,  98,
    101, 114,  85, 130,  70, 130,  69, 229, 230, 229,  66, 133,
     82, 118,  82, 117,  98, 101,  99,  86,  99,  85,  85,  79,
     95,   0,  28,  16,  11,  25,   0,  62,  79,   6,  79,   6,
      0,  69,  32,  12,  46,   0, 115,  15,  15,  12,  79,  15,
     12,   0, 127,  25,  17, 240, 240,  99,  67, 101,  37,  85,
     37,  85,  37,  84,  52,  85,  37,  84,  52,  99,  67, 129,
     97,   0,  71,  16,  15,  18,   0,   9,  52,  82, 107, 108,
     83,  39, 113,  84,   0,  79,  80,
};

//*****************************************************************************
//
// The font definition for the 42 point Cm bold font.
//
//*****************************************************************************
const tFont g_sFontCm42b =
{
    //
    // The format of the font.
    //
    FONT_FMT_PIXEL_RLE,

    //
    // The maximum width of the font.
    //
    41,

    //
    // The height of the font.
    //
    44,

    //
    // The baseline of the font.
    //
    33,

    //
    // The offset to each character in the font.
    //
    {
           0,    5,   41,   76,  186,  262,  377,  466,
         487,  534,  580,  625,  689,  711,  722,  736,
         827,  886,  923,  977, 1034, 1091, 1145, 1202,
        1263, 1318, 1374, 1397, 1428, 1464, 1481, 1530,
        1578, 1682, 1758, 1823, 1893, 1957, 2032, 2095,
        2166, 2231, 2267, 2327, 2406, 2469, 2579, 2660,
        2725, 2787, 2875, 2950, 3009, 3083, 3149, 3226,
        3336, 3417, 3489, 3555, 3601, 3635, 3681, 3695,
        3709, 3730, 3773, 3839, 3875, 3939, 3980, 4022,
        4080, 4146, 4181, 4229, 4292, 4328, 4395, 4442,
        4483, 4541, 4602, 4635, 4671, 4713, 4758, 4804,
        4870, 4916, 4981, 5020, 5031, 5043, 5068,
    },

    //
    // A pointer to the actual font data
    //
    g_pucCm42bData
};
