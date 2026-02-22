
#include "../cppdeps.h"
#include "avr5_hardware.hpp"

int main();

extern "C"
void busy_wait_loop5(uint16_t count) {
	// trusting that ABI will put the count parameter in r25:r24
	// cycles total: 3 + count*5-1 + 3
	//   +2 (ldi r25:r24 prior to cal) + count*5 - 1 + 3
	//   +1 (ser)
	//   +count*5-1 (loop)
	//   +2 (rjmp)
	//   +1 (ret)
	asm ("ser r18\n"
		 "__busy_wait_loop5_loop: subi r18,1\n"
		 "	   sbci r24,0\n"
		 "	   sbci r25,0\n"
		 "	   brne __busy_wait_loop5_loop\n"
		 "rjmp __busy_wait_loop5_end\n"
		 "__busy_wait_loop5_end: "); // a ret is appended here
}

extern "C" __attribute__((naked))
void __do_copy_data() {
	asm ("ldi r17, hi8(__data_end)\n");
	asm ("ldi r26, lo8(__data_start)\n");
	asm ("ldi r27, hi8(__data_start)\n");
	asm ("ldi r30, lo8(__data_load_start)\n");
	asm ("ldi r31, hi8(__data_load_start)\n");
	asm ("rjmp __do_copy_data_start\n");
	asm ("__do_copy_data_loop:\n");
	asm (    	"lpm r0, Z+\n");
	asm (	    "st  X+, r0\n");
	asm (	"__do_copy_data_start:\n");
	asm (    	"cpi r26, lo8(__data_end)\n");
	asm (    	"cpc r27, r17\n");
	asm (    	"brne __do_copy_data_loop\n");
	asm (	"ret\n");
}

extern "C" __attribute__((naked)) __attribute__((used))
void __do_clear_bss() {
asm (
	"ldi r17, hi8(__bss_end)\n"
    "ldi r26, lo8(__bss_start)\n"
    "ldi r27, hi8(__bss_start)\n"
    "rjmp __do_clear_bss_start\n"
	"__do_clear_bss_loop:\n"
    	"st  X+, r1\n"
	"__do_clear_bss_start:\n"
    	"cpi r26, lo8(__bss_end)\n"
    	"cpc r27, r17\n"
    	"brne __do_clear_bss_loop\n"
	"ret\n");
}
 
extern "C" __attribute((used,naked))
void __delay_us() {
asm (
	"ldi r18, 5\n"
	"__delay_ms_loop:\n"
		"dec r18\n"
		"brne __delay_ms_loop\n"
	"nop\n"
	"ret\n");
}

/*volatile uint8_t* SREG = (volatile uint8_t*)0x5f;

uint32_t millis() {
	uint8_t sreg_bkp = *SREG;
	asm ("cli");
	result = timer0_millis;
	SREG = sreg_bkp;
	return result;
}*/

extern "C" __attribute((naked,section("__irqs")))
void __default_irq()  {
	asm ("jmp __default_irq");
}

extern "C" __attribute((naked,section("__isr")))
void __vectors() {

	 /* atmega328p has 26 interruptions */
	 asm("jmp __init");
	 asm("jmp __default_irq"); // IRQ0
	 asm("jmp __default_irq"); // IRQ1
	 asm("jmp __default_irq"); // PCINT0
	 asm("jmp __default_irq"); // PCINT1
	 asm("jmp __default_irq"); // PCINT2
	 asm("jmp __default_irq"); // Watchdog Timer
	 asm("jmp __default_irq"); // Timer2 Compare A
	 asm("jmp __default_irq"); // Timer2 Compare B
	 asm("jmp __default_irq"); // Timer2 Overflow
	 asm("jmp __default_irq"); // Timer1 Capture
	 asm("jmp __default_irq"); // Timer1 Compare A
	 asm("jmp __default_irq"); // Timer1 Compare B 
	 asm("jmp __default_irq"); // Timer1 Overflow
	 asm("jmp __default_irq"); // Timer0 Compare A
	 asm("jmp __default_irq"); // Timer0 Compare B
	 asm("jmp __default_irq"); // Timer0 Overflow
	 asm("jmp __default_irq"); // SPI Transfer Complete
	 asm("jmp __default_irq");  // USART, RX Complete
	 asm("jmp __default_irq"); // USART, UDR Empty
	 asm("jmp __default_irq");  // USART, TX Complete
	 asm("jmp __default_irq"); // ADC Conversion
	 asm("jmp __default_irq"); // EEPROM Ready
	 asm("jmp __default_irq"); // Analog Comparator
	 asm("jmp __default_irq"); // 2-wire Serial Interface
	 asm("jmp __default_irq"); // Store Program Memory Ready
	 
	 asm("__init:");
	 // clear r1 (zero reg)
	 asm("clr r1");
	 // clear sreg
	 asm("out 0x3f, r1");
	 // set stack at 0x08FF (0x100+2K-1)
	 asm("ldi r28, 0xFF");
	 asm("ldi r29, 0x08");
	 asm("out 0x3e, r29");
	 asm("out 0x3d, r28");
 
	 /* same code in .rob, but it produces a larger assembly code
	 *register int8 SREG at 0x3f;
	 register int16 STACK at 0x3d;
	 SREG = 0;
	 STACK = 0x08FF;*/
	 
	 // copy global vars init data
	 asm("call __do_copy_data");
 
	 // clear zeroed global vars 
	 asm("call __do_clear_bss");
	 
	 asm("call run_init_array");
	 asm("call main");
 
	 // if main returns, disable interruptions and sleep. 
	 // this is used to quit simavr in our tests
	 asm("cli");
	 asm("sleep");
 }

extern "C" {

	__attribute((naked)) void run_init_array(void) {
		asm (
	    	".extern __init_array_start\n"
			".extern __init_array_end\n"
			"ldi r26, lo8(__init_array_start)\n" // load start into X
    		"ldi r27, hi8(__init_array_start)\n"
    		"ldi r28, lo8(__init_array_end)\n" // load end into Y
    		"ldi r29, hi8(__init_array_end)\n"
			"loop_init_array:\n"
    		"	cp  r26, r28\n" // Z >= end?
    		"	cpc r27, r29\n"
    		"	brcc end_init_array\n"

			"   movw r30, r26\n" // put X into Z
    		"	lpm r24, Z+\n"   // load Z addr from flash into r24:r25
    		"	lpm r25, Z+\n"
			"   movw r26, r30\n" // save next Z into X
			"	movw r30, r24\n" // move r24:r25 (function pointer) to Z
    		"	icall\n"

    		"	rjmp loop_init_array\n"

			"end_init_array: ret\n" // ret added here
		);
	}

	/* same as above, one more instruction */
	/*static inline uint16_t pgm_read_word(const uint16_t *addr) {
		uint16_t result;
		asm volatile (
			"lpm %A0, Z+\n\t"
			"lpm %B0, Z"
			: "=r" (result)
			: "Z" (addr)
		);
		return result;
	}

	typedef void (*constructor_t)(void);

	#define PROGMEM  __attribute__((section(".progmem1.data")))
	extern constructor_t __init_array_start[] PROGMEM;
	extern constructor_t __init_array_end[] PROGMEM;

	__attribute((naked)) void run_init_array(void) {
		constructor_t *ctor = __init_array_start;

		while (ctor < __init_array_end) {
			// Read function pointer from program memory
			constructor_t func = (constructor_t)pgm_read_word((uint16_t*)ctor);
			func();
			ctor++;
		}
	}*/
}