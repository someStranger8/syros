/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#define COM1 0x3F8

void serial_init();
int  is_transmit_empty();
void serial_putc(char a);
void serial_puts(const char *str);

#endif
