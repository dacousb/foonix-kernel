#ifndef __ACPI_H__
#define __ACPI_H__

#include <types.h>

typedef struct
{
    char signature[4];
    u32 length;
    u8 revision;
    u8 checksum;

    char OEM_id[6];
    char OEM_table_id[8];
    u32 OEM_revision;

    u32 creator_id;
    u32 creator_revision;
} __attribute__((packed)) sdt_header_t;

typedef struct
{
    char signature[8];
    u8 checksum;
    char OEM_id[6];
    u8 revision;
    u32 rsdt;
} __attribute__((packed)) rsdp_t;

typedef struct
{
    sdt_header_t header;
    u32 sdts[];
} __attribute__((packed)) rsdt_t;

typedef struct
{
    u8 entry_type;
    u8 length;
} __attribute__((packed)) madt_record_h;

typedef struct
{
    sdt_header_t header;
    u32 lapic;
    u32 flags;
    madt_record_h records[];
} __attribute__((packed)) madt_h;

typedef struct
{
    madt_h *madt;
    u64 lapic;
} acpi_t;

acpi_t init_acpi(u64 rsdp);

#endif