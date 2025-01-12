#ifndef __LUCOS_SPINLOCK_H
#define __LUCOS_SPINLOCK_H

#include "preempt.h"
#include "irqflags.h"

typedef struct { } spinlock_t;

#define spin_lock_init(_lock)

static inline void spin_lock(spinlock_t *lock)
{
	preempt_disable();
}

#define spin_lock_irqsave(lock, flags)				\
do {								\
	local_irq_save(flags);			\
	preempt_disable();				\
} while (0)

static inline void spin_unlock(spinlock_t *lock)
{
	preempt_enable();
}

static inline void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
{
	local_irq_restore(flags);
	preempt_enable();
}

#endif /* __LUCOS_SPINLOCK_H */
