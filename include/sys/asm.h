/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef __KERNEL__

#ifndef _ASM_H_
#define _ASM_H_

#include <stdint.h>
#include <sys/types.h>

inline u8
inb(u16 port)
{
	u8 ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

inline u16
inw(u16 port)
{
	u16 ret;
	asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

inline void
outb(u16 port, u8 val)
{
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

inline void
outw(u16 port, u16 val)
{
	asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

#endif

#endif
