#ifndef __ASM_ARM_THREAD_INFO_H
#define __ASM_ARM_THREAD_INFO_H

#include "page.h"

#define THREAD_SIZE_ORDER	1
#define THREAD_SIZE		(PAGE_SIZE << THREAD_SIZE_ORDER)
#define THREAD_START_SP		(THREAD_SIZE - 8)

struct task_struct;

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
	struct task_struct *task;
	struct cpu_context_save	cpu_context;	/* cpu context */
};

#define INIT_THREAD_INFO(tsk)						\
{									\
	.task		= &tsk,						\
}

#define init_thread_info	(init_thread_union.thread_info)
#define init_stack		(init_thread_union.stack)

register unsigned long current_stack_pointer __asm__ ("sp");

static inline struct thread_info *current_thread_info(void)
{
	return (struct thread_info *)
		(current_stack_pointer & ~(THREAD_SIZE - 1));
}

#endif