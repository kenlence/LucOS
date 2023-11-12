#ifndef __ASM_LINKAGE_H
#define __ASM_LINKAGE_H

#define __ALIGN .align 0
#define __ALIGN_STR ".align 0"

#define ALIGN __ALIGN
#define ASM_NL		 ;

#define END(name) \
	.size name, .-name

#define ENDPROC(name) \
  .type name, %function; \
  END(name)

#define ENTRY(name) \
	.globl name ASM_NL \
	ALIGN ASM_NL \
	name:

#endif