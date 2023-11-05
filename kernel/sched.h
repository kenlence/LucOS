#ifndef __SCHED_H_
#define __SCHED_H_

#define THREAD_SIZE (8 * 1024)

struct runtime {

};

struct thread {

};

/* 为一个线程申请的内存是THREAD_SIZE，在低地址放置了一个描述线程的结构体
 * 结构体以上的地址为栈，这个THREAD_SIZE的最高地址就设置为栈指针
 * 栈的大小等于 THRAD_SIZE - sizeof(struct thread)
 */
union thread_union {
	struct thread thrad;
	unsigned long stack[THREAD_SIZE/sizeof(long)];
};

#endif