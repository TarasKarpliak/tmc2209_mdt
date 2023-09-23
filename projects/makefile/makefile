# Application name
APP_NAME := TMC2209_MDT

# Compiler and linker
CC := arm-none-eabi-gcc
LD := arm-none-eabi-g++

# Source files
SRC_FILES := \
	../../sources/main/main.c \
	../../sources/system/system_stm32f7xx.c \
	../../sources/chip/chip.c \
	../../sources/led_ctrl/led_ctrl.c \
	../../sources/state_machine/state_machine.c \
	../../sources/common_def/common_def.c \
	../../sources/usart/usart.c \
	
INCLUDE_DIRS := \
	../../CMSIS/Device/ST/STM32F7xx/Include \
	../../CMSIS/Core/Include \
	../../sources \
	../../

# startup.s
STARTUP_FILE := startup_stm32f767xx.s

# Output directories
BUILD_DIR := ../../build_dir
OBJ_DIR   := $(BUILD_DIR)/obj

# Compiler flags
CFLAGS := -mcpu=cortex-m7 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 \
	-fsigned-char -ffunction-sections -fdata-sections -ffreestanding \
	-fno-move-loop-invariants -Wall -Wextra -g3 -DDEBUG -DSTM32F767xx -std=gnu11 -MMD -MP \
	$(addprefix -I,$(INCLUDE_DIRS))

# Linker flags
LDFLAGS := -mcpu=cortex-m7 -mthumb -mfloat-abi=soft -T linker_script.ld --specs=nosys.specs -fno-move-loop-invariants \
	-Wall -Wextra -g3 -Xlinker --gc-sections -Wl,-Map,"$(BUILD_DIR)/$(APP_NAME).map"
	
# Output binary
OUTPUT := $(BUILD_DIR)/$(APP_NAME).bin


all: $(OUTPUT)

$(OUTPUT): $(BUILD_DIR)/$(APP_NAME).elf
	@echo "Generating binary file..."
	arm-none-eabi-objcopy -O binary $< $@

$(BUILD_DIR)/$(APP_NAME).elf: $(OBJ_DIR)/startup.o $(addprefix $(OBJ_DIR)/,$(subst /,_,$(SRC_FILES:.c=.o)))
	@echo "Linking..."
	$(LD) $(LDFLAGS) -o $@ $^
	@echo ""

$(OBJ_DIR)/startup.o: $(STARTUP_FILE) | $(SRC_FILES)
	@echo "Compiling startup file ..."
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo ""
    
$(SRC_FILES): $(BUILD_DIR)
	@echo "Compiling source file $@ ..."
	$(CC) $(CFLAGS) -c -o $(addprefix $(OBJ_DIR)/,$(subst /,_,$(@:.c=.o))) $@
	@echo ""

$(BUILD_DIR): clean
	mkdir -p $@
	mkdir -p $(OBJ_DIR)
	@echo ""


clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo ""

.PHONY: all clean
