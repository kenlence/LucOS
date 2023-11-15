/* user code main entry */
#include "../include/printk.h"
#include "../include/sched.h"

int user_default_task(void *arg)
{
    for (;;) {
        printk("main loop\n");
        schedule();
    }
}
