#ifndef __LUCOS_SEMAPHORE_H__
#define __LUCOS_SEMAPHORE_H__

#include "list.h"
#include "spinlock.h"

/* Please don't access any members of this structure directly */
struct semaphore {
	spinlock_t lock;
	unsigned int count;
	struct list_head wait_list;
};

#define __SEMAPHORE_INITIALIZER(name, n)				\
{									\
	.lock		= spin_lock_init((name).lock),	\
	.count		= n,						\
	.wait_list	= LIST_HEAD_INIT((name).wait_list),		\
}

#define DEFINE_SEMAPHORE(name)	\
	struct semaphore name = __SEMAPHORE_INITIALIZER(name, 1)

static inline void sema_init(struct semaphore *sem, int val)
{
	*sem = (struct semaphore) __SEMAPHORE_INITIALIZER(*sem, val);
}

void sema_down(struct semaphore *sem);
int sema_try_down(struct semaphore *sem);   // 0: failed, 1: success
int sema_down_timeout(struct semaphore *sem, unsigned long timeout);  // 暂时不能用
void sema_up(struct semaphore *sem);

#endif