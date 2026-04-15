/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef __KERNEL__

#ifndef _ASM_H_
#define _ASM_H_

#include <stdint.h>

inline uint8_t
inb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

inline uint16_t
inw(uint16_t port)
{
	uint16_t ret;
	asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

inline void
outb(uint16_t port, uint8_t val)
{
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

inline void
outw(uint16_t port, uint16_t val)
{
	asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

#endif

#endif
