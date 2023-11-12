#ifndef _LINUX_STDDEF_H
#define _LINUX_STDDEF_H

#undef NULL
#define NULL ((void *)0)

enum {
	false	= 0,
	true	= 1
};

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
