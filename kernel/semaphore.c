#include "semaphore.h"
#include "sched.h"
#include "current.h"

struct semaphore_waiter {
	struct list_head list;
	struct task_struct *task;
	int up;
};

static void up(struct semaphore *sem)
{
	struct semaphore_waiter *waiter = list_first_entry(&sem->wait_list,
						struct semaphore_waiter, list);
	list_del(&waiter->list);
	waiter->up = true;
	wake_up_process(waiter->task);
}

void sem_up(struct semaphore *sem)
{
    unsigned long flags;

    spin_lock_irqsave(&sem->lock, flags);
    if (list_empty(&sem->wait_list))
        sem->count++;
    else
        up(sem);
    spin_unlock_irqrestore(&sem->lock, flags);
}

static int down_common(struct semaphore *sem, long state, long timeout, unsigned long flags)
{
	struct task_struct *task = current;
	struct semaphore_waiter waiter;

	list_add_tail(&waiter.list, &sem->wait_list);
	waiter.task = task;
	waiter.up = false;

	for (;;) {
		if (timeout <= 0)
			goto timed_out;
		__set_task_state(task, state);
		spin_unlock_irqrestore(&sem->lock, flags);
        schedule();
		spin_lock_irqsave(&sem->lock, flags);
		if (waiter.up)
			return 0;
	}

 timed_out:
	list_del(&waiter.list);
	return -1; // -ETIMEOUT
}

static void down(struct semaphore *sem, unsigned long flags)
{
    down_common(sem, TASK_SLEEP, MAX_SCHEDULE_TIMEOUT, flags);
}

void sem_down(struct semaphore *sem)
{
    unsigned long flags;

    spin_lock_irqsave(&sem->lock, flags);
	if (sem->count > 0)
		sem->count--;
	else
		down(sem, flags);
    spin_unlock_irqrestore(&sem->lock, flags);
}

int sem_try_down(struct semaphore *sem)
{
    unsigned long flags;
    int count = 0;

    spin_lock_irqsave(&sem->lock, flags);
    count = sem->count - 1;
    if (count >= 0)
        sem->count = count;
    spin_unlock_irqrestore(&sem->lock, flags);

    return count >= 0;
}

int sem_down_timeout(struct semaphore *sem, unsigned long timeout)
{
    return -1;
}
