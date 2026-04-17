/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <bits/time.h>
#ifdef __KERNEL__

#include <sys/types.h>

#ifndef _EXT2_H_
#define _EXT2_H_

#define EXT2_MAGIC 0xEF53

#define EXT2_STATE_CLEAN 1
#define EXT2_STATE_ERRORS 2

#define EXT2_ERROP_IGNORE 1
#define EXT2_ERROP_REMOUNT 2
#define EXT2_ERROP_PANIC 3

/*
 * This might not be right but im just picking a number
 * that isnt already listed on the osdev wiki
 */
#define EXT2_SYROS_OSID 5

#define EXT2_USE_COMPRESSION 0x0001
#define EXT2_DIR_CONTAIN_TYPE_FIELD 0x0002
#define EXT2_REPLAY_JOURNAL 0x0004
#define EXT2_USE_JOURNAL 0x0008

#define EXT2_RONLY_SPARSE_DT 0x0001
#define EXT2_QWORD_FILE_SIZE 0x0002
#define EXT2_STORE_DIR_AS_BTREE 0x0004

struct ext2_sb
{
	u32 total_inodes;
	u32 total_blocks;
	u32 total_reserved_blocks;
	u32 total_free_blocks;
	u32 total_free_inodes;
	u32 superblock_n;
	u32 block_shift_kib;
	u32 fragment_shift_kib;
	u32 blocks_per_group;
	u32 fragments_per_group;
	u32 inodes_per_group;
	u32 last_mount_time;
	u32 last_written_time;
	u16 mounts_since_fsck;
	u16 mounts_per_fsck;
	u16 signature;
	u16 fs_state;
	u16 errop;
	u16 min_version;
	u32 last_fsck_time;
	u32 time_between_fsck;
	u32 osid;
	u32 maj_version;
	u16 uid;
	u16 gid;

	u8 req_feature_flags;
	u8 ronly_feature_flags;
};

struct ext2_bgdt
{
	u32 block_bitmap;
	u32 inode_bitmap;
	u32 inode_table;
	u16 free_blocks_count;
	u16 free_inodes_count;
	u16 used_dirs_count;
	u16 pad;
	u32 _reserved;
} __attribute__((packed));

struct ext2_inode
{
	u16 mode;
	u16 uid;
	u32 size_lower;
	u32 last_access_time;
	u32 ctime;
	u32 mtime;
	u32 dtime;
	u16 gid;
	u16 link_count;
	u32 blocks;
	u32 flags;
	u32 _reserved;
	u32 dblock_ptr[15]; /*
			     * 0-11: direct
			     * 12: singly indirect
			     * 13: doubly indirect
			     * 14: triply indirect
			     */
	u32 gen_num;
	u32 file_acl;
	u32 size_high;
	u32 faddr;
	u8  frag;
	u8  fsize;
	u16 pad;
	u16 uid_high;
	u16 gid_high;
	u32 _reserved2;
} __attribute__((packed));

/* File types */
#define EXT2_FT_UNKNOWN 0
#define EXT2_FT_REGULAR 1
#define EXT2_FT_DIR 2
#define EXT2_FT_CDEV 3
#define EXT2_FT_BLKDEV 4
#define EXT2_FT_FIFO 5
#define EXT2_FT_SOCKET 6
#define EXT2_FT_SYMLINK 7

struct ext2_dir_entry
{
	struct ext2_inode inode;
	u16		  size;
	u8		  name_len;
	u8		  type;
	char		  name[];
} __attribute__((packed));

#endif

#endif
