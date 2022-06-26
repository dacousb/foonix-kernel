#ifndef __MMAP_H__
#define __MMAP_H__

#include <types.h>

#define MEM_IO_BASE (u64)0xffff800000000000

static inline u64 phys_to_io(u64 addr)
{
    return addr + MEM_IO_BASE;
}

static inline u64 io_to_phys(u64 addr)
{
    return addr - MEM_IO_BASE;
}

#endif