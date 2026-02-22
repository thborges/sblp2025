MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
	RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

__reset_stack_pointer = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
	.isr_vector : {
		_start = .;
		LONG(__reset_stack_pointer);		
		LONG(__isr_reset | 1);
		LONG(__isr_nmi | 1);
		/* The interrupt table is 332 bytes long. Advance to that position. */
		. = ORIGIN(FLASH) + 332;
	} > FLASH

	.text : {
		. = ALIGN(4);
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata.*)
		. = ALIGN(4);
	} > FLASH

	.init_array : {
        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(.preinit_array*))
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*)) 
        __init_array_end = .;
    } > FLASH

    .data : {
		. = ALIGN(4);
		__data_start = .;
		*(*.data*)
        . = ALIGN(4);
		__data_end = .;
	} > RAM AT> FLASH

	__data_src_init = LOADADDR(.data);

	.bss ADDR(.data) + SIZEOF(.data) : AT (ADDR (.bss)) {
		. = ALIGN(4);
		__bss_start = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
	} > RAM

	__heap_start = LOADADDR(.data);
  
	.ARM.attributes 0 : { *(.ARM.attributes) }
}
