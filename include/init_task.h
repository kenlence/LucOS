#ifndef _LINUX__INIT_TASK_H
#define _LINUX__INIT_TASK_H

#include "thread_info.h"
#include "list.h"

#define INIT_TASK(tsk)	\
{									\
	.state		= 0,						\
	.stack		= &init_thread_info,				\
	.tasks		= LIST_HEAD_INIT(tsk.tasks),			\
	.running_tasks = LIST_HEAD_INIT(tsk.running_tasks), \
}

#define __init_task_data __attribute__((__section__(".data..init_task")))

#endif