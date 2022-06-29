#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <tasking/task.h>

typedef struct
{
    i32 init;
    task_t *current_task;
    task_t *tasks;
} scheduler_t;

void schedule(regs_t *regs);
void remove_task(i32 pid);
void append_task(task_t *task);
void set_task_status(i32 pid, i32 status);
void init_scheduler();

#endif