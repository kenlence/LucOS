#ifndef _LUCOS_TYPES_H
#define _LUCOS_TYPES_H

typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned long     long uint64_t;
typedef	  signed char  	 	   s8;
typedef	  signed short 	  int  s16;
typedef	  signed int 		   s32;
typedef	  signed long long int s64;
typedef	unsigned char 		   u8;
typedef	unsigned short int     u16;
typedef	unsigned int 		   u32;
typedef	unsigned long long int u64;

typedef unsigned long size_t;

typedef int	ssize_t;

struct list_head {
	struct list_head *next, *prev;
};

#endif