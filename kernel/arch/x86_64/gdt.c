#include "gdt.h"
#include <lib/printf.h>

static gdtr_t gdtr = {0};
static gdt_t gdt = {0};
static tss_t tss = {0};

tss_t *get_tss()
{
    return &tss;
}

tss_entry_t new_tss_entry(u64 tss_addr)
{
    return (tss_entry_t){
        .limit_low = sizeof(tss_t),
        .base_low = tss_addr & 0xffff,

        .base_mid = (tss_addr >> 16) & 0xff,
        .type = 9,
        .P = 1,

        .base = (tss_addr >> 24) & 0xff,
        .base_high = tss_addr >> 32,
    };
}

void init_gdt()
{
    gdt.entries[0] = (gdt_entry_t){0}; // null
    gdt.entries[1] = (gdt_entry_t){
        .A = 0,
        .W = 1,
        .DC = 0,
        .E = 1,
        .S = 1,
        .DPL = 0,
        .P = 1,

        .L = 1,
    }; // kernel code
    gdt.entries[2] = (gdt_entry_t){
        .A = 0,
        .W = 1,
        .DC = 0,
        .E = 0,
        .S = 1,
        .DPL = 0,
        .P = 1,

        .L = 0,
    }; // kernel data
    gdt.entries[3] = (gdt_entry_t){
        .A = 0,
        .W = 1,
        .DC = 0,
        .E = 0,
        .S = 1,
        .DPL = 3,
        .P = 1,

        .L = 0,
    }; // user data
    gdt.entries[4] = (gdt_entry_t){
        .A = 0,
        .W = 1,
        .DC = 0,
        .E = 1,
        .S = 1,
        .DPL = 3,
        .P = 1,

        .L = 1,
    }; // user code
    gdt.tss_entry = new_tss_entry((u64)&tss);

    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (u64)&gdt;

    reload_gdt(&gdtr);
    printf("[OK] load GDT\n");

    reload_tss();
    printf("[OK] load TSS\n");
}
