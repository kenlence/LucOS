#ifndef __LUCOS_SYSTICK_H_
#define __LUCOS_SYSTICK_H_

#include <stdint.h>

#define HZ 1000  // 每毫秒一次时钟

void systick_init(void);

uint64_t get_systick(void);

#endif