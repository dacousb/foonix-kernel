#include <arch/x86_64/apic.h>
#include <arch/x86_64/asm.h>
#include <arch/x86_64/pic.h>
#include <mem/mmap.h>
#include <printf.h>

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
    lapic_write(LAPIC_TIMER_DIVIDE, 3);                 // divider 16
    lapic_write(LAPIC_TIMER_INITIAL_COUNT, 0xffffffff); // -1 when signed

    // TODO HPET
    printf("     waiting...\n");

    lapic_write(LAPIC_TIMER, LAPIC_INT_MASKED);

    u32 tick_in_10ms = 0xffffffff - lapic_read(LAPIC_TIMER_CURRENT_COUNT);

    lapic_write(LAPIC_TIMER, LAPIC_TIMER_IRQ | LAPIC_PERIODIC_TIMER);
    lapic_write(LAPIC_TIMER_DIVIDE, 3);
    lapic_write(LAPIC_TIMER_INITIAL_COUNT, tick_in_10ms);
}

void init_apic(acpi_t acpi)
{
    lapic_base = phys_to_io(acpi.madt->lapic);

    disable_pic();
    lapic_write(LAPIC_SPURIOUS, lapic_read(LAPIC_SPURIOUS) | (1 << 8));
    init_lapic_timer();

    printf("[OK] init LAPIC\n");
}