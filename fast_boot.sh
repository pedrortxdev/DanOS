#!/bin/bash

# Compila o kernel se necessário
if [ ! -f kernel.bin ] || [ boot.asm -nt kernel.bin ] || [ kernel.c -nt kernel.bin ]; then
    echo "Recompilando o kernel..."
    make clean
    make
    
    if [ $? -ne 0 ]; then
        echo "Erro: Falha ao compilar o kernel"
        exit 1
    fi
fi

# Configura o modelo de CPU baseado na disponibilidade do KVM
if [ -e /dev/kvm ]; then
    CPU_MODEL="host"
else
    CPU_MODEL="qemu64"
fi

echo "Iniciando DanOS em modo ultra-rápido..."
qemu-system-x86_64 -kernel kernel.bin -cpu $CPU_MODEL -m 64 -nographic 