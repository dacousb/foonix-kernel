#include <arch/x86_64/asm.h>
#include <mem/heap.h>
#include <mem/mem.h>
#include <mem/mmap.h>
#include <mem/vmm.h>
#include <printf.h>

static vmm_entry_t *kernel_pm = nil;

static inline vmm_entry_t *vmm_get_pm(vmm_entry_t *table, u64 index, u64 flags)
{
    if (!(table[index] & VMM_PRESENT))
    {
        addr_range_t range = pmm_alloc_zeroed(PM_SIZE); // must be physical

        table[index] = range.base;
        table[index] |= flags;
    }

    return (vmm_entry_t *)phys_to_io(table[index] & ~((u64)0xfff));
}

static inline void vmm_map_page(vmm_entry_t *pm4, u64 virtual_addr, u64 physical_addr, u64 flags)
{
    u64 pm4_index = (virtual_addr & ((u64)0x1ff << 39)) >> 39;
    u64 pm3_index = (virtual_addr & ((u64)0x1ff << 30)) >> 30;
    u64 pm2_index = (virtual_addr & ((u64)0x1ff << 21)) >> 21;
    u64 pm1_index = (virtual_addr & ((u64)0x1ff << 12)) >> 12;

    vmm_entry_t *pm3 = vmm_get_pm(pm4, pm4_index, flags);
    vmm_entry_t *pm2 = vmm_get_pm(pm3, pm3_index, flags);
    vmm_entry_t *pm1 = vmm_get_pm(pm2, pm2_index, flags);
    pm1[pm1_index] = physical_addr | flags;

    __invlpg__(virtual_addr);
}

void init_vmm(struct limine_kernel_address_response *kaddr, struct limine_memmap_response *memmap)
{
    addr_range_t range = heap_alloc_zeroed(PM_SIZE);
    kernel_pm = (vmm_entry_t *)range.base;

    for (u64 i = 0; i < 0x10000000; i += PAGE_SIZE)
        vmm_map_page(kernel_pm, kaddr->virtual_base + i, kaddr->physical_base + i, VMM_PRESENT | VMM_WRITABLE);
    printf("     .");

    for (u64 i = 0x1000; i < 0x100000000; i += PAGE_SIZE)
    {
        vmm_map_page(kernel_pm, i, i, VMM_PRESENT | VMM_WRITABLE);
        vmm_map_page(kernel_pm, phys_to_io(i), i, VMM_PRESENT | VMM_WRITABLE);
    }
    printf(".");

    for (u64 i = 0; i < memmap->entry_count; i++)
    {
        struct limine_memmap_entry *entry = memmap->entries[i];

        u64 base = entry->base;
        u64 top = entry->base + entry->length;

        if (top <= 0x100000000)
            continue;

        for (u64 j = base; j < top; j += PAGE_SIZE)
        {
            if (j < 0x100000000)
                continue;
            vmm_map_page(kernel_pm, j, j, VMM_PRESENT | VMM_WRITABLE);
            vmm_map_page(kernel_pm, phys_to_io(j), j, VMM_PRESENT | VMM_WRITABLE);
        }
    }
    printf(".\n");

    __cr3__(io_to_phys((u64)kernel_pm));

    printf("[OK] init VMM\n");
}