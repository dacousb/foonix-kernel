#ifndef __ELF_H__
#define __ELF_H__

#include <lib/types.h>
#include <mem/vmm.h>

enum elf_ident_class
{
    ELF_ICLASS_32 = 1,
    ELF_ICLASS_64 = 2,
};

enum elf_ident_encoding
{
    ELF_IENCODING_LE = 1,
    ELF_IENCODING_BE = 2,
};

enum elf_program_type
{
    ELF_PTYPE_NULL = 0,
    ELF_PTYPE_LOAD = 1,
    ELF_PTYPE_DYNAMIC = 2,
    ELF_PTYPE_INTERP = 3,
    ELF_PTYPE_NOTE = 4,
    ELF_PTYPE_PROGRAM = 6,
};

typedef struct
{
    u8 magic_numbers[4];

    u8 class;
    u8 encoding;
    u8 version;

    u8 abi;
    u8 abi_version;

    u8 reserved[7];
} __attribute__((packed)) elf_header_ident_t;

typedef struct
{
    elf_header_ident_t ident;

    u16 type;
    u16 machine;
    u32 version;

    u64 entry;
    u64 programs_offset;
    u64 sections_offset;

    u32 flags;

    u16 header_size;
    u16 program_size;
    u16 programs_count;
    u16 section_size;
    u16 sections_count;

    u16 names_section_index;
} __attribute__((packed)) elf_header_t;

typedef struct
{
    u32 type;
    u32 flags;

    u64 offset;
    u64 virtual_addr;
    u64 phys_addr;

    u64 in_file_size;
    u64 in_mem_size;

    u64 alignment;
} __attribute__((packed)) elf_program_header_t;

typedef struct
{
    u32 name;
    u32 type;
    u64 flags;

    u64 virtual_addr;
    u64 offset;
    u64 in_file_size;

    u32 link;
    u32 info;

    u64 alignment;
    u64 entry_size;
} __attribute__((packed)) elf_section_header_t;

void load_elf(vmm_entry_t *pm, elf_header_t *elf_header);

#endif
