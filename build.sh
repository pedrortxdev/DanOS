#!/bin/bash

mkdir -p bin

# Compila o código
nasm -f elf32 boot.asm -o boot.o
gcc -m32 -fno-stack-protector -c kernel.c -o kernel.o
gcc -m32 -fno-stack-protector -c ./src/print.c -o ./src/print.o
gcc -m32 -fno-stack-protector -c ./src/commands.c -o ./src/commands.o
ld -m elf_i386 -T link.ld -o ./bin/kernel.bin boot.o kernel.o ./src/print.o ./src/commands.o

# Executa o QEMU com opções otimizadas para inicialização rápida
qemu-system-x86_64 -kernel ./bin/kernel.bin -nographic -no-reboot -machine acpi=off -boot d -cpu qemu64 -smp 1 -m 64
