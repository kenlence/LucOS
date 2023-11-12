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
    free_ptr = (void *)mem_pool;
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

void *__get_free_pages(unsigned int order) {
    //alian for page
    //free_ptr = (void*)(((unsigned int)free_ptr & 0xfffff000) + PAGE_SIZE);
    unsigned int tmp;
    tmp = (unsigned int) free_ptr;
    tmp &= 0xfffff000;
    tmp += PAGE_SIZE;
    free_ptr = (void *)tmp;
    return kmalloc(PAGE_SIZE * (1 << order));
}

