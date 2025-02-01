#include "systick.h"
#include "spinlock.h"
#include "epit.h"
#include "stddef.h"
#include "printk.h"
#include "epit.h"
#include "workqueue.h"

static uint64_t systick = 0;
static spinlock_t systick_lock;
static struct workqueue *wq;

static void systick_func(struct work *work)
{
    spin_lock(&systick_lock);
    systick++;
    spin_unlock(&systick_lock);
}
static DEFINE_WORK(systick_work, systick_func);

static int systick_add(void *arg)
{
    queue_work(wq, &systick_work);
    return 0;
}

void systick_init(void)
{
    spin_lock_init(&systick_lock);
    wq = workqueue_alloc();
    if (wq == NULL) {
        printk("workqueue_alloc failed\n");
        return;
    }

    epit1_init(0, 66000);
    epit1_register_irqhandler(systick_add, NULL);
}

uint64_t get_systick(void)
{
    spin_lock(&systick_lock);
    uint64_t ret = systick;
    spin_unlock(&systick_lock);
    return ret;
}

