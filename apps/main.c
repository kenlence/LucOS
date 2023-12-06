/* user code main entry */
#include "../include/printk.h"
#include "../include/sched.h"
#include "../include/current.h"
#include "../include/kthread.h"

int user_define_task(void *arg)
{
    for (;;) {
        schedule();
    }
}

int user_default_task(void *arg)
{
    struct task_struct *user_define = kthread_run(user_define_task, 0, "user_task_2");

    (void)user_define;
    for (;;) {
        schedule();
    }
}
