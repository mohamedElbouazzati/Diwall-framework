BUILD_DIR?=/home/med/Desktop/workflow/build/digilent_arty_cv32e41p

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

BASE_DIR = .
OBJ_DIR = $(BASE_DIR)/obj
LITEX_LIB_DIR = $(BASE_DIR)/LitexLib
LD_DIR = $(LITEX_LIB_DIR)/ld


CSOURCES   :=  $(wildcard $(LITEX_LIB_DIR)/*.c)  $(wildcard $(BASE_DIR)/*.c)  
ASOURCES   :=  $(wildcard $(CPU_DIRECTORY)/crt0.S)
COBJS      := $(addprefix $(OBJ_DIR)/, $(notdir $(CSOURCES:.c=.o)))
AOBJS      := $(addprefix $(OBJ_DIR)/, $(notdir $(ASOURCES:.S=.o)))

LOCAL_INCLUDE = -I$(LITEX_LIB_DIR)  
L_INCLUDE = -L$(LITEX_LIB_DIR) 

c_flags = $(LOCAL_INCLUDE) $(CFLAGS)
l_flags = $(L_INCLUDE) $(LOCAL_INCLUDE) $(LDFLAGS)

define compile
$(CC) -c $(c_flags) $(1) $< -o $@
endef


OBJECTS = $(COBJS) $(AOBJS)
OTHER_OBJECTS   = crt0.o helloc.o main.o 

all: demo.bin

$(OBJECTS): | $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

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

$(OBJ_DIR)/%.o: $(LITEX_LIB_DIR)/%.c 
	$(compile)
%.o: %.cpp
	$(compilexx)

%.o: %.S
	$(assemble)

prog:demo.bin
	sudo chmod -R 777 /dev/ttyUSB*
	litex_term --kernel=demo.bin /dev/ttyUSB*

clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d) demo.elf demo.bin demo.elf.map .*~ *~
	$(RM) -r $(OBJ_DIR)

.PHONY: all $(OBJ_DIR)/main.o clean screen
