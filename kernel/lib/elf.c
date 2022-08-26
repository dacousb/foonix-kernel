#include <lib/elf.h>
#include <lib/panic.h>
#include <lib/printf.h>
#include <mem/heap.h>
#include <mem/mem.h>
#include <mem/mmap.h>

void load_elf(vmm_entry_t *pm, elf_header_t *elf_header)
{
    assert(memcmp(elf_header, "\127ELF", 4));
    assert(elf_header->ident.class == ELF_ICLASS_64);
    assert(elf_header->ident.encoding == ELF_IENCODING_LE);

    for (u64 i = 0; i < elf_header->programs_count; i++)
    {
        elf_program_header_t *program_header =
            (elf_program_header_t *)((u64)elf_header + elf_header->programs_offset + i * elf_header->program_size);

        u64 size = ALIGN_UP(program_header->in_mem_size);

        if (program_header->type == ELF_PTYPE_LOAD)
        {
            addr_range_t range = heap_alloc(size);

            memcpy((void *)range.base, (void *)((u64)elf_header + program_header->offset), program_header->in_file_size);

            vmm_map_range(pm, program_header->virtual_addr, io_to_phys(range.base), size,
                          VMM_PRESENT | VMM_WRITABLE | VMM_USER);
        }
    }
}