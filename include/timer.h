#ifndef __LUCOS_TIMER_H__
#define __LUCOS_TIMER_H__

#include "types.h"
#include "list.h"

#define HZ 1000  // 每毫秒一次时钟

struct timer;
typedef void (*timer_func_t)(struct timer *timer);

struct timer {
	struct list_head entry;
	uint64_t expires;
	timer_func_t func;
	void *data;
};

#define __TIMER_INITIALIZER(name, f, d) { \
    .entry = LIST_HEAD_INIT((name).entry), \
    .expires = 0, \
    .func = (f), \
    .data = (d) \
    }

#define DEFINE_TIMER(name, f, d) \
    struct timer name = __TIMER_INITIALIZER(name, f, d)

static inline void timer_init(struct timer *timer, timer_func_t func, void *data)
{
	*timer = (struct timer) __TIMER_INITIALIZER(*timer, func, data);
}

void timer_handle_init(void);
void add_timer(struct timer *timer);
void del_timer(struct timer *timer);
void mod_timer(struct timer *timer, uint64_t expires);
uint64_t get_systick(void);

#endif