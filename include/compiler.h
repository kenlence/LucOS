#ifndef __COMPILER_H
#define __COMPILER_H

#define barrier() __asm__ __volatile__("": : :"memory")

#endif