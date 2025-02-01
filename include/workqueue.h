#ifndef __LUCOS_WORK_QUEUE_H
#define __LUCOS_WORK_QUEUE_H

#include "list.h"
#include "mutex.h"
#include "sched.h"
#include "semaphore.h"
#include "spinlock.h"

struct work;
typedef void (*work_func_t)(struct work *work);

// 每一条工作队列都有一个线程
struct workqueue {
    struct list_head works;
    struct task_struct *thread;
    struct semaphore sem;
    spinlock_t lock;
};

struct work {
    struct list_head list;
    work_func_t func;
    void *data;
};

#define __WORK_INITIALIZER(n, f) {					\
	.func = (f),							\
    .list = LIST_HEAD_INIT((n).list),				\
	}

#define DEFINE_WORK(n, f)						\
	struct work n = __WORK_INITIALIZER(n, f)

struct workqueue *workqueue_alloc();
int queue_work(struct workqueue *wq, struct work *work);

#endif