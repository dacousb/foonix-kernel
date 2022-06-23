#include <mem/mem.h>
#include <mem/pmm.h>
#include <panic.h>
#include <printf.h>

u64 highest_addr = 0; // aligned
bitmap_t bitmap = {0};

static inline void set_used(u64 addr)
{
    u64 page = addr / PAGE_SIZE;
    bitmap.bitmap[page / 8] |= (1 << (page % 8));
}

static inline void set_unused(u64 addr)
{
    u64 page = addr / PAGE_SIZE;
    bitmap.bitmap[page / 8] &= ~(1 << (page % 8));
}

static inline void set_used_range(u64 addr, u64 n)
{
    for (u64 i = addr; i < addr + n * PAGE_SIZE; i += PAGE_SIZE)
        set_used(i);
}

static inline void set_unused_range(u64 addr, u64 n)
{
    for (u64 i = addr; i < addr + n * PAGE_SIZE; i += PAGE_SIZE)
        set_unused(i);
}

static inline int is_used(u64 addr)
{
    u64 page = addr / PAGE_SIZE;
    return bitmap.bitmap[page / 8] & (1 << (page % 8));
}

void init_pmm(struct limine_memmap_response *memmap)
{
    for (u64 i = 0; i < memmap->entry_count; i++)
    {
        struct limine_memmap_entry *entry = memmap->entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;

        if (entry->base + entry->length > highest_addr)
            highest_addr = entry->base + entry->length;
    }

    bitmap.size = ALIGN_UP(ALIGN_DOWN(highest_addr) / PAGE_SIZE / 8); // each bit is a page
    printf("     bm size %ld\n", bitmap.size);

    for (u64 i = 0; i < memmap->entry_count; i++)
    {
        struct limine_memmap_entry *entry = memmap->entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;

        if (entry->length >= bitmap.size)
        {
            bitmap.bitmap = (u8 *)entry->base + MEM_IO_BASE;
            entry->length -= bitmap.size;
            entry->base += bitmap.size;

            break;
        }
    }
    if (bitmap.bitmap == nil)
        panic("bitmap allocation failed");

    memset(bitmap.bitmap, 0xff, bitmap.size); // set all pages to `used`

    u64 available_bytes = 0;
    for (u64 i = 0; i < memmap->entry_count; i++)
    {
        struct limine_memmap_entry *entry = memmap->entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;

        set_unused_range(entry->base, entry->length / PAGE_SIZE); // set the usable ones to `free`
        available_bytes += entry->length;
    }
    printf("     %ld mb free\n", available_bytes / 1024 / 1024);

    printf("[OK] init PMM\n");
}