#ifndef __LUCOS_MUTEX_H__
#define __LUCOS_MUTEX_H__

#include "semaphore.h"

struct mutex {
    struct semaphore sem;
};

#define __MUTEX_INITIALIZER(lockname) \
    { .sem = __SEMAPHORE_INITIALIZER((lockname).sem, 1) }

#define DEFINE_MUTEX(mutexname) \
	struct mutex mutexname = __MUTEX_INITIALIZER(mutexname)

void mutex_init(struct mutex *lock);
void mutex_lock(struct mutex *lock);
int mutex_trylock(struct mutex *lock);
void mutex_unlock(struct mutex *lock);

#endif