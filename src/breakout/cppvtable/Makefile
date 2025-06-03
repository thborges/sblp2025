#INCLUDES=-I~/.platformio/packages/toolchain-atmelavr@1.70300.191015
INCLUDES=-I /usr/local/Cellar/avr-gcc@9/9.4.0_1/avr/include
OPTS=-mmcu=atmega328p -DF_CPU=16000000 -isystem /usr/local/Cellar/avr-gcc@9/9.4.0_1/avr -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections
#OPTS+=--save-temps
#OPTS+=-g -O1

AVRDUDECFG=-C ~/.platformio/packages/tool-avrdude@1.60300.200527/avrdude.conf
AVRDUDEPORT=`ls /dev/cu.usb*`

SRCS = $(wildcard **/*.cpp *.cpp)
OBJS = $(patsubst %.cpp,out/%.o,$(SRCS))

all: $(OBJS)
	ld.lld -nostdlib -Bstatic --gc-sections --lto-whole-program-visibility --lto=thin -L../../common -Tatmega328p.ld -o main.elf $(OBJS) ../../common/avr5.o
	#avr-ld -nostdlib -Bstatic --gc-sections -L../../common -Tatmega328p.ld -o main.elf $(OBJS) ../../common/avr5.o
	avr-size main.elf
	#avr-objdump --no-addresses --demangle -D main.elf | python3 ../../common/sorts.py > main.S
	avr-objdump --no-addresses --demangle -D main.elf > main.S

out/%.o : %.cpp
	clang++ -Oz -target avr -funified-lto $(OPTS) $(INCLUDES) -c $< -o $@
	#avr-c++ -Os $(OPTS) $(INCLUDES) -c $< -o $@

%.hex : %.elf
	llvm-objcopy -Oihex $< $@

%.up: %.hex
	avrdude ${AVRDUDECFG} -v -V -c arduino -p m328p -b57600 -U flash:w:$<:i -P ${AVRDUDEPORT}

size: main.elf
	#avr-nm -t o -C -S --size-sort $<
	avr-readelf -WC -s main.elf|sed -E "s/ +/\t/g"

passes: $(SRCS)
	rm -f out/*.ll
	clang++ -Oz -target avr -S -emit-llvm $(OPTS) $(INCLUDES) $(SRCS)
	mv *.ll out/
	llvm-link out/*.ll -S -o main.ll

clean:
	rm -f $(OBJS)
	rm -f main.{hex,elf,S}
