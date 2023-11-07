#include "malloctor.h"
#include "print.h"

extern int memory_pool;
extern int memory_pool_end;
static unsigned char *mem_pool = (unsigned char *)(&memory_pool);
static unsigned char *mem_pool_end = (unsigned char *)(&memory_pool_end);
static void *free_ptr;

void mem_pool_init() {
    printk("mem_pool: %#x\n", mem_pool);
    printk("mem_pool_end: %#x\n", mem_pool_end);
    printk("total memery is: %d MB\n", (mem_pool_end - mem_pool) >> 20);
    free_ptr = (void *)memory_pool;
}

void *kmalloc(unsigned int size) {
    void *ret = free_ptr;
    free_ptr += size;
    if ((unsigned long)free_ptr >= (unsigned long)mem_pool_end) {
        printk("memory to end, PANIC\n");
        for(;;);
    }

    return ret;
}

void kfree(const void *block) {
    return ;
}


