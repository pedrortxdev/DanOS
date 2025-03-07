CC = gcc
LD = ld
NASM = nasm
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc -I.
LDFLAGS = -m elf_i386 -T linker.ld
OBJS = kernel.o src/print.o src/keyboard.o src/shell.o src/commands.o src/system.o src/string.o src/stdio.o src/memory.o boot.o

all: kernel.bin

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(NASM) -f elf32 $< -o $@

clean:
	rm -f $(OBJS) kernel.bin

.PHONY: all clean 