/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/disk.h>
#include <sys/serial.h>

#include <flanterm.h>
#include <flanterm_backends/fb.h>
#include <limine.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define LIMINE_REQUEST __attribute__((used, section(".limine_requests")))
#define LIMINE_REQUESTS_START \
	__attribute__((used, section(".limine_requests_start")))
#define LIMINE_REQUESTS_END \
	__attribute__((used, section(".limine_requests_end")))

LIMINE_REQUEST static volatile uint64_t limine_base_revision[] =
	LIMINE_BASE_REVISION(6);

LIMINE_REQUEST static volatile struct limine_framebuffer_request
	framebuffer_request = { .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
		.revision	    = 0 };

LIMINE_REQUESTS_START static volatile uint64_t limine_requests_start_marker[] =
	LIMINE_REQUESTS_START_MARKER;

LIMINE_REQUESTS_END static volatile uint64_t limine_requests_end_marker[] =
	LIMINE_REQUESTS_END_MARKER;

static void
hcf(void)
{
	for (;;)
		asm("hlt");
}

void
kmain(void)
{
	if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
	{
		hcf();
	}

	if (framebuffer_request.response == NULL ||
		framebuffer_request.response->framebuffer_count < 1)
	{
		hcf();
	}

	struct limine_framebuffer *framebuffer =
		framebuffer_request.response->framebuffers[0];

	struct flanterm_context *ft_ctx = flanterm_fb_init(NULL, NULL,
		framebuffer->address, framebuffer->width, framebuffer->height,
		framebuffer->pitch, framebuffer->red_mask_size,
		framebuffer->red_mask_shift, framebuffer->green_mask_size,
		framebuffer->green_mask_shift, framebuffer->blue_mask_size,
		framebuffer->blue_mask_shift, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, 0, 0, 1, 0, 0, 0, 0);

#define FLANTERM_WRITE(msg) flanterm_write(ft_ctx, msg, sizeof(msg) - 1)

	FLANTERM_WRITE("Hello World!\r\n");

	serial_init();
	serial_puts("Testing serial output\r\n");

	u16 write_buf[SECTOR_SIZE_WORDS];
	u16 read_buf[SECTOR_SIZE_WORDS];
	u32 target_lba = 100;

	memset(write_buf, 0, sizeof(write_buf));
	const char *msg =
		"THIS MESSAGE IS BEING WRITTEN TO AND READ FROM THE DISK\r\n";
	memcpy(write_buf, msg, strlen(msg));

	write_sectors(target_lba, 1, write_buf);

	memset(read_buf, 0, sizeof(read_buf));

	read_sectors(target_lba, 1, read_buf);

	if (memcmp(write_buf, read_buf, sizeof(read_buf)) == 0)
	{
		FLANTERM_WRITE("SUCCESSFULLY READ FROM AND WROTE TO DISK\r\n");
		FLANTERM_WRITE("MESSAGE FROM DISK: \r\n");
		const char *disk_msg = (const char *)read_buf;
		flanterm_write(ft_ctx, disk_msg, strlen(disk_msg));

	} else
	{
		FLANTERM_WRITE("COULD NOT PROPERLY READ OR WRITE TO DISK\r\n");
	}

	hcf();
}
