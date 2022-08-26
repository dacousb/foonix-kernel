#ifndef __PMM_H__
#define __PMM_H__

#include <lib/types.h>
#include <limine.h>

#define PAGE_SIZE 4096

#define ALIGN_UP(addr) (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define ALIGN_DOWN(addr) ((addr) & ~(PAGE_SIZE - 1))
#define IS_ALIGNED(addr) ((addr) % PAGE_SIZE == 0)

typedef struct
{
    u64 size;
    u8 *bitmap;
} bitmap_t;

typedef struct
{
    u64 base;
    u64 size;
} addr_range_t;

void init_pmm(struct limine_memmap_response *memmap);
addr_range_t pmm_alloc(u64 size);
addr_range_t pmm_alloc_zeroed(u64 size);
void pmm_free(addr_range_t range);

#endif