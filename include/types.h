#ifndef _LUCOS_TYPES_H
#define _LUCOS_TYPES_H

typedef unsigned long size_t;

typedef int	ssize_t;

struct list_head {
	struct list_head *next, *prev;
};

#endif