#include "heap.h"
#include <mem/mmap.h>

addr_range_t heap_alloc(u64 size)
{
    addr_range_t result = pmm_alloc(ALIGN_UP(size));
    result.base = phys_to_io(result.base);

    return result;
}

addr_range_t heap_alloc_zeroed(u64 size)
{
    addr_range_t result = pmm_alloc_zeroed(ALIGN_UP(size));
    result.base = phys_to_io(result.base);

    return result;
}

void heap_free(addr_range_t range)
{
    range.base = io_to_phys(range.base);
    pmm_free(range);
}
