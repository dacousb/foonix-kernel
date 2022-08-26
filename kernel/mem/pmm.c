#include <lib/mutex.h>
#include <lib/panic.h>
#include <lib/printf.h>
#include <mem/mem.h>
#include <mem/mmap.h>
#include <mem/pmm.h>

static mutex_t pmm_mutex = UNLOCKED;

static u64 highest_addr = 0; // aligned
static bitmap_t bitmap = {0};

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

addr_range_t pmm_alloc(u64 size)
{
    lock(&pmm_mutex);

    assert(IS_ALIGNED(size));

    addr_range_t range = {0};
    for (u64 page = 0; page < bitmap.size * 8; page++)
    {
        if (bitmap.bitmap[page / 8] & (1 << (page % 8)))
        { // used region
            range.base = 0;
            range.size = 0;
        }
        else
        { // free region
            if (range.size == 0)
                range.base = page * PAGE_SIZE;
            range.size += PAGE_SIZE;
        }

        if (range.size == size)
            break;
    }

    assert(range.size >= size);
    set_used_range(range.base, range.size / PAGE_SIZE);

    unlock(&pmm_mutex);
    return range;
}

addr_range_t pmm_alloc_zeroed(u64 size)
{
    addr_range_t range = pmm_alloc(size);
    memset((void *)phys_to_io(range.base), 0, size);

    return range;
}

void pmm_free(addr_range_t range)
{
    lock(&pmm_mutex);

    assert(IS_ALIGNED(range.size));
    set_unused_range(range.base, range.size / PAGE_SIZE);

    unlock(&pmm_mutex);
}

void init_pmm(struct limine_memmap_response *memmap)
{
    assert(memmap->entry_count > 0);

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
            bitmap.bitmap = (u8 *)phys_to_io(entry->base);
            entry->length -= bitmap.size;
            entry->base += bitmap.size;

            break;
        }
    }

    assert(bitmap.bitmap != nil);
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