#include "mutex.h"
#include <semaphore.h>

void mutex_init(struct mutex *lock)
{
    sema_init(&lock->sem, 1);
}

void mutex_lock(struct mutex *lock)
{
    sema_down(&lock->sem);
}

int mutex_trylock(struct mutex *lock)
{
    return sema_try_down(&lock->sem);
}

void mutex_unlock(struct mutex *lock)
{
    sema_up(&lock->sem);
}
