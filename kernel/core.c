#include "../include/sched.h"

static int
try_to_wake_up(struct task_struct *p)
{
	return 1;
}

int wake_up_process(struct task_struct *tsk)
{
    return try_to_wake_up(tsk);
}