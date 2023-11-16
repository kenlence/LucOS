#include "../include/kthread.h"
#include "../include/kernel.h"
#include "../include/slab.h"
#include "../include/gfp.h"
#include "../include/list.h"
#include "../include/sched.h"
#include "../include/current.h"

typedef int(*func)(void*);

//通过r4和r5给kthread传参
register unsigned long r4 __asm__ ("r4");
register unsigned long r5 __asm__ ("r5");

//给函数套个衣服，不然裸跑的话，出问题影响整个系统
static void kthread(void)
{
    int(*threadfn)(void*) = (func)r4;
    void *data = (void*)r5;

	threadfn(data);
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
    thread->thread_info.cpu_context.r4 = (unsigned long)threadfn;
    thread->thread_info.cpu_context.r5 = (unsigned long)data;
    thread->thread_info.cpu_context.sp = (unsigned long)(thread) + THREAD_START_SP;

    //把新创建的线程放进链表里
    list_add(&tsk->tasks, &current->tasks);
	return tsk;
}