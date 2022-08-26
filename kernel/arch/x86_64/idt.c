#include <arch/x86_64/asm.h>
#include <arch/x86_64/idt.h>
#include <lib/printf.h>

extern u64 interrupts[256]; // interrupts.asm

static idtr_t idtr = {0};
static idt_entry_t idt[256] = {0};

void set_isr(u64 n, u64 handler, u8 flags)
{
    idt[n].base_low = handler;
    idt[n].base_mid = handler >> 16;
    idt[n].base_high = handler >> 32;

    idt[n].seg_selector = 8; // kernel code segment selector in GDT
    idt[n].attributes = flags;
}

void init_idt()
{
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (u64)&idt;

    for (i32 i = 0; i < 256; i++)
        set_isr(i, interrupts[i], ISR_PRESENT | ISR_INTERRUPT_GATE);

    __asm__ volatile("lidt %0" ::"m"(idtr));
    __asm__ volatile("sti");

    printf("[OK] load IDT\n");
}