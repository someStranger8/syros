/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

#include <stdbool.h>
#include <stdint.h>

/* Integer types */
typedef uint8_t	 u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t	i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

/*
 * usize is for addresses
 * isize is for offsets that may be negative */
typedef __SIZE_TYPE__	 usize;
typedef __PTRDIFF_TYPE__ isize;

#endif
