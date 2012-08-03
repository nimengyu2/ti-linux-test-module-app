/**
 * @file data_types.h
 * @brief define data types
 *
 * Copyright (C) 2006-2011, toby
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author toby.zhang <toby.chs@gmail.com>
 * @date 2011-9-22 15:32:59
 * @version 0.01
 *
 * This file is pary of toby's open source.
 */

#ifndef __DATA_TYPES_H__
#define __DATA_TYPES_H__
/**
 * @addtogroup api_data_types
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

// TODO: Add your code here

/**
 * Base data types
 */
typedef unsigned char u8_t;   //!< 8 bits unsigned integer data type
typedef signed char s8_t;     //!< 8 bits signed integer data type
typedef unsigned short u16_t; //!< 16 bits unsigned integer data type
typedef signed short s16_t;   //!< 16 bits signed integer data type
typedef unsigned long u32_t;  //!< 32 bits unsigned integer data type
typedef signed long s32_t;    //!< 32 bits signed integer data type
typedef float f32_t;          //!< 32 bits float data type
typedef double f64_t;         //!< 64 double precision floating point data type

/**
 * bool types
 */
typedef u8_t bool_t;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifdef __cplusplus
}
#endif

/**
 * Close the Doxygen group.
 * @}
 */
#endif // __DATA_TYPES_H__

