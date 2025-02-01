#include "mutex.h"
#include <semaphore.h>

void mutex_init(struct mutex *lock)
{
    sem_init(&lock->sem, 1);
}

void mutex_lock(struct mutex *lock)
{
    sem_down(&lock->sem);
}

int mutex_trylock(struct mutex *lock)
{
    return sem_try_down(&lock->sem);
}

void mutex_unlock(struct mutex *lock)
{
    sem_up(&lock->sem);
}
