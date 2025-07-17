#include "sched.h"
#include "list.h"
#include "current.h"

int wake_up_process(struct task_struct *tsk)
{
    tsk->state = TASK_RUNNING;
    return 0;
}