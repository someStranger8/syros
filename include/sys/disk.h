/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef __KERNEL__

#ifndef _DISK_H_
#define _DISK_H_

#include <stdint.h>

#define ATA_DATA_PORT 0x1F0
#define ATA_SECTOR_COUNT 0x1F2
#define ATA_LBA_LOW 0x1F3
#define ATA_LBA_MID 0x1F4
#define ATA_LBA_HIGH 0x1F5
#define ATA_LBA 0x1F6
#define ATA_COMMAND_PORT 0x1F7

#define ATA_COMMAND_READ_SECTORS 0x20
#define ATA_COMMAND_WRITE_SECTORS 0x30
#define ATA_COMMAND_FLUSH_CACHE 0xE7

#define ATA_BSY 0x80
#define ATA_DRDY 0x40
#define ATA_DRQ 0x08

#define SECTOR_SIZE_WORDS 256

#include <sys/types.h>

void read_sectors(u32 lba, u32 count, u16 *buf);
void write_sectors(u32 lba, u32 count, u16*buf);

#endif

#endif
