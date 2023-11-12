#include "sched.h"
#include "../mm/malloctor.h"
#include "../include/stddef.h"

#define MAX_TASK_NUM 64

#define __init_task_data __attribute__((__section__(".data..init_task")))

struct task init_task;

union thread_union init_thread_union __init_task_data = {
    {.task = &init_task,}
};

struct task init_task = {
    .state = 0,
    .stack = &(init_thread_union.thread_info),
    .pid = 0,
};

static struct task *current_task = &init_task;

/*创建的task*/
static struct task *task_list[MAX_TASK_NUM];
static unsigned int free_pid = 0;

struct task *get_current_task(void) {
    return current_task;
}

static unsigned int alloc_pid(void) {
    if (free_pid >= MAX_TASK_NUM) {
        return -1;
    } else {
        free_pid++;
        return free_pid - 1;
    }
}

static unsigned int register_task(struct task *tsk) {
    int pid = alloc_pid();
    if (pid < 0) {
        return pid;
    }

    task_list[pid] = tsk;
    return pid;
}

struct task *create_task(task_func func) {
    struct task *tsk = (struct task *)kmalloc(sizeof(struct task));
    union thread_union *thread= __get_free_pages(1); //get 2^1 pages

    tsk->stack = thread;
    tsk->state = 0; //runnable
    thread->thread_info.task = tsk;
    thread->thread_info.cpu_context.pc = (unsigned long)func;
    thread->thread_info.cpu_context.sp = (unsigned long)(thread->stack + THREAD_START_SP);

    tsk->pid = register_task(tsk);
    return tsk;
}

static inline struct task *pick_next_task(struct task *prev)
{
    if (current != &init_task) {
        return &init_task;
    }

    for (int i = 0; i < MAX_TASK_NUM; i++) {
        if (task_list[i] == NULL) 
            continue;

        if (task_list[i] != prev && task_list[i]->state == 0) {
            return task_list[i];
        }
    }

    return &init_task;
}

extern struct task *__switch_to(struct task *, struct thread_info *, struct thread_info *);

#define switch_to(prev,next,last)					\
do {									\
	last = __switch_to(prev,task_thread_info(prev), task_thread_info(next));	\
} while (0)

static void context_switch(struct task *prev, struct task *next)
{
	/* Here we just switch the register state and the stack. */
	switch_to(prev, next, prev);
    __asm__ __volatile__("":::"memory");
}


static void __schedule(void)
{
	struct task *prev, *next;

	//preempt_disable();  禁用抢占
	prev = current;

    //如果是主动发起的调度，线程一般会把自己的状态改为不是running
    //就要把线程移除running队列，放入其他队列
    //如果是线程抢占了，那么保持再running即可，什么都不用做直接往下

	next = pick_next_task(prev);

	if (prev != next) {
		current_task = next;
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



