#include "imx_uart.h"
#include "print.h"
#include "epit.h"
#include "int.h"
#include "clk.h"
#include "imx6ul.h"
#include "../mm/malloctor.h"
#include "sched.h"

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

static void test_app(void) {
    for (;;) {
        printk("test_app\n");
        schedule();
    }
}

static struct task* test;

int entry()
{
    board_init();
    kernel_init();

    printk("kernel started\n");

    test = create_task(test_app);

    for (;;) {
        printk("idle thread\n");
        schedule();
    }
    //__asm("MRC p15, 4, r1, c15, c0, 0");
    return 0;
}
