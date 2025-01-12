#include "slab.h"
#include "printk.h"
#include "gfp.h"
#include "page.h"

void *kmalloc(unsigned int size) {

    return alloc_pages(0);
}

void kfree(const void *block) {
    return free_pages(block, 0);
}
