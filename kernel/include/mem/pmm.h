#ifndef __PMM_H__
#define __PMM_H__

#include <limine.h>
#include <types.h>

#define MEM_IO_BASE (u64)0xffff800000000000
#define MEM_KERNEL_BASE (u64)0xffffffff80000000

#define PAGE_SIZE 4096

#define ALIGN_UP(addr) (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define ALIGN_DOWN(addr) ((addr) & ~(PAGE_SIZE - 1))

typedef struct
{
    u64 size;
    u8 *bitmap;
} bitmap_t;

void init_pmm(struct limine_memmap_response *memmap);

#endif