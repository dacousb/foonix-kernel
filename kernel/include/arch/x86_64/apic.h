#ifndef __APIC_H__
#define __APIC_H__

#include <arch/x86_64/acpi.h>
#include <types.h>

#define LAPIC_TIMER_IRQ 32

enum lapic_reg
{
    LAPIC_EOI = 0x0b0,
    LAPIC_SPURIOUS = 0x0f0,

    LAPIC_TIMER = 0x320,
    LAPIC_TIMER_INITIAL_COUNT = 0x380,
    LAPIC_TIMER_CURRENT_COUNT = 0x390,
    LAPIC_TIMER_DIVIDE = 0x3e0
};

enum lapic_command
{
    LAPIC_INT_MASKED = 0x10000,
    LAPIC_PERIODIC_TIMER = 0x20000,
};

void lapic_eoi();
void init_apic(acpi_t acpi);

#endif