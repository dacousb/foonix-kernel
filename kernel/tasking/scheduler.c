#include "scheduler.h"

#include <arch/x86_64/asm.h>
#include <arch/x86_64/gdt.h>
#include <lib/mutex.h>
#include <lib/printf.h>
#include <mem/heap.h>
#include <mem/mmap.h>

static mutex_t scheduler_mutex = UNLOCKED;
static scheduler_t scheduler = {0};

void schedule(regs_t *regs)
{
    if (!scheduler.init || scheduler.tasks == nil)
        return;

    lock(&scheduler_mutex);

    if (scheduler.current_task != nil)
        scheduler.current_task->frame = *regs;

    if (scheduler.current_task == nil || scheduler.current_task->next == nil)
        scheduler.current_task = scheduler.tasks;
    else
        scheduler.current_task = scheduler.current_task->next;

    *regs = scheduler.current_task->frame;

    if (scheduler.current_task->gs.syscall_kernel_stack != 0)
        __wrmsr__(MSR_KERN_GS, (u64)&scheduler.current_task->gs);

    if (__read_cr3__() != io_to_phys((u64)scheduler.current_task->pm))
        __write_cr3__(io_to_phys((u64)scheduler.current_task->pm));

    unlock(&scheduler_mutex);
}

void set_task_status(i32 pid, i32 status)
{
    lock(&scheduler_mutex);

    task_t *task = scheduler.tasks;
    for (; task != nil; task = task->next)
    {
        if (task->pid == pid)
        {
            task->status = status;
            break;
        }
    }

    unlock(&scheduler_mutex);
}

void remove_task(i32 pid)
{
    lock(&scheduler_mutex);

    task_t *task = scheduler.tasks;
    task_t *prev = nil;

    if (task != nil && task->pid == pid)
    {
        scheduler.tasks = task->next;
        heap_free(task->task_range);
        heap_free(task->stack_range);
        if (task->syscall_stack_range.size != 0)
            heap_free(task->syscall_stack_range);
    }
    else
    {
        for (; task != nil; prev = task, task = task->next)
        {
            if (task->pid == pid)
            {
                prev->next = task->next;
                heap_free(task->task_range);
                heap_free(task->stack_range);
                if (task->syscall_stack_range.size != 0)
                    heap_free(task->syscall_stack_range);
                break;
            }
        }
    }

    unlock(&scheduler_mutex);
}

void append_task(task_t *task)
{
    lock(&scheduler_mutex);

    if (scheduler.tasks == nil)
        scheduler.tasks = task;
    else
    {
        task_t *last = scheduler.tasks;
        for (; last->next != nil; last = last->next)
            ;
        last->next = task;
    }

    unlock(&scheduler_mutex);
}

void init_scheduler()
{
    addr_range_t int_stack_range = heap_alloc_zeroed(STACK_SIZE);
    get_tss()->rsp[0] = int_stack_range.base + int_stack_range.size;

    scheduler.init = 1;
    printf("[OK] init scheduler\n");
}
