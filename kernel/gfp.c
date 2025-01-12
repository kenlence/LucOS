#include "gfp.h"
#include "list.h"
#include "page.h"
#include "spinlock.h"
#include "string.h"
#include "printk.h"
#include "kernel.h"

#define ORDER_2_PAGE(order) (0x1 << order)
#define ORDER_2_SIZE(order) (ORDER_2_PAGE(order) * PAGE_SIZE)

extern int memory_pool;
extern int memory_pool_end;
static unsigned long mem_pool = (unsigned long)(&memory_pool);
static unsigned long mem_pool_end = (unsigned long)(&memory_pool_end);

static struct list_head page_orders[GFP_MAX_ORDER + 1];
static spinlock_t lock;

struct page {
    unsigned int order;
    struct list_head list;
};

static struct page *init_page(void *mem, int order)
{
    struct page *page = (struct page *)mem;

    memset(page, 0, sizeof(*page));
    page->order = GFP_MAX_ORDER;
    INIT_LIST_HEAD(&page->list);
    return page;
}

void gfp_init()
{
    printk("total memery is: %d MB\n", (mem_pool_end - mem_pool) >> 20);

    spin_lock_init(&lock);

    spin_lock(&lock);
    for (int i = 0; i <= GFP_MAX_ORDER; i++)
    {
        INIT_LIST_HEAD(&page_orders[i]);
    }

    for (unsigned long mem = mem_pool; mem < mem_pool_end; mem += ORDER_2_SIZE(GFP_MAX_ORDER)) {
        struct page *page = init_page((void *)mem, GFP_MAX_ORDER);
        list_add_tail(&page->list, &page_orders[GFP_MAX_ORDER]);
    }
    spin_unlock(&lock);
}

static void *do_alloc_page(unsigned int order)
{
    struct list_head *head;
    struct page *page;
    void *mem;

    if (order > GFP_MAX_ORDER)
        return NULL;

    if (!list_empty(&page_orders[order]))
    {
        head = page_orders[order].next;
        list_del(head);
        page = list_entry(head, struct page, list);
        memset(page, 0, sizeof(struct page));
        return (void *)page;
    }

    if (order == GFP_MAX_ORDER)
        return NULL;

    mem = do_alloc_page(order + 1);
    if (mem == NULL)
        return NULL;

    page = init_page((void *)((unsigned long)mem + ORDER_2_SIZE(order)), order);
    list_add_tail(&page->list, &page_orders[order]);
    return mem;
}

void *alloc_pages(unsigned int order)
{
    void *mem;

    spin_lock(&lock);
    mem = do_alloc_page(order);
    spin_unlock(&lock);
    return mem;
}

static unsigned long get_buddy(unsigned long addr, unsigned int order)
{
    unsigned long tmp;

    tmp = addr >> PAGE_SHIFT;
    if ((tmp & 0x1) == 0)
        tmp++;
    return tmp << PAGE_SHIFT;
}

static void do_free_pages(unsigned long addr, unsigned int order)
{
    unsigned long buddy;
    struct page *page;
    struct page *n;

    if (order > GFP_MAX_ORDER)
        return ;

    buddy = get_buddy(addr, order);

    list_for_each_entry_safe(page, n, &page_orders[order], list) {
        if ((unsigned long)page == buddy) {
            list_del(&page->list);
            do_free_pages(min(buddy, addr), order + 1);
            return ;
        }
    }

    page = init_page((void *)addr, order);
    list_add_tail(&page->list, &page_orders[order]);
}

void free_pages(unsigned long addr, unsigned int order)
{
    spin_lock(&lock);
    do_free_pages(addr, order);
    spin_unlock(&lock);
}
