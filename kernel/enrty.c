#include "imx_uart.h"
#include "print.h"
#include "timer.h"


void board_init(void) {
    uart_init();
    timer_init();
}

void kernel_init(void) {
    return;
}

void main(void) {
    char *str = "Hello world\n";
    printk("%s\n", str);
}

int entry()
{
    board_init();
    kernel_init();
    main();
    for(;;);
    return 0;
}
