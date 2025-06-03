#INCLUDES=-I~/.platformio/packages/toolchain-atmelavr@1.70300.191015
#OPTS=-Os -mmcu=atmega328p -DF_CPU=16000000 -ffunction-sections -fdata-sections
#OPTS+=--save-temps
INCLUDES=-I /usr/local/Cellar/avr-gcc@9/9.4.0_1/avr/include
OPTS=-mmcu=atmega328p -DF_CPU=16000000 -isystem /usr/local/Cellar/avr-gcc@9/9.4.0_1/avr -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections

AVRDUDECFG=-C ~/.platformio/packages/tool-avrdude@1.60300.200527/avrdude.conf
AVRDUDEPORT=`ls /dev/cu.usb*`

all: avr5_hardware.o avr5_mcu.o main.o
	ld.lld -nostdlib -Bstatic --gc-sections --lto-whole-program-visibility --lto=thin -L../../common -Tatmega328p.ld -o main.elf avr5_mcu.o main.o avr5_hardware.o 
	avr-size main.elf
	avr-objdump --no-addresses --demangle -D main.elf | python3 ../../common/sorts.py > main.S

%.o : %.cpp
	clang++ -Oz -target avr $(OPTS) $(INCLUDES) -c $<
	#avr-c++ -Os $(OPTS) $(INCLUDES) -c $<

%.hex : %.elf
	llvm-objcopy -Oihex $< $@

%.up: %.hex
	avrdude ${AVRDUDECFG} -v -V -c arduino -p m328p -b57600 -U flash:w:$<:i -P ${AVRDUDEPORT}

clean:
	rm -f *.s *.ii *.o *.elf
