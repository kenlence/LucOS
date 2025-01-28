#include "panic.h"
#include "printk.h"
#include <stdarg.h>

void panic(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    printk("PANIC: ");
    printk(fmt, args);
    va_end(args);

    while (1);
}