#ifndef __SCHED_H_
#define __SCHED_H_

#define THREAD_SIZE (8 * 1024)
#define THREAD_START_SP (THREAD_SIZE - 8)

#define task_thread_info(task) ((struct thread_info *)(task)->stack)
#define task_stack_page(task) ((task)->stack)

enum task_state {
	TASK_UNRUNNABLE = -1,
	TASK_RUNNABLE = 0,
	TASK_STOPPED = 1,
};

typedef void(*task_func)(void);

struct task {
	/* -1 unrunnable, 0 runnable, >0 stopped */
	volatile long state;
	unsigned int pid;
	void *stack;
	//struct list_head tasks;
};

struct cpu_context_save {
	unsigned long r4;
	unsigned long r5;
	unsigned long r6;
	unsigned long r7;
	unsigned long r8;
	unsigned long r9;
	unsigned long sl;
	unsigned long fp;
	unsigned long sp;
	unsigned long pc;
	unsigned long extra[2];		/* Xscale 'acc' register, etc */
};

/* 由于没有实现类似linux的自动生成宏定义，因为如果修改了这个结构体
 * 就需要对应修改switch_to，主要是cpu_context在这个结构体中的偏移
 */
struct thread_info {
	struct task *task;
	struct cpu_context_save	cpu_context;	/* cpu context */
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

struct task *create_task(task_func func);

void schedule(void);

#define current get_current_task()

struct task *get_current_task(void);

#endif