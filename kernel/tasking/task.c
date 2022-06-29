#include <mem/heap.h>
#include <tasking/scheduler.h>
#include <tasking/task.h>

static i32 pid_count = 0;

i32 new_kernel_task(u64 entry)
{
    addr_range_t task_range = heap_alloc_zeroed(sizeof(task_t));
    addr_range_t stack_range = heap_alloc_zeroed(STACK_SIZE);

    task_t *task = (task_t *)task_range.base;

    task->pid = pid_count++;
    task->status = TASK_IDLE;

    task->frame.frame_registers.rip = entry;
    task->frame.frame_registers.rsp = stack_range.base + STACK_SIZE;
    task->frame.frame_registers.cs = 1 * 8;     // kernel code
    task->frame.frame_registers.ss = 2 * 8;     // kernel data
    task->frame.frame_registers.rflags = 0x202; // enable interrupts
    task->pm = get_kernel_pm();

    task->task_range = task_range;
    task->stack_range = stack_range;

    task->next = nil;

    append_task(task);
    return task->pid;
}