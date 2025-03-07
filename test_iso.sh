#!/bin/bash

# Verifica se a ISO existe
if [ ! -f OSDan2.1of.iso ]; then
    echo "ISO não encontrada. Execute create_iso.sh primeiro."
    exit 1
fi

# Executa o QEMU com a ISO em modo texto
echo "Iniciando DanOS a partir da ISO em modo texto..."
echo "Pressione Ctrl+A seguido de X para sair do QEMU"
echo "Iniciando em 1 segundo..."
sleep 1

qemu-system-x86_64 -cdrom OSDan2.1of.iso -boot d -m 64 -nographic 