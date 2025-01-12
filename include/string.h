#ifndef __STRING_H
#define __STRING_H

#include "types.h"

char * strrchr(const char * s, int c);
char * strchr(const char * s, int c);
size_t strlen(const char *s);
int strcmp(const char *cs, const char *ct);
int memcmp(const void *cs, const void *ct, size_t count);
void * memcpy(void *__dest, __const void *__src, size_t __n);
void * memmove(void *__dest, __const void *__src, size_t count);
void * memchr(const void *s, int c, size_t count);
void * memset(void *s, int c, size_t count);
void memzero(void *s, size_t count);

#endif
