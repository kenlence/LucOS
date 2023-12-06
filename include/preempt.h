#ifndef __LINUX_PREEMPT_H
#define __LINUX_PREEMPT_H

#include "compiler.h"

extern void preempt_count_add(int val);
extern void preempt_count_sub(int val);

#define preempt_count_inc() preempt_count_add(1)
#define preempt_count_dec() preempt_count_sub(1)

#define preempt_disable() \
do { \
	preempt_count_inc(); \
	barrier(); \
} while (0)

//内核里如果抢占的计数到了0，即真的发生到了是能抢占的时候，他会立刻重新调度一下
#define preempt_enable() \
do { \
	barrier(); \
	preempt_count_dec(); \
} while (0)

#endif