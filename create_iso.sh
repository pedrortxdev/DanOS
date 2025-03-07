#!/bin/bash

# Compila o kernel
echo "Compilando o kernel..."
make clean
make

if [ $? -ne 0 ]; then
    echo "Erro: Falha ao compilar o kernel"
    exit 1
fi

# Cria a imagem ISO
echo "Criando imagem ISO..."
cp kernel.bin iso/boot/
grub-mkrescue -o DanOS.iso iso/

echo -e "\nImagem ISO criada: DanOS.iso\n"
echo "Para executar no VirtualBox:"
echo "1. Abra o VirtualBox"
echo "2. Clique em 'Novo'"
echo "3. Dê um nome como 'DanOS'"
echo "4. Selecione 'Tipo: Other' e 'Versão: Other/Unknown'"
echo "5. Aloque 64MB de memória"
echo "6. Não crie um disco rígido virtual"
echo "7. Após criar a VM, clique em 'Configurações'"
echo "8. Vá para 'Armazenamento'"
echo "9. Adicione a imagem ISO 'DanOS.iso' ao controlador IDE"
echo "10. Inicie a VM" 