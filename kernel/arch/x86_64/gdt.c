#include <arch/x86_64/gdt.h>
#include <printf.h>

static gdtr_t gdtr = {0};
static gdt_t gdt = {0};

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
        .E = 1,
        .S = 1,
        .DPL = 3,
        .P = 1,

        .L = 1,
    }; // user code
    gdt.entries[4] = (gdt_entry_t){
        .A = 0,
        .W = 1,
        .DC = 0,
        .E = 0,
        .S = 1,
        .DPL = 3,
        .P = 1,

        .L = 0,
    }; // user data

    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (u64)&gdt;

    reload_gdt(&gdtr);
    printf("[OK] load GDT\n");
}