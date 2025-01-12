#ifndef __LUCOS_GFP_H
#define __LUCOS_GFP_H

#define GFP_MAX_ORDER	12

void gfp_init();

void *alloc_pages(unsigned int order);

void free_pages(unsigned long addr, unsigned int order);

#endif