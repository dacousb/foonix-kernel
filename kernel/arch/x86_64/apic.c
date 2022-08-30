#include "apic.h"

#include <arch/x86_64/asm.h>
#include <arch/x86_64/hpet.h>
#include <arch/x86_64/pic.h>
#include <lib/printf.h>
#include <mem/mmap.h>

static u64 lapic_base = 0;

static inline void lapic_write(u32 reg, u32 value)
{
    *((volatile u32 *)(lapic_base + reg)) = value;
}

static inline u32 lapic_read(u32 reg)
{
    return *((volatile u32 *)(lapic_base + reg));
}

void lapic_eoi()
{
    lapic_write(LAPIC_EOI, 0);
}

static void init_lapic_timer()
{
    lapic_write(LAPIC_TIMER_DIVIDE, 3);
    lapic_write(LAPIC_TIMER_INITIAL_COUNT, (-1)); // set lapic timer counter to -1

    hpet_sleep(10);                                                    // sleep 10 ms
    lapic_write(LAPIC_TIMER, LAPIC_INT_MASKED);                        // stop lapic timer
    u32 ticks_per_10ms = (-1) - lapic_read(LAPIC_TIMER_CURRENT_COUNT); // save ticks/10 ms

    lapic_write(LAPIC_TIMER, LAPIC_TIMER_IRQ | LAPIC_PERIODIC_TIMER); // start lapic timer
    lapic_write(LAPIC_TIMER_DIVIDE, 3);                               // use divider 16
    lapic_write(LAPIC_TIMER_INITIAL_COUNT, ticks_per_10ms);
}

void init_apic(acpi_t acpi)
{
    lapic_base = phys_to_io(acpi.madt->lapic);

    disable_pic();
    lapic_write(LAPIC_SPURIOUS, lapic_read(LAPIC_SPURIOUS) | (1 << 8));
    init_lapic_timer();

    printf("[OK] init LAPIC\n");
}
