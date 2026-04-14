/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/serial.h>

#include <stdint.h>

static inline uint8_t
inb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void
outb(uint16_t port, uint8_t val)
{
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void
serial_init()
{
	outb(COM1 + 1, 0x00); /* Disable all interupts */
	outb(COM1 + 3, 0x80); /* Enable DLAB */
	outb(COM1 + 0, 0x03); /* Set divisor to 3 */
	outb(COM1 + 1, 0x00); /* hi byte */
	outb(COM1 + 3, 0x03); /* 8 bits no parity one stop bit */
	outb(COM1 + 2, 0xC7); /* Enable FIFO, clear, 14-byte threshold */
	outb(COM1 + 4, 0x0B); /* IRQ enabled RTS/DSR set */
}

int
is_transmit_empty()
{
	return inb(COM1 + 5) & 0x20;
}

void
serial_putc(char a)
{
	while (is_transmit_empty() == 0)
		;
	outb(COM1, a);
}

void
serial_puts(const char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
		serial_putc(str[i]);
}
