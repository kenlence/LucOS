#include "../include/preempt.h"
#include "../include/irqflags.h"

static int preempt_count = 0;

void preempt_count_add(int val)
{
    preempt_count += val;
}

void preempt_count_sub(int val)
{
    preempt_count -= val;
}

int preemptible(void)
{
    return preempt_count == 0 && !irqs_disabled();
}