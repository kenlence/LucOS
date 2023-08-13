ENTRY(reset)
SECTIONS
{
	/* 0x80000000 - 0xFFFFFFFF for ddr, totol 2GB */
	. = 0x80000000;
 	.startup . : { obj/start.o(.text) }
 	.text : { *(.text) }
    . = 0x80900000;
 	.data : { *(.data) }
 	.bss : { *(.bss COMMON) }
 	. = ALIGN(8);
 	. = . + 0x8000; /* 32kB of stack memory */
 	svc_stack_top = .;
}