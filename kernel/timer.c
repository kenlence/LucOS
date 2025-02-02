#include "timer.h"
#include "workqueue.h"
#include "epit.h"
#include "printk.h"
#include "types.h"

// timer支持的最大精度为毫秒级
static LIST_HEAD(timer_list_10ms);
static LIST_HEAD(timer_list_100ms);
static LIST_HEAD(timer_list_1000ms);
static LIST_HEAD(timer_list_over_1s);
static DEFINE_SPINLOCK(lock);
struct workqueue *wq;
static uint64_t systick = 0;

static void timer_func(struct work *work)
{
    struct timer *timer;
    struct timer *n;
    unsigned long flags;
    uint32_t systick_l;

    spin_lock_irqsave(&lock, flags);
    systick++;
    systick_l = (uint32_t)systick;

    // 1s以上的定时器，每秒判断一次
    if (systick_l % 1000 == 0) {
        list_for_each_entry_safe(timer, n, &timer_list_over_1s, entry) {
            if (timer->expires <= systick + 1000) {
                list_del(&timer->entry);
                list_add(&timer->entry, &timer_list_100ms);
            }
        }
    }

    // 1000ms内可能超时的，需要每100毫秒判断一下
    if (systick_l % 100 == 0) {
        list_for_each_entry_safe(timer, n, &timer_list_1000ms, entry) {
            if (timer->expires <= systick + 1000) {
                list_del(&timer->entry);
                list_add(&timer->entry, &timer_list_100ms);
            }
        }
    }

    // 100ms内可能超时的，需要每10毫秒判断一下
    if (systick_l % 10 == 0) {
        list_for_each_entry_safe(timer, n, &timer_list_100ms, entry) {
            if (timer->expires <= systick + 100) {
                list_del(&timer->entry);
                list_add(&timer->entry, &timer_list_10ms);
            }
        }
    }

    // 10ms内可能超时的，需要每毫秒判断一下
    list_for_each_entry_safe(timer, n, &timer_list_10ms, entry) {
        if (timer->expires <= systick) {
            list_del(&timer->entry);
            spin_unlock_irqrestore(&lock, flags);
            timer->func(timer);
            spin_lock_irqsave(&lock, flags);
        }
    }

    spin_unlock_irqrestore(&lock, flags);
}
static DEFINE_WORK(timer_work, timer_func);

static int epit1_isr(void *arg)
{
    queue_work(wq, &timer_work);   // TODO:实测下来工作队列会丢帧
    return 0;
}

void timer_handle_init(void)
{
    wq = workqueue_alloc();
    if (!wq) {
        printk("workqueue_alloc failed\n");
        return ;
    }

    epit1_init(0, 66000);
    epit1_register_irqhandler(epit1_isr, NULL);
}

void add_timer(struct timer *timer)
{
    unsigned long flags;
    uint64_t rest;

    spin_lock_irqsave(&lock, flags);
    rest = timer->expires - systick;

    if (rest < 10) {
        list_add(&timer->entry, &timer_list_10ms);
    } else if (rest < 100) {
        list_add(&timer->entry, &timer_list_100ms);
    } else if (rest < 1000) {
        list_add(&timer->entry, &timer_list_1000ms);
    } else {
        list_add(&timer->entry, &timer_list_over_1s);
    }
    spin_unlock_irqrestore(&lock, flags);
}

void del_timer(struct timer *timer)
{
    unsigned long flags;

    spin_lock_irqsave(&lock, flags);
    list_del(&timer->entry);
    spin_unlock_irqrestore(&lock, flags);
}

void mod_timer(struct timer *timer, uint64_t expires)
{
    del_timer(timer);
    timer->expires = expires;
    add_timer(timer);
}

uint64_t get_systick(void)
{
    unsigned long flags;

    spin_lock_irqsave(&lock, flags);
    uint64_t ret = systick;
    spin_unlock_irqrestore(&lock, flags);
    return ret;
}
