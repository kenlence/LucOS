#ifndef __TIMER_H__
#define __TIMER_H__

#include <imx_gpt.h>
#include <stdbool.h>
#include <stdint.h>

extern void udelay(uint32_t);
extern void mdelay(uint32_t);
extern void timer_init(void);

#endif
