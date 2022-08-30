#ifndef __TASK_H__
#define __TASK_H__

#include <arch/x86_64/interrupts.h>
#include <lib/elf.h>
#include <lib/types.h>
#include <mem/pmm.h>
#include <mem/vmm.h>

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
    addr_range_t syscall_stack_range;

    struct
    {
        u64 syscall_kernel_stack;
        u64 syscall_user_stack;
    } __attribute__((packed)) gs;

    task_t *next;
} task_t;

i32 new_kernel_task(u64 entry);
i32 new_user_task(elf_header_t *elf_header);

#endif
