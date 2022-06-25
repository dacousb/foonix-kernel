#include <mem/heap.h>

addr_range_t heap_alloc(u64 size)
{
    addr_range_t result = pmm_alloc(ALIGN_UP(size));
    result.base += MEM_IO_BASE;

    return result;
}

void heap_free(addr_range_t range)
{
    range.base -= MEM_IO_BASE;
    pmm_free(range);
}