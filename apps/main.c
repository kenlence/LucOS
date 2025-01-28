/* user code main entry */
#include "printk.h"
#include "sched.h"
#include "current.h"
#include "kthread.h"
#include "slab.h"

int user_define_task(void *arg)
{
    void *mem;
    for (;;) {
        printk("%s\n", __func__);
        mem = kmalloc(2);
        kfree(mem);
        schedule();
    }
}

int user_default_task(void *arg)
{
    struct task_struct *user_define = kthread_run(user_define_task, 0, "user_task_2");
    void *mem;
    int count = 0;

    (void)user_define;
    for (;;) {
        count++;
        printk("%s, %d\n", __func__, count);
        mem = kmalloc(1);
        kfree(mem);
        schedule();
    }
}
