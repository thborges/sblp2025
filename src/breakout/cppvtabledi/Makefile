OPTS=-Oz -nostdlibinc -nostdlib -mmcu=atmega328p -DF_CPU=16000000 -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections -fuse-init-array
#OPTS+=--save-temps
OPTS+=-gdwarf-4 -O1

AVRDUDECFG=-C ~/.platformio/packages/tool-avrdude@1.60300.200527/avrdude.conf
AVRDUDEPORT=`ls /dev/cu.usb*`

SRCS = $(wildcard **/*.cpp *.cpp)
OBJS = $(patsubst %.cpp,out/%.o,$(SRCS))

all: $(OBJS)
	ld.lld -nostdlib -Bstatic --gc-sections --lto-whole-program-visibility --lto=thin -L../../common -Tatmega328p.ld -o main.elf $(OBJS) ../../common/avr5.o
	@#avr-ld -nostdlib -Bstatic --gc-sections -L../../common -Tatmega328p.ld -o main.elf $(OBJS) ../../common/avr5.o
	avr-size main.elf
	@#avr-objdump --no-addresses --demangle -D main.elf > main.S
	avr-objdump --demangle -D main.elf > main.S

out/%.o : %.cpp
	clang++ -target avr $(OPTS) -c $< -o $@
	@#avr-c++ -Os $(OPTS) -c $< -o $@

%.hex : %.elf
	llvm-objcopy -Oihex $< $@

%.up: %.hex
	avrdude ${AVRDUDECFG} -v -V -c arduino -p m328p -b57600 -U flash:w:$<:i -P ${AVRDUDEPORT}

size: main.elf
	@#avr-nm -t o -C -S --size-sort $<
	avr-readelf -WC -s main.elf|sed -E "s/ +/\t/g"

passes: $(SRCS)
	rm -f out/*.ll
	clang++ -flto -target avr -S -emit-llvm $(OPTS) $(SRCS)
	mv *.ll out/
	llvm-link out/*.ll -S -o main.ll

test:
	/usr/bin/time robcmp-simavr-debug -f main.elf -c 16000000 -m atmega328p -hw ssd1306 &> log.txt
	@tail -n 1 log.txt
	@fgrep -o - log.txt | wc -l

clean:
	rm -f $(OBJS)
	rm -f *.{ii,s,ll}
	rm -f main.{hex,elf,S}
