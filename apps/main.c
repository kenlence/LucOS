/* user code main entry */
#include "printk.h"
#include "sched.h"
#include "current.h"
#include "kthread.h"

int user_define_task(void *arg)
{
    for (;;) {
        printk("%s\n", __func__);
        schedule();
    }
}

int user_default_task(void *arg)
{
    struct task_struct *user_define = kthread_run(user_define_task, 0, "user_task_2");

    (void)user_define;
    for (;;) {
        printk("%s\n", __func__);
        schedule();
    }
}
