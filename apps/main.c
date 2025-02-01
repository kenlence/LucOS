/* user code main entry */
#include "printk.h"
#include "sched.h"
#include "current.h"
#include "kthread.h"
#include "slab.h"
#include "mutex.h"
#include <sched.h>
#include <stdint.h>
#include "systick.h"

DEFINE_MUTEX(mutex);
static int count = 0;

int user_define_task(void *arg)
{
    for (;;) {
        schedule();
        if (mutex_trylock(&mutex) == 0) {
            continue;
        }

        if (count > 0) {
            count--;
            printk("count: %d\n", count);
        }
        mutex_unlock(&mutex);
    }
}

int user_default_task(void *arg)
{
    struct task_struct *user_define = kthread_run(user_define_task, 0, "user_task_2");
    uint64_t last = get_systick();

    (void)user_define;
    for (;;) {
        if (get_systick() - last > 1000) {
            last = get_systick();
            mutex_lock(&mutex);
            count++;
            mutex_unlock(&mutex);
        }
        schedule();
    }
}
