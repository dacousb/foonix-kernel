#include <arch/x86_64/acpi.h>
#include <mem/mem.h>
#include <mem/mmap.h>
#include <panic.h>
#include <printf.h>

acpi_t init_acpi(u64 rsdp)
{
    acpi_t acpi = {0};

    rsdp_t *rsdp_ = (rsdp_t *)rsdp;
    rsdt_t *rsdt = (rsdt_t *)phys_to_io(rsdp_->rsdt);

    for (u32 i = 0; i < (rsdt->header.length - sizeof(rsdt->header)) / 4; i++)
    {
        sdt_header_t *header = (sdt_header_t *)phys_to_io(rsdt->sdts[i]);

        if (!memcmp(header->signature, "APIC", 4))
        {
            acpi.madt = (madt_h *)header;
            printf("     found APIC\n");
        }
        else if (!memcmp(header->signature, "HPET", 4))
        {
            acpi.hpet = (hpet_t *)header;
            printf("     found HPET\n");
        }
    }

    assert(acpi.madt != nil);
    assert(acpi.hpet != nil);

    printf("[OK] init ACPI\n");
    return acpi;
}