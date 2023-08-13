#ifndef __EPIT_H_
#define __EPIT_H_

#include "imx6ul.h"

void epit1_init(unsigned int frac, unsigned int value);
void epit1_register_irqhandler(int(*isr)(void *), void *arg);

#endif