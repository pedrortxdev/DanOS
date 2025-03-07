#!/bin/bash

# Verifica se o kernel já foi compilado
if [ ! -f ./bin/kernel.bin ]; then
    echo "Kernel não encontrado. Compilando primeiro..."
    
    mkdir -p bin
    
    # Compila o código
    nasm -f elf32 boot.asm -o boot.o
    gcc -m32 -fno-stack-protector -c kernel.c -o kernel.o
    gcc -m32 -fno-stack-protector -c ./src/print.c -o ./src/print.o
    gcc -m32 -fno-stack-protector -c ./src/commands.c -o ./src/commands.o
    ld -m elf_i386 -T link.ld -o ./bin/kernel.bin boot.o kernel.o ./src/print.o ./src/commands.o
fi

# Executa o QEMU com opções de inicialização direta
# Esta é a maneira mais rápida e confiável de iniciar o sistema
# Usa stdio para entrada/saída, evitando completamente a interface gráfica e o processo de boot

echo "Iniciando NoobOS em modo direto..."
echo "Pressione Ctrl+C para sair"
echo "Iniciando em 1 segundo..."
sleep 1

# Redireciona a saída diretamente para o terminal
qemu-system-x86_64 -kernel ./bin/kernel.bin \
    -append "console=ttyS0" \
    -serial stdio \
    -display none \
    -no-reboot \
    -machine acpi=off \
    -cpu qemu64 \
    -smp 1 \
    -m 16 