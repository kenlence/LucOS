#ifndef __MALLOCTOR_H_
#define __MALLOCTOR_H_

#include "types.h"

struct slab {
    struct slab *next_slab;
    struct slab *next_block;
    unsigned char cnt;
};

struct kmem_cache {
    unsigned int size; // slab针对小内存设计，不处理大于等于PAGE_SIZE的内存
    struct slab *slabs;
    struct list_head list; // 这里可以不省空间，用list.h
};

void *kmalloc(unsigned int size);
void *kzalloc(unsigned int size);
void kfree(const void *);

void kmem_cache_init(void);

#if 0
struct kmem_cache *kmem_cache_create(const char *, size_t, size_t,
			unsigned long,
			void (*)(void *));
void kmem_cache_destroy(struct kmem_cache *);
#endif

void *kmem_cache_alloc(struct kmem_cache *cache);
void kmem_cache_free(struct kmem_cache *cache, const void *block);

#endif
