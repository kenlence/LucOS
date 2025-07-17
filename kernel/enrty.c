#include "imx_uart.h"
#include "printk.h"
#include "slab.h"
#include "sched.h"
#include "int.h"
#include "clk.h"
#include "imx6ul.h"
#include "kthread.h"
#include "current.h"
#include "gfp.h"
#include "types.h"
#include "timer.h"

static void board_init(void) {
	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	clk_enable();				/* 使能所有的时钟 			*/
    uart_init();
}

static void kernel_init(void) {
    gfp_init();
    kmem_cache_init();

/* 1ms, timer is used for scheduler, so start when scheduler start*/
    timer_handle_init();
    return;
}

extern int user_default_task(void *arg);

int entry()
{
    board_init();
    kernel_init();

    printk("kernel started\n");

    struct task_struct *user_default =
        kthread_run(user_default_task, 0, "user_default_task");
    (void)user_default;

    for (;;) {
        schedule();
    }
    return 0;
}
