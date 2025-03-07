BITS 32 ; We are switching to protected mode after booting from the disk

; There exists a standard for loading various x86 kernels using a bootloader called Multiboot specification

; Constantes do Multiboot
MBALIGN     equ  1 << 0            ; Alinha módulos carregados em limites de página
MEMINFO     equ  1 << 1            ; Fornece mapa de memória
FLAGS       equ  MBALIGN | MEMINFO ; Flags do multiboot
MAGIC       equ  0x1BADB002        ; 'Magic number' para o bootloader identificar
CHECKSUM    equ -(MAGIC + FLAGS)   ; Checksum do cabeçalho multiboot

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KiB
stack_top:

section .text
global _start
extern kernel_main

_start:
    ; Configura a pilha
    mov esp, stack_top

    ; Chama o kernel
    call kernel_main

    ; Se o kernel retornar, desabilita interrupções e entra em loop infinito
    cli
.hang:
    hlt
    jmp .hang