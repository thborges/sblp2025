INCLUDES=-I~/.platformio/packages/toolchain-atmelavr@1.70300.191015
OPTS=-Os -mmcu=atmega328p -DF_CPU=16000000
#OPTS+=--save-temps
AVRDUDECFG=-C ~/.platformio/packages/tool-avrdude@1.60300.200527/avrdude.conf
AVRDUDEPORT=`ls /dev/cu.usb*`

all:
	robcmp -a atmega328p -o atmega328p.o -Oz -I . atmega328p.rob -bdep
	robcmp -a atmega328p -o main.o -Oz -I . main.rob -s avr.rob -bdep
	ld.lld -nostdlib -Bstatic --gc-sections --lto=thin --lto-whole-program-visibility -L../../common -Tatmega328p.ld -o main.elf atmega328p.o main.o
	avr-size main.elf
	avr-objdump --no-addresses -D main.elf | python3 ../../common/sorts.py > main.S

%.ll : %.rob FORCE
	robcmp -a atmega328p -Oz -I . -s avr.rob -bdep $< > $@

%.hex: %.elf
	llvm-objcopy -Oihex $< $@

%.up: %.hex
	avrdude ${AVRDUDECFG} -v -V -c arduino -p m328p -b57600 -U flash:w:$<:i -P ${AVRDUDEPORT}

clean:
	rm -f *.s *.ii *.o *.elf

FORCE: ;
