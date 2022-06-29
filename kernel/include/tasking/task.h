#ifndef __TASK_H__
#define __TASK_H__

#include <arch/x86_64/interrupts.h>
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <types.h>

#define STACK_SIZE 0x20000

enum task_status
{
    TASK_IDLE,
    TASK_ACTIVE
};

typedef struct task task_t;
typedef struct task
{
    i32 pid;
    i32 status;

    regs_t frame;
    vmm_entry_t *pm;

    addr_range_t task_range;
    addr_range_t stack_range;

    task_t *next;
} task_t;

i32 new_kernel_task(u64 entry);

#endif