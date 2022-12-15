BUILD_DIR?=../../build/digilent_arty_lora

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

BASE_DIR = .
LD_DIR = $(BASE_DIR)
OBJ_DIR = $(BASE_DIR)/obj
THIRD_PARTY = $(BASE_DIR)/third_party
LORA_DIR = $(BASE_DIR)/loramacnode
BOARDS_DIR = $(LORA_DIR)/boards
LITEX_LIB_DIR = $(BOARDS_DIR)/LitexLib
RADIO_DIR = $(LORA_DIR)/radio
SYSTEM_DIR = $(LORA_DIR)/system
PERIPHERALS =$(LORA_DIR)/peripherals/soft-se
MAC = $(LORA_DIR)/mac
REGION = $(MAC)/region
APPS_DIR = $(LORA_DIR)/apps
LORA_MAC =$(APPS_DIR)/LoRaMac
COMMON = $(LORA_MAC)/common
LMHANDLER = $(COMMON)/LmHandler
PACKAGES_MAC = $(LMHANDLER)/packages
FUOTA=$(LORA_MAC)/fuota-test-01/code
PERIODIC=$(LORA_MAC)/periodic-uplink-lpp/code

CSOURCES   := $(wildcard $(LITEX_LIB_DIR)/*.c) $(wildcard $(BASE_DIR)/*.c) \
$(wildcard $(SYSTEM_DIR)/*.c)  $(wildcard $(RADIO_DIR)/*.c)  $(wildcard $(BOARDS_DIR)/*.c) \
$(wildcard $(MAC)/*.c)  $(wildcard $(REGION)/*.c)  $(wildcard $(COMMON)/*.c) $(wildcard $(PERIODIC)/*.c)	\
$(wildcard $(LMHANDLER)/*.c)  $(wildcard $(PACKAGES_MAC)/*.c)  $(wildcard $(FUOTA)/*.c) $(wildcard $(PERIPHERALS)/*.c)		\

DEPS       := $(wildcard $(BASE_DIR)/*.h) $(wildcard $(SYSTEM_DIR)/*.h)  $(wildcard $(RADIO_DIR)/*.h)  $(wildcard $(BOARDS_DIR)/*.h) \
$(wildcard $(MAC)/*.h)  $(wildcard $(REGION)/*.h)  $(wildcard $(COMMON)/*.h) $(wildcard $(PERIODIC)/*.h)	\
$(wildcard $(LMHANDLER)/*.h)  $(wildcard $(PACKAGES_MAC)/*.h)  $(wildcard $(FUOTA)/*.h) $(wildcard $(PERIPHERALS)/*.h)	\

ASOURCES   :=  $(wildcard $(CPU_DIRECTORY)/crt0.S)
COBJS      := $(addprefix $(OBJ_DIR)/, $(notdir $(CSOURCES:.c=.o)))
AOBJS      := $(addprefix $(OBJ_DIR)/, $(notdir $(ASOURCES:.S=.o)))

LOCAL_INCLUDE = -I$(BASE_DIR)  -I$(LITEX_LIB_DIR) -I$(BOARDS_DIR) -I$(RADIO_DIR) -I$(SYSTEM_DIR) \
				 -I$(MAC)  -I$(REGION)  -I$(COMMON)  -I$(PERIODIC)  -I$(LMHANDLER)  -I$(PACKAGES_MAC) \
				 -I$(FUOTA)   -I$(PERIPHERALS)\
				 


L_INCLUDE = -L$(LITEX_LIB_DIR)  -L$(BOARDS_DIR)  -L$(RADIO_DIR)  -L$(SYSTEM_DIR) \
	-L$(MAC)  -L$(REGION)  -L$(COMMON)  -L$(PERIODIC)  -L$(LMHANDLER)  -L$(PACKAGES_MAC) \
				 -L$(FUOTA) -L$(PERIPHERALS)	\

# add software secure element  SOFT-SE and Europe REGION on 868, desactivate other region to avoid compile errors
c_flags = $(LOCAL_INCLUDE) $(CFLAGS) -DSOFT_SE -DREGION_EU868=1 -DREGION_KR920=0 -DREGION_AU915=0 -DREGION_CN470=0

l_flags = $(L_INCLUDE) $(LOCAL_INCLUDE) $(LDFLAGS)

define compile 
	$(CC) -c $(c_flags) $(1) $< -o $@ 
endef


OBJECTS = $(COBJS) $(AOBJS)
OTHER_OBJECTS   =  helloc.o crt0.o main.o #apps/LoRaMac/periodic-uplink-lpp/code/periodic_uplink.o


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
	

$(OBJ_DIR)/main.o: main.c $(DEPS)
	$(compile)

$(OBJ_DIR)/%.o: %.c $(DEPS)
	$(compile)

$(OBJ_DIR)/crt0.o: $(CPU_DIRECTORY)/crt0.S
	$(assemble)

$(OBJ_DIR)/%.o: $(LITEX_LIB_DIR)/%.c  $(DEPS)
	$(compile)
$(OBJ_DIR)/%.o: $(SYSTEM_DIR)/%.c $(DEPS)
	$(compile)
$(OBJ_DIR)/%.o: $(RADIO_DIR)/%.c $(DEPS)
	$(compile)
$(OBJ_DIR)/%.o: $(BOARDS_DIR)/%.c $(DEPS)
	$(compile)
$(OBJ_DIR)/%.o: $(MAC)/%.c $(DEPS)
	$(compile)
$(OBJ_DIR)/%.o: $(REGION)/%.c $(DEPS)
	$(compile)
$(OBJ_DIR)/%.o: $(LMHANDLER)/%.c $(DEPS)
	$(compile)
$(OBJ_DIR)/%.o: $(COMMON)/%.c $(DEPS)
	$(compile)
$(OBJ_DIR)/%.o: $(PACKAGES_MAC)/%.c $(DEPS)
	$(compile)
$(OBJ_DIR)/%.o: $(FUOTA)/%.c $(DEPS)
	$(compile)
$(OBJ_DIR)/%.o: $(PERIODIC)/%.c $(DEPS)
	$(compile)	
$(OBJ_DIR)/%.o: $(PERIPHERALS)/%.c $(DEPS)
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
