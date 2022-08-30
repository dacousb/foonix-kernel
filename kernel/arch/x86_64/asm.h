#ifndef __ASM_H__
#define __ASM_H__

#include <lib/types.h>

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

static inline void __pause__()
{
    __asm__ volatile("pause");
}

static inline void __invlpg__(u64 virtual_addr)
{
    __asm__ volatile("invlpg (%0)"
                     :
                     : "r"(virtual_addr));
}

static inline void __write_cr3__(u64 value)
{
    __asm__ volatile("mov %0, %%cr3"
                     :
                     : "a"(value));
}

static inline u64 __read_cr3__()
{
    u64 cr3;
    __asm__ volatile("mov %%cr3, %0"
                     : "=r"(cr3));
    return cr3;
}

static inline void __wrmsr__(u32 msr, u64 value)
{
#define MSR_EFER 0xc0000080
#define MSR_STAR 0xc0000081
#define MSR_LSTAR 0xc0000082
#define MSR_GS 0xc0000101
#define MSR_KERN_GS 0xc0000102

    u32 low = value;
    u32 high = value >> 32;
    __asm__ volatile("wrmsr"
                     :
                     : "c"(msr), "a"(low), "d"(high));
}

static inline u64 __rdmsr__(u32 msr)
{
    u32 low, high;
    __asm__ volatile("rdmsr"
                     : "=a"(low), "=d"(high)
                     : "c"(msr));
    return ((u64)high << 32) | low;
}

#endif
