#include "malloctor.h"
#include "print.h"

extern int memory_pool;
extern int memory_pool_end;
static unsigned char *mem_pool = (unsigned char *)(&memory_pool);
static unsigned char *mem_pool_end = (unsigned char *)(&memory_pool_end);

void *kmalloc(unsigned int size) {
    printk("mem_pool: %#x\n", mem_pool);
    printk("mem_pool_end: %#x\n", mem_pool_end);
    return (void *)0;
}

void kfree(const void *block) {
    return ;
}


