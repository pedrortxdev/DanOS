# Compilador e flags
CC = gcc
AS = nasm
LD = ld

# Flags do compilador
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc
CFLAGS += -I.
CFLAGS += -Wall -Wextra

# Flags do linker
LDFLAGS = -m elf_i386 -T linker.ld

# Arquivos fonte
SRCS = kernel.c \
       src/print.c \
       src/keyboard.c \
       src/shell.c \
       src/commands.c \
       src/system.c \
       src/string.c \
       src/stdio.c \
       src/memory.c

# Arquivos objeto
OBJS = $(SRCS:.c=.o)
OBJS += boot.o

# Regras de compilação
all: kernel.bin

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot.o: boot.asm
	$(AS) -f elf32 $< -o $@

clean:
	rm -f $(OBJS) kernel.bin 