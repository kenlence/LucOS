#include "sched.h"

#define __init_task_data __attribute__((__section__(".data..init_task")))

static struct thread init_thread __init_task_data = {

};





