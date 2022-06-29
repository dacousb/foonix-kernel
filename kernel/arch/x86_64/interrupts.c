#include <arch/x86_64/apic.h>
#include <arch/x86_64/asm.h>
#include <arch/x86_64/interrupts.h>
#include <printf.h>
#include <tasking/scheduler.h>

static const char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"};

static void exception_handler(regs_t *regs)
{
    printf("------------------------------------------------\n");
    printf("EXCEPTION (%s) (err_code=0x%lx)\n",
           exception_messages[regs->frame_registers.int_no], regs->frame_registers.err_code);
    printf("------------------------------------------------\n");

    __hlt_for__();
}

u64 interrupt_handler(regs_t *regs)
{
    if (regs->frame_registers.int_no < 32)
        exception_handler(regs);
    else if (regs->frame_registers.int_no == LAPIC_TIMER_IRQ)
        schedule(regs);

    lapic_eoi();
    return (u64)regs;
}