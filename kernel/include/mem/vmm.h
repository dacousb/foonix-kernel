#ifndef __VMM_H__
#define __VMM_H__

#include <limine.h>
#include <types.h>

#define PM_SIZE (sizeof(vmm_entry_t) * 512)
typedef u64 vmm_entry_t;

enum vmm_flag
{
    VMM_PRESENT = (1 << 0),
    VMM_WRITABLE = (1 << 1),
    VMM_USER = (1 << 2)
};

void init_vmm(struct limine_kernel_address_response *kaddr, struct limine_memmap_response *memmap);

#endif