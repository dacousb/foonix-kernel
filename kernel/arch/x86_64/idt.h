#ifndef __IDT_H__
#define __IDT_H__

#include <lib/types.h>

typedef struct
{
    u16 limit;
    u64 base;
} __attribute__((packed)) idtr_t;

enum isr_flag
{
    ISR_PRESENT = (1 << 7),
    ISR_USER = (3 << 5),
    ISR_INTERRUPT_GATE = 0xe,
    ISR_TRAP_GATE = 0xf
};

typedef struct
{
    u16 base_low;
    u16 seg_selector;

    u8 IST : 3;
    u8 reserved_mid : 5;
    u8 attributes; // flags

    u16 base_mid;
    u32 base_high;
    u32 reserved_high;
} __attribute__((packed)) idt_entry_t;

void init_idt();

#endif