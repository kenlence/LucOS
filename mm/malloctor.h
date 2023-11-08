#ifndef __MALLOCTOR_H_
#define __MALLOCTOR_H_

#define PAGE_SIZE (4 * 1024)

void mem_pool_init();
void *kmalloc(unsigned int size);
void kfree(const void *);
void *__get_free_pages(unsigned int order);

#endif
