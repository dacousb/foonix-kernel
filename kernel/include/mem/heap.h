#ifndef __HEAP_H__
#define __HEAP_H__

#include <mem/pmm.h>

addr_range_t heap_alloc(u64 size);
void heap_free(addr_range_t range);

#endif