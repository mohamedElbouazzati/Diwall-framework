BUILD_DIR?=/home/tanguy/travail/recherche/supervisions/stages/stage-Christophe_TERRINE/repo/build/poto_digilent_basys3

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

OBJECTS   = donut.o helloc.o crt0.o testspi.o iotest.o \
			tools/binary.o tools/delai.o tools/libspi.o tools/console.o\
			main.o
#lora/system/delay.o lora/system/fifo.o lora/system/gpio.o lora/system/timer.o \
#lora/boards/board.o lora/boards/spi-board.o lora/boards/gpio-board.o lora/boards/sx1276-board.o \
#lora/radio/sx1276.o \  test1.o

			
#lora/sx1276mb1las-board.o  
ifdef WITH_CXX
	OBJECTS += hellocpp.o
endif


all: demo.bin


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
ifneq ($(OS),Windows_NT)
	chmod -x $@
endif

vpath %.a $(PACKAGES:%=../%)

demo.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -T linker.ld -N -o $@ \
		$(OBJECTS) \
		$(PACKAGES:%=-L$(BUILD_DIR)/software/%) \
		-Wl,--whole-archive \
		-Wl,--gc-sections \
		-Wl,-Map,$@.map \
		$(LIBS:lib%=-l%)

ifneq ($(OS),Windows_NT)
	chmod -x $@
endif

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

donut.o: CFLAGS   += -w

VPATH = $(BIOS_DIRECTORY):$(BIOS_DIRECTORY)/cmds:$(CPU_DIRECTORY)


%.o: %.cpp
	$(compilexx)

%.o: %.c
	$(compile)

%.o: %.S
	$(assemble)

clean:
	$(RM) $(OBJECTS) demo.elf demo.bin .*~ *~ *.d ./lora/*/*~ ./lora/*/*.d ./tools/*.d

.PHONY: all clean
