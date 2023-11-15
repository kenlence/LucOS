#include "../include/kthread.h"
#include "../include/kernel.h"
#include "../include/slab.h"
#include "../include/gfp.h"
#include "../include/list.h"
#include "../include/sched.h"

//TODO:在这里，把任务放到tasks链表里
static int kthread(int(*threadfn)(void*), void *data)
{
	/* Copy data: it's on kthread's stack */
	struct kthread_create_info *create = _create;
	int (*threadfn)(void *data) = create->threadfn;
	void *data = create->data;
	int ret;

    /* 如果又调度回来了，说明被唤醒了 */
	ret = threadfn(data);
    /* 执行完线程函数，线程就死了 */
    __set_current_state(TASK_DEAD);
    /* 这个线程死了就换别的线程来工作 */
	schedule();
    /* 死了的线程不应该再运行了，如果还在运行，那就出问题了 */
    /* 这里要不要把中断和抢占关了？ */
    for (;;);
}

struct task_struct *kthread_create_on_node(int (*threadfn)(void *data),
					   void *data, int node,
					   const char namefmt[],
					   ...)
{
    struct task_struct *tsk = (struct task_struct *)kmalloc(sizeof(struct task_struct));
    union thread_union *thread= __get_free_pages(1); //get 2^1 pages

    tsk->stack = thread;
    tsk->state = TASK_SLEEP;

    //TODO:线程的名字要放进来

    thread->thread_info.task = tsk;
    thread->thread_info.cpu_context.pc = (unsigned long)kthread;
    thread->thread_info.cpu_context.sp = (unsigned long)(thread->stack + THREAD_START_SP);

	return tsk;
}