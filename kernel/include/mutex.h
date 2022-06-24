#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <arch/x86_64/asm.h>

#define UNLOCKED 0
#define LOCKED 1

typedef volatile int mutex_t;

static inline void lock(mutex_t *mutex)
{
    while (!__sync_bool_compare_and_swap(mutex, UNLOCKED, LOCKED))
        __pause__();
}

static inline void unlock(mutex_t *mutex)
{
    *mutex = UNLOCKED;
}

#endif