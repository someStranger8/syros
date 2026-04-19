/*
 * Copyright (c) 2026 LJC, Gage Atkinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/types.h>
#include <string.h>

// flags for entries
#define PAGE_SIZE 4096
#define PAGE_PRESENT (1ULL << 0)
#define PAGE_WRITE   (1ULL << 1)
#define PAGE_USER    (1ULL << 2)
#define PAGE_ALIGN __attribute__((aligned(4096)))
#define RECURSIVE_SLOT 511UL
#define PML4_VIRT 0xFFFFFFFFFFFFF000ULL

// paging system
typedef u64 pt_entry;

typedef struct {
    pt_entry entries[512];
} page_table;

u64 pml4[512] PAGE_ALIGN;
u64 pdpt[512] PAGE_ALIGN;
u64 pd[512]   PAGE_ALIGN;

u8* bitmap;
u64 max_frames;
u64 bitmap_size;

void setup_identity_paging() {
    // ensure no garbage flags are set
    for(int i = 0; i < 512; i++) {
        pml4[i] = 0;
        pdpt[i] = 0;
        pd[i] = 0;
    }

    // i hate my life
    pml4[0] = (u64)pdpt | PAGE_PRESENT | PAGE_WRITE;
    pdpt[0] = (u64)pd | PAGE_PRESENT | PAGE_WRITE;
    pd[0] = 0x00000000 | PAGE_PRESENT | PAGE_WRITE | PAGE_HUGE;
    pml4[RECURSIVE_SLOT] = ((u64)pml4) | PAGE_PRESENT | PAGE_WRITE;
}

u64* get_pte_address(u64 virt) {
    u64 addr = 0xFFFFFF8000000000;
    addr |= (virt >> 9) & 0x7FFFFFFFF0;
    return (u64*)addr;
}

void map_page_recursive(u64 virt, u64 phys, u64 flags) {
    u64* pte = get_pte_address(virt);
    *pte = phys | flags | PAGE_PRESENT;
    
    asm volatile("invlpg (%0)" :: "r"(virt) : "memory");
}

u64* get_pt_virt(u64 virt) {
    return (u64*)(0xFFFFFF8000000000ULL | ((virt >> 9) & 0x7FFFFFFFF000ULL));
}

u64* get_pd_virt(u64 virt) {
    return (u64*)(0xFFFFFFFFC0000000ULL | ((virt >> 18) & 0x3FFFFF000ULL));
}

u64* get_pdpt_virt(u64 virt) {
    return (u64*)(0xFFFFFFFFFFE00000ULL | ((virt >> 27) & 0x1FF000ULL));
}

void pmm_set_bit(u64 frame) {
    bitmap[frame / 8] |= (1 << (frame % 8));
}

void pmm_clear_bit(u64 frame) {
    bitmap[frame / 8] &= ~(1 << (frame % 8));
}

int pmm_get_bit(u64 frame) {
    return (bitmap[frame / 8] >> (frame % 8)) & 0x1;
}

u64 pmm_alloc() {
    for (u64 i = 0; i < max_frames; i++) {
        if (!pmm_get_bit(i)) {
            pmm_set_bit(i);
            return i * PAGE_SIZE;
        }
    }
    return 0; // uh oh 
}

void pmm_free(u64 phys_addr) {
    u64 frame = phys_addr / PAGE_SIZE;
    pmm_clear_bit(frame);
}

void pmm_init(void* mmap_addr, uint64_t total_memory) {
    max_frames = total_memory / PAGE_SIZE;
    bitmap_size = max_frames / 8;

    memset(bitmap, 0xFF, bitmap_size);
}

void map_page(u64 virt, u64 phys, u64 flags) {
    u64 pml4_idx = (virt >> 39) & 0x1FF;
    u64 pdpt_idx = (virt >> 30) & 0x1FF;
    u64 pd_idx   = (virt >> 21) & 0x1FF;
    u64 pt_idx   = (virt >> 12) & 0x1FF;

    u64* pml4 = (u64*)PML4_VIRT;

    if (!(pml4[pml4_idx] & PAGE_PRESENT)) {
        u64 new_frame = pmm_alloc(); 
        pml4[pml4_idx] = new_frame | PAGE_PRESENT | PAGE_WRITE;
        __builtin_memset(get_pdpt_virt(virt), 0, 4096);
    }

    u64* pdpt = get_pdpt_virt(virt);
    if (!(pdpt[pdpt_idx] & PAGE_PRESENT)) {
        u64 new_frame = pmm_alloc();
        pdpt[pdpt_idx] = new_frame | PAGE_PRESENT | PAGE_WRITE;
        __builtin_memset(get_pd_virt(virt), 0, 4096);
    }

    u64* pd = get_pd_virt(virt);
    if (!(pd[pd_idx] & PAGE_PRESENT)) {
        u64 new_frame = pmm_alloc();
        pd[pd_idx] = new_frame | PAGE_PRESENT | PAGE_WRITE;
        __builtin_memset(get_pt_virt(virt), 0, 4096);
    }

    u64* pt = get_pt_virt(virt);
    pt[pt_idx] = (phys & ~0xFFF) | flags | PAGE_PRESENT;

    asm volatile("invlpg (%0)" :: "r"(virt) : "memory");
}

void *
memcpy(void *restrict dest, const void *restrict src, size_t n)
{
	u8 *restrict pdest = (u8 *restrict)dest;
	const u8 *restrict psrc = (const u8 *restrict)src;

	for (size_t i = 0; i < n; i++)
		pdest[i] = psrc[i];

	return pdest;
}

void *
memset(void *s, int c, size_t n)
{
	u8 *p = (u8 *)s;

	for (size_t i = 0; i < n; i++)
		p[i] = (u8)c;

	return s;
}

void *
memmove(void *dest, const void *src, size_t n)
{
	u8 *pdest = (u8 *)dest;
	const u8 *psrc  = (const u8 *)src;

	if ((uintptr_t)src > (uintptr_t)dest)
	{
		for (size_t i = 0; i < n; i++)
			pdest[i] = psrc[i];
	} else if ((uintptr_t)src < (uintptr_t)dest)
	{
		for (size_t i = n; i > 0; i--)
			pdest[i - 1] = psrc[i - 1];
	}

	return dest;
}

int
memcmp(const void *s1, const void *s2, size_t n)
{
	const u8 *p1 = (const u8 *)s1;
	const u8 *p2 = (const u8 *)s2;

	for (size_t i = 0; i < n; i++)
	{
		if (p1[i] != p2[i])
			return p1[i] < p2[i] ? -1 : 1;
	}

	return 0;
}

size_t
strlen(const char *str)
{
	const char *s;

	for (s = str; *s; ++s)
		;
	return (s - str);
}
