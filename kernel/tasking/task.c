#include "task.h"

#include <mem/heap.h>
#include <mem/mmap.h>
#include <tasking/scheduler.h>

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

i32 new_user_task(elf_header_t *elf_header)
{
    addr_range_t task_range = heap_alloc_zeroed(sizeof(task_t));
    addr_range_t stack_range = heap_alloc_zeroed(STACK_SIZE);
    addr_range_t syscall_stack_range = heap_alloc_zeroed(STACK_SIZE);

    task_t *task = (task_t *)task_range.base;

    task->pid = pid_count++;
    task->status = TASK_IDLE;

    task->frame.frame_registers.rip = elf_header->entry;
    task->frame.frame_registers.rsp = io_to_phys(stack_range.base) + STACK_SIZE;
    task->frame.frame_registers.ss = 3 * 8 | 3; // user data
    task->frame.frame_registers.cs = 4 * 8 | 3; // user code
    task->frame.frame_registers.rflags = 0x202; // enable interrupts
    task->pm = new_pm();

    vmm_map_range(task->pm, io_to_phys(stack_range.base), io_to_phys(stack_range.base),
                  STACK_SIZE, VMM_PRESENT | VMM_WRITABLE | VMM_USER);

    load_elf(task->pm, elf_header);

    task->task_range = task_range;
    task->stack_range = stack_range;
    task->syscall_stack_range = syscall_stack_range;

    task->gs.syscall_kernel_stack = syscall_stack_range.base + syscall_stack_range.size;

    task->next = nil;

    append_task(task);
    return task->pid;
}
