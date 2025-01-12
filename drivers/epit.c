#include "epit.h"
#include "int.h"
#include "stddef.h"

static int(*epit1_isr)(void *);
static void *epit1_arg;

void epit1_irqhandler(void)
{
	if(EPIT1->SR & (1<<0)) {
        if (epit1_isr) {
            epit1_isr(epit1_arg);
        }
	}


	EPIT1->SR |= 1<<0; 				/* 清除中断标志位 */
}

void epit1_init(unsigned int frac, unsigned int value)
{
	if(frac > 0XFFF)
		frac = 0XFFF;

	EPIT1->CR = 0;	/* 先清零CR寄存器 */

	/*
     * CR寄存器:
     * bit25:24 01 时钟源选择Peripheral clock=66MHz
     * bit15:4  frac 分频值
     * bit3:	1  当计数器到0的话从LR重新加载数值
     * bit2:	1  比较中断使能
     * bit1:    1  初始计数值来源于LR寄存器值
     * bit0:    0  先关闭EPIT1
     */
	EPIT1->CR = (1<<24 | frac << 4 | 1<<3 | 1<<2 | 1<<1);

	EPIT1->LR = value;	/* 倒计数值 */
	EPIT1->CMPR	= 0;	/* 比较寄存器，当计数器值和此寄存器值相等的话就会产生中断 */

	/* 使能GIC中对应的中断 			*/
	GIC_EnableIRQ(EPIT1_IRQn);

	/* 注册中断服务函数 			*/
	system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)epit1_irqhandler, NULL);

	EPIT1->CR |= 1<<0;	/* 使能EPIT1 */
}

void epit1_register_irqhandler(int(*isr)(void *), void *arg) {
    epit1_isr = isr;
    epit1_arg = arg;
}


