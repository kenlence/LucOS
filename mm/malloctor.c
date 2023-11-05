#include "malloctor.h"
#include "print.h"

extern int memory_pool;
extern int memory_pool_end;
static unsigned char *mem_pool = (unsigned char *)(&memory_pool);
static unsigned char *mem_pool_end = (unsigned char *)(&memory_pool_end);

void mem_pool_init() {
    printk("mem_pool: %#x\n", mem_pool);
    printk("mem_pool_end: %#x\n", mem_pool_end);
    printk("total memery is: %#x\n", mem_pool_end - mem_pool);
}

void *kmalloc(unsigned int size) {
    static void *ptr = memory_pool;
    void *ret = ptr;
    ptr += size;
    if (ptr >= mem_pool_end) {
        printk("memory to end, PANIC\n");
        for(;;);
    }

    return (void *)ptr;
}

void kfree(const void *block) {
    return ;
}


