#ifndef __HPET_H__
#define __HPET_H__

#include <arch/x86_64/acpi.h>
#include <lib/types.h>

enum hpet_reg
{
    HPET_GENERAL_CAPABILITIES_ID = 0x0,
    HPET_GENERAL_CONFIGURATION = 0x10,
    HPET_GENERAL_INTERRUPT_STATUS = 0x20,
    HPET_MAIN_COUNTER_VALUE = 0xF0,
};

void init_hpet(acpi_t acpi);
void hpet_sleep(u64 ms);

#endif
