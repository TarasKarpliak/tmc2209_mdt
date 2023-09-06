# Application name
APP_NAME := TMC2209_MDT

# Compiler and linker
CC := arm-none-eabi-gcc
LD := arm-none-eabi-gcc

# Source files
SRC_FILES := \
	../../sources/main/main.c \
	../../sources/system/system_stm32f7xx.c \
	../../sources/chip/chip.c
	
INCLUDE_DIRS := \
	../../CMSIS/Device/ST/STM32F7xx/Include \
	../../CMSIS/Core/Include \
	../../sources \
	../../

# startup.s
STARTUP_FILE := startup_stm32f767xx.s

# Compiler flags
CFLAGS := -mcpu=cortex-m7 -mthumb -O2 -Wall $(addprefix -I,$(INCLUDE_DIRS))

# Linker flags
LDFLAGS := -T linker_script.ld --specs=nosys.specs

# Output directories
BUILD_DIR := ../../build_dir
OBJ_DIR   := $(BUILD_DIR)/obj

# Output binary
OUTPUT := $(BUILD_DIR)/$(APP_NAME).bin

OBJ_FILES := 

all: $(OUTPUT)

$(OUTPUT): $(BUILD_DIR)/$(APP_NAME).elf
	@echo "Generating binary file..."
	arm-none-eabi-objcopy -O binary $< $@

$(BUILD_DIR)/$(APP_NAME).elf: $(OBJ_DIR)/startup.o $(addprefix $(OBJ_DIR)/,$(subst /,_,$(SRC_FILES:.c=.o)))
	@echo "Linking..."
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/startup.o: $(STARTUP_FILE) | $(SRC_FILES)
	@echo "Compiling startup file ..."
	$(CC) $(CFLAGS) -c -o $@ $<
    
$(SRC_FILES): $(BUILD_DIR)
	@echo "Compiling source file $@ ..."
	$(CC) $(CFLAGS) -c -o $(addprefix $(OBJ_DIR)/,$(subst /,_,$(@:.c=.o))) $@

$(BUILD_DIR):
	mkdir -p $@
	mkdir -p $(OBJ_DIR)


clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
