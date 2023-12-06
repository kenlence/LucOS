#ifndef _LINUX_TRACE_IRQFLAGS_H
#define _LINUX_TRACE_IRQFLAGS_H

#include "../arm/irqflags.h"

#define raw_local_irq_disable()		arch_local_irq_disable()
#define raw_local_irq_enable()		arch_local_irq_enable()

#define raw_local_save_flags(flags)			\
	do {						\
		flags = arch_local_save_flags();	\
	} while (0)

#define raw_irqs_disabled_flags(flags)			\
	({						\
		arch_irqs_disabled_flags(flags);	\
	})

#define raw_local_irq_save(flags)			\
	do {						\
		flags = arch_local_irq_save();		\
	} while (0)
#define raw_local_irq_restore(flags)			\
	do {						\
		arch_local_irq_restore(flags);		\
	} while (0)

#define local_irq_enable()	do { raw_local_irq_enable(); } while (0)
#define local_irq_disable()	do { raw_local_irq_disable(); } while (0)
#define local_irq_save(flags)					\
	do {							\
		raw_local_irq_save(flags);			\
	} while (0)
#define local_irq_restore(flags) do { raw_local_irq_restore(flags); } while (0)

#define irqs_disabled()					\
	({						\
		unsigned long _flags;			\
		raw_local_save_flags(_flags);		\
		raw_irqs_disabled_flags(_flags);	\
	})

#endif