#include <arch/x86_64/acpi.h>
#include <mem/pmm.h>
#include <printf.h>

acpi_t init_acpi(u64 rsdp)
{
    acpi_t acpi = {0};

    rsdp_t *rsdp_ = (rsdp_t *)rsdp;
    rsdt_t *rsdt = (rsdt_t *)((u64)rsdp_->rsdt + MEM_IO_BASE);

    for (u32 i = 0; i < (rsdt->header.length - sizeof(rsdt->header)) / 4; i++)
    {
        sdt_header_t *header = (sdt_header_t *)((u64)rsdt->sdts[i] + MEM_IO_BASE);

        if (header->signature[0] == 'A' &&
            header->signature[1] == 'P' &&
            header->signature[2] == 'I' &&
            header->signature[3] == 'C')
        {
            acpi.madt = (madt_h *)header;
            acpi.lapic = acpi.madt->lapic;
            printf("     found APIC\n");
        }
    }

    printf("[OK] init ACPI\n");
    return acpi;
}