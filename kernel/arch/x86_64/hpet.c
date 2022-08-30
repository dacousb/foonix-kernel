#include "hpet.h"

#include <lib/printf.h>
#include <mem/mmap.h>

u64 hpet_base = 0, hpet_freq = 0;

static inline void hpet_write(u64 reg, u64 value)
{
    *((volatile u64 *)(hpet_base + reg)) = value;
}

static inline u64 hpet_read(u64 reg)
{
    return *((volatile u64 *)(hpet_base + reg));
}

void init_hpet(acpi_t acpi)
{
    hpet_base = phys_to_io(acpi.hpet->address);

    u64 period = hpet_read(HPET_GENERAL_CAPABILITIES_ID) >> 32; // fs
    hpet_freq = 1000000000000000 / period;                      // hz

    printf("     freq %lx hz\n", hpet_freq);

    hpet_write(HPET_GENERAL_CONFIGURATION, 0); // halt main counter
    hpet_write(HPET_MAIN_COUNTER_VALUE, 0);    // main counter to 0
    hpet_write(HPET_GENERAL_CONFIGURATION, 1); // start main counter

    printf("[OK] init HPET\n");
}

void hpet_sleep(u64 ms)
{
    u64 ticks_to_sleep = hpet_read(HPET_MAIN_COUNTER_VALUE) + ms * (hpet_freq / 1000);
    while (hpet_read(HPET_MAIN_COUNTER_VALUE) <= ticks_to_sleep)
        ;
}
