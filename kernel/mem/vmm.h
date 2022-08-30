#ifndef __VMM_H__
#define __VMM_H__

#include <lib/types.h>
#include <limine.h>

#define PM_SIZE (sizeof(vmm_entry_t) * 512)
typedef u64 vmm_entry_t;

enum vmm_flag
{
    VMM_PRESENT = (1 << 0),
    VMM_WRITABLE = (1 << 1),
    VMM_USER = (1 << 2)
};

vmm_entry_t *get_kernel_pm();
vmm_entry_t *new_pm();
void vmm_map_range(vmm_entry_t *pm4, u64 virtual_addr, u64 physical_addr, u64 size, u64 flags);
void init_vmm(struct limine_kernel_address_response *kaddr, struct limine_memmap_response *memmap);

#endif
