SECTIONS{
	. = 0X80800000;
	.text :
	{
		obj/start.o 
		*(.text)
	}
	.rodata ALIGN(4) : {*(.rodata*)}     
	.data ALIGN(4)   : { *(.data) }    
	__bss_start = .;    
	.bss ALIGN(4)  : { *(.bss)  *(COMMON) }    
	__bss_end = .;
	.inittask ALIGN(((1 << 12) << 1)): {*(.data..init_task)}
	. = ALIGN((1 << 12));
	memory_pool = .;
	memory_pool_end = 0xA0000000;
}