#include "imx_uart.h"
#include "print.h"
#include "epit.h"
#include "int.h"
#include "clk.h"
#include "gpt.h"
#include "imx6ul.h"

void board_init(void) {
	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	clk_enable();				/* 使能所有的时钟 			*/
    uart_init();
}

void kernel_init(void) {
    


/* 1ms, timer is used for scheduler, so start when scheduler start*/
    epit1_init(0, 66000);	    
    return;
}

void test_app(void) {
    char *str = "Hello world\n";
    printk("%s\n", str);
}

void dump_gic(void) {
  	GIC_Type *gic = (GIC_Type *)(__get_CBAR() & 0xFFFF0000UL);
    printk("vbar : %#x\n", (unsigned int)__get_VBAR());
    printk("gic & *gic : %#x\n", (unsigned int)gic);
    printk("CTLR : %#x\n", gic->D_CTLR);
    printk("TYPER: %#x\n", gic->D_TYPER);
    printk("IIDR : %#x\n", gic->D_IIDR);
}


int entry()
{
    board_init();
    kernel_init();

    __asm("MRC p15, 4, r1, c15, c0, 0");

    test_app();
    for (;;) {
        mdelay(1000);
        dump_gic();
    }
    return 0;
}
