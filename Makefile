CC = gcc
AS = gcc
AR = ar
LD = ld
VERSION := $(shell cat VERSION)

# Общие флаги. -fPIC нужен и для .so, и для безопасной линковки в PIE-бинари.
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib \
         -Iinclude -Wall -Wextra -DCACTLIB_VERSION=\"$(VERSION)\"
CFLAGS_PIC = -m32 -ffreestanding -fPIC -fno-stack-protector -nostdlib \
             -Iinclude -Wall -Wextra -DCACTLIB_VERSION=\"$(VERSION)\"
ASFLAGS = -m32 -ffreestanding -fno-pie -nostdlib

SRC_DIR = src
BUILD_DIR = build
PIC_DIR   = $(BUILD_DIR)/pic
INCLUDE_DIR = include

C_SRCS = $(wildcard $(SRC_DIR)/*.c)
S_SRCS = $(wildcard $(SRC_DIR)/*.S)
C_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRCS))
S_OBJS = $(patsubst $(SRC_DIR)/%.S, $(BUILD_DIR)/%.o, $(S_SRCS))
OBJS = $(C_OBJS) $(S_OBJS)

# Для .so собираем все .c заново как PIC. start.S в .so не нужен — это crt0,
# линкуется только в исполняемые бинари (но PIC-вариант start.o используется
# при линковке PIE-бинарей против libc.so).
C_PIC_OBJS = $(patsubst $(SRC_DIR)/%.c, $(PIC_DIR)/%.o, $(C_SRCS))
START_O_PIC = $(PIC_DIR)/start.o

TARGET_A  = libc.a
TARGET_SO = libc.so

all: $(TARGET_A) $(TARGET_SO) $(START_O_PIC)

# ── статический архив (как раньше) ────────────────────────────────────────
$(TARGET_A): $(OBJS)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) -c $< -o $@

# ── shared object (ET_DYN, фиксированная база 0x10000000) ─────────────────
$(PIC_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(PIC_DIR)
	$(CC) $(CFLAGS_PIC) -c $< -o $@

# start.S must be assembled as PIC so _GLOBAL_OFFSET_TABLE_ relocations resolve.
$(PIC_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(PIC_DIR)
	$(CC) $(CFLAGS_PIC) -c $< -o $@

$(TARGET_SO): $(C_PIC_OBJS) libc.ld
	$(LD) -m elf_i386 -shared -nostdlib \
	      --hash-style=both -soname=libc.so \
	      -T libc.ld -o $@ $(C_PIC_OBJS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET_A) $(TARGET_SO)

version:
	@echo "CactLib version: $(VERSION)"

.PHONY: all clean version
