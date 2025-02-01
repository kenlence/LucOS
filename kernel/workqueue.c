#include "workqueue.h"
#include "slab.h"
#include "kthread.h"

static int workqueue_thread(void *data)
{
    struct workqueue *wq = (struct workqueue *)data;
    struct work *work;
    struct list_head *pos, *n;

    for (;;) {
        sem_down(&wq->sem);
        spin_lock(&wq->lock);
        list_for_each_safe(pos, n, &wq->works) {
            work = list_entry(pos, struct work, list);
            list_del(pos);
            spin_unlock(&wq->lock);
            work->func(work->data);
            spin_lock(&wq->lock);
        }
        spin_unlock(&wq->lock);
    }
    return 0;
}

struct workqueue *workqueue_alloc()
{
    struct workqueue *wq = kmalloc(sizeof(struct workqueue));
    INIT_LIST_HEAD(&wq->works);
    sem_init(&wq->sem, 0);
    spin_lock_init(&wq->lock);
    wq->thread = kthread_run(workqueue_thread, wq, "workqueue");
}

int queue_work(struct workqueue *wq, struct work *work)
{
    spin_lock(&wq->lock);
    list_add_tail(&work->list, &wq->works);
    sem_up(&wq->sem);
    spin_unlock(&wq->lock);
    return 0;
}