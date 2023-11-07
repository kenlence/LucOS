#ifndef __MALLOCTOR_H_
#define __MALLOCTOR_H_

void mem_pool_init();
void *kmalloc(unsigned int size);
void kfree(const void *);

#endif
