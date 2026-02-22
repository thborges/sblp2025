/*
 * Robcmp interruptions vector for stm32f1
 */

#include "stm32f1_hardware.hpp"
#include "../../../common/stm32f1_regs.hpp"

int main();

extern "C"
void __set_system_clock_to_25Mhz() {
	// Enable HSE and waits it to settle down
	RCC_CR->hse_on = true;
	while (!RCC_CR->hse_rdy);
	// Set HSE as the system clock
	RCC_CFGR->sw = 0x01;
}

extern "C"
void __system_clock_config() {
    //__set_system_clock_to_25Mhz();
    *SYSTICK_LOAD = 0xFFFFFF; //24 bits 
	SYSTICK_CTRL->enable = true;
	SYSTICK_CTRL->clksource = true; // processor clock
}

extern "C"
void __error_handler() {
    while (1) {
        asm ("wfi");
    }
}

extern "C"
void memset() {
// reference signature: 
// void *memset(void *dst, int val, size_t count)
//                   r0 ↑    r1 ↑          r2 ↑
asm
   ("cmp		r2, #0\n"
    "it		eq\n"
	"bxeq	lr\n"					/* return if r2/"count==0 */
	"mov		r12, r0\n"
	"tst		r0, #3\n"				/* is 4-bytes aligned? */
	"beq		__memset_aligned\n"
    "\n"
	"__memset_unaligned:\n"
	"	strb	r1, [r12], #1\n"
	"	subs	r2, r2, #1\n"
	"	it		eq\n"
	"	bxeq	lr\n"				/* return if r2/count==0 */
	"	tst		r12, #13\n"		/* is 4-bytes aligned now? */
	"	bne		__memset_unaligned\n"
    "\n"
	"__memset_aligned:\n"
	"	bfi		r1, r1, #8, #8\n"
	"	bfi		r1, r1, #16, #16\n"
	"	mov		r3, r1\n"
	"	cmp		r2, #16\n"
	"	blo		__memset_less16\n"
	"	push	{r4, lr}\n"
	"	mov		r4, r1\n"
	"	mov		lr, r1\n"
	"	cmp		r2, #32\n"
	"	blo		__memset_less32\n"
    "\n"
	"__memset_write32:\n"
	"	stmia	r12!, {r1, r3, r4, lr}\n"	/* write 32 bytes */
	"	stmia	r12!, {r1, r3, r4, lr}\n"
	"	subs	r2, r2, #32\n"
	"	it		eq\n"
	"	popeq	{r4, pc}\n"
	"	cmp		r2, #32\n"
	"	bhs		__memset_write32\n"
    "\n"
	"__memset_less32:\n"
	"	cmp		r2, #16\n"
	"	it		hs\n"
	"	stmiahs	r12!, {r1, r3, r4, lr}\n"
	"	it		eq\n"
	"	popeq	{r4, pc}\n"
	"	pop		{r4, lr}\n"
    "\n"
	"__memset_less16:\n"
	"	lsls	r2, r2, #29\n"
	"	it		cs\n"
	"	stmiacs	r12!, {r1, r3}\n"
	"	it		eq\n"
	"	bxeq	lr\n"
	"	it		mi\n"
	"	strmi	r1, [r12], #4\n"
	"	lsls	r2, r2, #1\n"
	"	it		mi\n"
	"	strhmi	r1, [r12], #2\n"
	"	it		ne\n"
	"	strbne	r1, [r12]\n"
   );
}

extern "C"
void __attribute__((naked)) __do_copy_data() {
	asm volatile (
        // Copy .data from Flash to RAM
        "ldr r0, =__data_src_init\n"   // Source (Flash)
        "ldr r1, =__data_start\n"  // Destination (RAM)
        "ldr r2, =__data_end\n"    // End of RAM data
        "b copy_check\n"

		"copy_loop:\n"
        	"ldr r3, [r0], #4\n"       // Load from Flash, increment r0
        	"str r3, [r1], #4\n"       // Store to RAM, increment r1
		"copy_check:\n"
        	"cmp r1, r2\n"             // Have we reached the end of RAM data?
	        "bcc copy_loop\n"

		// Zero out .bss in RAM
        "ldr r1, =__bss_start\n"
        "ldr r2, =__bss_end\n"
        "movs r3, #0\n"            // Value to write (zero)
        "b bss_check\n"

		"bss_loop:\n"
        	"str r3, [r1], #4\n"       // Zero out RAM, increment r1

		"bss_check:\n"
	        "cmp r1, r2\n"
    	    "bcc bss_loop\n"
		
		"bx lr\n"
	);
}

extern "C"
void __run_init_array() {
	extern void (*__init_array_start []) (void) __attribute__((weak));
	extern void (*__init_array_end []) (void) __attribute__((weak));

	size_t count = __init_array_end - __init_array_start;
	for (size_t i = 0; i < count; i++)
		__init_array_start[i]();
}

/*extern "C"
void __attribute__((naked)) __old_run_init_array() {
	asm volatile (
        "push {r4, r5, lr}\n"              // Save callee-saved regs and link register
        "ldr r4, =__init_array_start\n"    // Use r4 for start
        "ldr r5, =__init_array_end\n"      // Use r5 for end

        "init_loop:\n"
            "cmp r4, r5\n"                 // Check if we hit the end
            "bhs init_done\n"              // branch if Higher or Same (standard for end pointers)
            
            "ldr r0, [r4], #4\n"           // Fetch func ptr into r0, increment r4
            "blx r0\n"                     // Call the constructor
            "b init_loop\n"

        "init_done:\n"
        	"pop {r4, r5, pc}\n"           // Restore regs and return by popping lr into pc
    );
}*/


extern "C" __attribute__((used, noinline))
void __isr_reset() {
    
	__do_copy_data();
	__run_init_array();

    __system_clock_config();
	
    main();
 
	__error_handler();
}

extern "C" __attribute__((used, noinline))
void __isr_nmi() {
    asm("nop");
}
