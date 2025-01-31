#include "systick.h"
#include "spinlock.h"
#include "epit.h"
#include "stddef.h"
#include "printk.h"
#include "epit.h"

static uint64_t systick = 0;
static spinlock_t systick_lock;

static int systick_add(void *arg)
{
    spin_lock(&systick_lock);
    systick++;
    spin_unlock(&systick_lock);
    return 0;
}

void systick_init(void)
{
    spin_lock_init(&systick_lock);
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

