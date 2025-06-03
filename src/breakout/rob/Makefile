INCLUDES=-I~/.platformio/packages/toolchain-atmelavr@1.70300.191015
OPTS=-Os -mmcu=atmega328p -DF_CPU=16000000
#OPTS+=--save-temps
AVRDUDECFG=-C ~/.platformio/packages/tool-avrdude@1.60300.200527/avrdude.conf
AVRDUDEPORT=`ls /dev/cu.usb*`

all:
	robcmp -a atmega328p -o out/atmega328p.o -Oz -I . mcu/atmega328p.rob -bdep
	robcmp -a atmega328p -o out/main.o -Oz -I . main.rob -s avr.rob -bdep
	ld.lld -nostdlib -Bstatic --gc-sections --lto=thin --lto-whole-program-visibility -L../../common -Tatmega328p.ld -o main.elf out/*.o ../../common/avr5.o
	avr-size main.elf
	avr-objdump --no-addresses -D main.elf | python3 ../../common/sorts.py > main.S

%.ll : %.rob FORCE
	robcmp -a atmega328p -Oz -I . -s avr.rob -bdep $< > $@

%.hex: %.elf
	llvm-objcopy -Oihex $< $@

%.up: %.hex
	avrdude ${AVRDUDECFG} -v -V -c arduino -p m328p -b57600 -U flash:w:$<:i -P ${AVRDUDEPORT}

size: main.elf
	#avr-nm -t o -C -S --size-sort $<
	avr-readelf -WC -s main.elf|sed -E "s/ +/\t/g"

passes:
	robcmp -a atmega328p -Oz -I . main.rob -s avr.rob -bdep > main.ll
	opt -Oz -stats -disable-output main.ll

clean:
	rm -f *.s *.ii *.o *.elf

FORCE: ;
