#ifndef __MEM_H__
#define __MEM_H__

#include <lib/types.h>

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

static inline int memcmp(void *s1, void *s2, u64 n)
{
    for (u64 i = 0; i < n; i++)
    {
        int diff = ((char *)s1)[i] - ((char *)s2)[i];
        if (diff)
            return diff;
    }
    return 0;
}

#endif
