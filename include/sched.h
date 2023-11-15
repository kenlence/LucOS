#ifndef __SCHED_H_
#define __SCHED_H_

#include "list.h"
#include "thread_info.h"

#define TASK_COMM_LEN 16

#define TASK_RUNNING			0
#define TASK_SLEEP				2
#define TASK_DEAD				64

#define task_thread_info(task) ((struct thread_info *)(task)->stack)
#define task_stack_page(task) ((task)->stack)

struct task_struct {
	/* -1 unrunnable, 0 runnable, >0 stopped */
	volatile long state;
	void *stack;
	struct list_head tasks;
	char comm[TASK_COMM_LEN]; //线程的名字
};

/* 为一个线程申请的内存是THREAD_SIZE，在低地址放置了一个描述线程的结构体
 * 结构体以上的地址为栈，这个THREAD_SIZE的最高地址就设置为栈指针
 * 栈的大小等于 THRAD_SIZE - sizeof(struct thread)
 * 这个联合体用起来必须是8K对齐的，不然无法通过栈指针找到thread_info
 */
union thread_union {
	struct thread_info thread_info;
	unsigned long stack[THREAD_SIZE/sizeof(long)];
};

void schedule(void);

//void yield(void);

extern union thread_union init_thread_union;
extern struct task_struct init_task;

//Return: 1 if the process was woken up, 0 if it was already running.
int wake_up_process(struct task_struct *tsk);

/* 不带内存屏障的 */
#define __set_current_state(state_value)		\
	do { current->state = (state_value); } while (0)

/* 带内存屏障的，还没实现 */
#define set_current_state(state_value)

#endif