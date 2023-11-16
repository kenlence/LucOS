#ifndef _LINUX_KTHREAD_H
#define _LINUX_KTHREAD_H

#include "sched.h"

extern struct list_head running_tasks;

struct task_struct *kthread_create_on_node(int (*threadfn)(void *data),
					   void *data,
					   int node,
					   const char namefmt[], ...);

#define kthread_create(threadfn, data, namefmt, arg...) \
	kthread_create_on_node(threadfn, data, -1, namefmt, ##arg)

#define kthread_run(threadfn, data, namefmt, ...)			   \
({									   \
	struct task_struct *__k						   \
		= kthread_create(threadfn, data, namefmt, ## __VA_ARGS__); \
	if (__k != NULL)						   \
		wake_up_process(__k);					   \
	__k;								   \
})

#endif