#ifndef __ASM_H__
#define __ASM_H__

#include <types.h>

static inline u8 in8(u16 port)
{
    u8 data;
    __asm__ volatile("inb %1, %0"
                     : "=a"(data)
                     : "d"(port));
    return data;
}

static inline void out8(u16 port, u8 data)
{
    __asm__ volatile("outb %0, %1"
                     :
                     : "a"(data), "d"(port));
}

static inline u16 in16(u16 port)
{
    u16 data;
    __asm__ volatile("inw %1, %0"
                     : "=a"(data)
                     : "d"(port));
    return data;
}

static inline void out16(u16 port, u16 data)
{
    __asm__ volatile("outw %0, %1"
                     :
                     : "a"(data), "d"(port));
}

static inline u32 in32(u16 port)
{
    u32 data;
    __asm__ volatile("inl %1, %0"
                     : "=a"(data)
                     : "d"(port));
    return data;
}

static inline void out32(u16 port, u32 data)
{
    __asm__ volatile("outl %0, %1"
                     :
                     : "a"(data), "d"(port));
}

static inline void __hlt__()
{
    __asm__ volatile("hlt");
}

static inline void __hlt_for__()
{
    for (;;)
        __hlt__();
}

#endif