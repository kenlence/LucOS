#include "sched.h"
#include "stddef.h"
#include "list.h"
#include "current.h"

//在这里，遍历可运行的链表，然后选取一个任务来切换
//如果没有任何可运行的任务，则切换到init_task
static inline struct task_struct *pick_next_task(struct task_struct *prev)
{
	struct task_struct *tsk;

	tsk = container_of(prev->running_tasks.next, struct task_struct, running_tasks);
    return tsk;
}

extern struct task_struct *__switch_to(struct task_struct *, struct thread_info *, struct thread_info *);

#define switch_to(prev,next,last)					\
do {									\
	last = __switch_to(prev, task_thread_info(prev), task_thread_info(next));	\
} while (0)

static void context_switch(struct task_struct *prev, struct task_struct *next)
{
	/* Here we just switch the register state and the stack. */
	switch_to(prev, next, prev);
    __asm__ __volatile__("":::"memory");
}

static void __schedule(void)
{
	struct task_struct *prev, *next;

	//preempt_disable();  禁用抢占
	prev = current;

    //如果是主动发起的调度，线程一般会把自己的状态改为不是running
    //就要把线程移除running队列，放入其他队列
    //如果是线程抢占了，那么保持再running即可，什么都不用做直接往下

	next = pick_next_task(prev);

	if (prev != next) {
		context_switch(prev, next);
	}

	//sched_preempt_enable_no_resched(); 开启抢占
}

void schedule(void)
{
    do {
	    __schedule();
    } while(0);
    //should check need resched here, while(need_resched())
}

