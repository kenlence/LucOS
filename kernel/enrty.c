#include "imx_uart.h"
#include "../include/printk.h"
#include "../include/slab.h"
#include "../include/sched.h"
#include "epit.h"
#include "int.h"
#include "clk.h"
#include "imx6ul.h"
#include "../include/kthread.h"

static void board_init(void) {
	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	clk_enable();				/* 使能所有的时钟 			*/
    uart_init();
}

static void kernel_init(void) {
    mem_pool_init();

/* 1ms, timer is used for scheduler, so start when scheduler start*/
    //epit1_init(0, 66000);
    return;
}

extern int user_default_task(void *arg);

int entry()
{
    board_init();
    kernel_init();

    printk("kernel started\n");

    kthread_run(user_default_task, 0, "user_default_task");

    for (;;) {
        printk("idle thread\n");
        schedule();
    }
    return 0;
}
