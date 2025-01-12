#include "init_task.h"
#include "sched.h"

/* Initial task structure */
struct task_struct init_task = INIT_TASK(init_task);

/*
 * Initial thread structure. Alignment of this is handled by a special
 * linker map entry.
 */
union thread_union init_thread_union __init_task_data =
	{ INIT_THREAD_INFO(init_task) };



