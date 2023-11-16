#include "../include/sched.h"
#include "../include/list.h"
#include "../include/current.h"

extern struct list_head running_tasks;

int wake_up_process(struct task_struct *tsk)
{
    if (tsk->state == TASK_RUNNING) {
        return -1;
    }

    tsk->state = TASK_RUNNING;
    list_add(&tsk->running_tasks, &current->running_tasks);
    return 0;
}