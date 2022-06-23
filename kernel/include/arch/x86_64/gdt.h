#ifndef __GDT_H__
#define __GDT_H__

#include <types.h>

typedef struct
{
    u16 limit;
    u64 base;
} __attribute__((packed)) gdtr_t;

typedef struct
{
    u16 limit_low;
    u16 base_low;

    u8 base_mid;
    u8 A : 1; // read/write
    u8 W : 1;
    u8 DC : 1;
    u8 E : 1; // executable
    u8 S : 1;
    u8 DPL : 2; // ring
    u8 P : 1;   // present

    u8 limit : 4;
    u8 reserved : 1;
    u8 L : 1;
    u8 DB : 1;
    u8 G : 1;
    u8 base;
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
    gdt_entry_t entries[5];
} __attribute__((packed)) gdt_t;

void init_gdt();
extern void reload_gdt(gdtr_t *gdtr); // gdt.asm

#endif