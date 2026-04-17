/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/types.h>

#include "ext2.h"

static u32
get_log2(u32 n)
{
	u32 target = n / 1024;
	u32 log	   = 0;

	while (target >>= 1)
		log++;

	return log;
}

extern void memset(void *, int, usize);

void
ext2_init_fs(struct ext2_sb *sb, u32 block_size, u32 blocks_per_group,
	u32 inodes_per_group)
{
	memset(&sb, 0, sizeof(sb));

        
}
