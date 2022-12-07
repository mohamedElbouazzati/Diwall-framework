TARGET := digilent_arty_cv32e41p
THIS_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
BUILD_DIR?=$(THIS_DIR)/../../build/$(TARGET)

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

BASE_DIR = .
OBJ_DIR = $(BASE_DIR)/obj
TOOLS_DIR = $(BASE_DIR)/tools
LORA_DIR = $(BASE_DIR)/lora
BOARDS_DIR = $(LORA_DIR)/boards
RADIO_DIR = $(LORA_DIR)/radio
SYSTEM_DIR = $(LORA_DIR)/system
THIRD_PARTY = $(BASE_DIR)/third_party
LD_DIR = $(BASE_DIR)/ld


CSOURCES   :=  $(wildcard $(TOOLS_DIR)/*.c)  $(wildcard $(BASE_DIR)/*.c) $(wildcard $(SYSTEM_DIR)/*.c)  $(wildcard $(RADIO_DIR)/*.c)  $(wildcard $(BOARDS_DIR)/*.c)
ASOURCES   :=  $(wildcard $(CPU_DIRECTORY)/crt0.S)
COBJS      := $(addprefix $(OBJ_DIR)/, $(notdir $(CSOURCES:.c=.o)))
AOBJS      := $(addprefix $(OBJ_DIR)/, $(notdir $(ASOURCES:.S=.o)))

LOCAL_INCLUDE = -I$(TOOLS_DIR)  -I$(BOARDS_DIR)  -I$(RADIO_DIR)  -I$(SYSTEM_DIR) 
L_INCLUDE = -L$(TOOLS_DIR)  -L$(BOARDS_DIR)  -L$(RADIO_DIR)  -L$(SYSTEM_DIR) 

c_flags = $(LOCAL_INCLUDE) $(CFLAGS)
l_flags = $(L_INCLUDE) $(LOCAL_INCLUDE) $(LDFLAGS)

define compile
$(CC) -c $(c_flags) $(1) $< -o $@
endef


OBJECTS = $(COBJS) $(AOBJS)
OTHER_OBJECTS   = donut.o helloc.o crt0.o testspi.o iotest.o main.o test1.o

all: demo.bin

$(OBJECTS): | $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	chmod -x $@

demo.elf: $(OBJECTS)
	$(CC) $(l_flags) -T $(LD_DIR)/linker.ld -N -o $@ \
		$(OBJECTS) \
		$(PACKAGES:%=-L$(BUILD_DIR)/software/%) \
		-Wl,--gc-sections \
		-Wl,-Map,$@.map \
		$(LIBS:lib%=-l%)
	chmod -x $@

$(OBJ_DIR)/main.o: main.c
	$(compile)

$(OBJ_DIR)/%.o: %.c
	$(compile)

$(OBJ_DIR)/crt0.o: $(CPU_DIRECTORY)/crt0.S
	$(assemble)

$(OBJ_DIR)/%.o: $(TOOLS_DIR)/%.c 
	$(compile)
$(OBJ_DIR)/%.o: $(SYSTEM_DIR)/%.c 
	$(compile)
$(OBJ_DIR)/%.o: $(RADIO_DIR)/%.c 
	$(compile)
$(OBJ_DIR)/%.o: $(BOARDS_DIR)/%.c 
	$(compile)
%.o: %.cpp
	$(compilexx)

%.o: %.S
	$(assemble)
prog:demo.bin
	sudo chmod -R 777 /dev/ttyUSB*
	litex_term --kernel=demo.bin /dev/ttyUSB*

clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d) demo.elf demo.bin .*~ *~
	$(RM) -r $(OBJ_DIR)

.PHONY: all $(OBJ_DIR)/main.o clean screen
