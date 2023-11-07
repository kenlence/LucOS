#include "sched.h"

#define __init_task_data __attribute__((__section__(".data..init_task")))

struct task init_task;

union thread_union init_thread_union __init_task_data = {
    {.task = &init_task,}
};

struct task init_task = {
    .state = 0,
    .stack = &(init_thread_union.thread_info),
    .pid = 0,
};






