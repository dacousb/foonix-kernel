#ifndef __MEM_H__
#define __MEM_H__

#include <types.h>

static inline void *memset(void *ptr, int v, u64 n)
{
    for (u32 i = 0; i < n; i++)
        *((u8 *)ptr + i) = v;
    return ptr;
}

static inline void memcpy(void *dest, void *src, u64 n)
{
    for (u32 i = 0; i < n; i++)
        *((u8 *)dest + i) = *((u8 *)src + i);
}

#endif