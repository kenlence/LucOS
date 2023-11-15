#ifndef __LINUX_GFP_H
#define __LINUX_GFP_H


void *__get_free_pages(unsigned int order);

void free_pages(unsigned long addr, unsigned int order);

#define __get_free_page(gfp_mask) \
		__get_free_pages((gfp_mask), 0)

#define __free_page(page) __free_pages((page), 0)

#endif