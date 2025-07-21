#include "slab.h"
#include "gfp.h"
#include "page.h"
#include "string.h"
#include "spinlock.h"
#include "stddef.h"
#include "list.h"

#define KMALLOC_CACHE_COUNT 8

// kmem_cache_create接口内部需要kmalloc，所以kmalloc使用的cache需要单独初始化以完成自举
static struct kmem_cache kmalloc_cache[KMALLOC_CACHE_COUNT]; // 16 ~ 2048
static spinlock_t slab_lock;
static struct list_head slab_caches;

static void *slab_malloc(unsigned int size)
{
    // 这里不用遍历，下标应该是可以直接算出来的
    for (int i = 0; i < KMALLOC_CACHE_COUNT; i++) {
        if (size <= kmalloc_cache[i].size) {
            return kmem_cache_alloc(&kmalloc_cache[i]);
        }
    }

    return NULL;
}

static void slab_free(const void *block, unsigned int size)
{
    // 这里不用遍历，下标应该是可以直接算出来的
    for (int i = 0; i < KMALLOC_CACHE_COUNT; i++) {
        if (size <= kmalloc_cache[i].size) {
            return kmem_cache_free(&kmalloc_cache[i], block);
        }
    }
}

void *kmalloc(unsigned int size)
{
    unsigned int *mem;
    unsigned int page;

    size++;
    if (size < 1 << (KMALLOC_CACHE_COUNT + 3)) {
        mem = (unsigned int *)slab_malloc(size);
    } else {
        page = size / PAGE_SIZE;
        if (size % PAGE_SIZE)
            page++;
        mem = alloc_pages(page >> PAGE_SHIFT);
    }

    *mem = size;
    return (void *)(mem + 1);
}

void kfree(const void *block)
{
    unsigned int *mem = (unsigned int *)block - 1;

    if (*mem < 1 << (KMALLOC_CACHE_COUNT + 3)) {
        return slab_free((void *)mem, *mem);
    }

    return free_pages((unsigned long)mem, 0);
}

void *kzalloc(unsigned int size)
{
    void *mem = kmalloc(size);
    if (mem)
        memset(mem, 0, size);
    return mem;
}

static void kmalloc_init(void)
{
    for (int i = 0; i < KMALLOC_CACHE_COUNT; i++) {
        kmalloc_cache[i].size = 1 << (i + 4);
        kmalloc_cache[i].slabs = NULL;
        list_add_tail(&kmalloc_cache[i].list, &slab_caches);
    }
}

void kmem_cache_init(void)
{
    INIT_LIST_HEAD(&slab_caches);
    spin_lock_init(&slab_lock);
    spin_lock(&slab_lock);
    kmalloc_init();
    spin_unlock(&slab_lock);
}

static struct slab *create_slab(unsigned int size)
{
    void *page = alloc_pages(0);
    struct slab *slab = (struct slab *)page;
    struct slab *tmp = (struct slab *)page;

    if (page == NULL)
        return NULL;

    slab->next_slab = NULL;
    slab->next_block = NULL;
    slab->cnt = 0;

    while ((unsigned long)tmp + size < (unsigned long)page + PAGE_SIZE) {
        tmp = (struct slab *)((unsigned long)tmp + size);
        tmp->next_block = slab->next_block;
        slab->next_block = tmp;
        slab->cnt++;
    }

    return slab;
}

static void *alloc_from_slab(struct slab *slab)
{
    void *mem;

    if (slab->next_block == NULL) {
        mem = slab;
    } else {
        mem = slab->next_block;
        slab->next_block = slab->next_block->next_block;
    }

    slab->cnt--;
    return mem;
}

void *kmem_cache_alloc(struct kmem_cache *cache)
{
    void *mem;

    if (cache == NULL)
        return NULL;

    if (cache->slabs == NULL) {
        cache->slabs = create_slab(cache->size);
        if (cache->slabs == NULL)
            return NULL;
    }

    mem = alloc_from_slab(cache->slabs);
    if (cache->slabs->cnt == 0) {
        cache->slabs = cache->slabs->next_slab;
    }

    return mem;
}

static struct slab *find_slab(struct kmem_cache *cache, const void *block)
{
    for (struct slab *slab = cache->slabs; slab != NULL; slab = slab->next_slab) {
        if (((unsigned long)block >> PAGE_SHIFT) == ((unsigned long)slab >> PAGE_SHIFT))
            return slab;
    }

    return NULL;
}

void kmem_cache_free(struct kmem_cache *cache, const void *block)
{
    if (cache == NULL || block == NULL)
        return;

    struct slab *slab = find_slab(cache, block);
    if (slab == NULL) {
        struct slab *free_slab = (struct slab *)block;
        free_slab->next_slab = cache->slabs;
        cache->slabs = free_slab;
        free_slab->cnt = 1;
        free_slab->next_block = NULL;
        return ;
    }

    struct slab *free_slab = (struct slab *)block;
    free_slab->next_block = slab->next_block;
    slab->next_block = free_slab;
    slab->cnt++;
/*
    if (slab->cnt == PAGE_SIZE / cache->size)
        free_pages((unsigned long)slab & (~0xffful), 0);
*/
}
