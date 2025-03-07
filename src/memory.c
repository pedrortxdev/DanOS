#include "../include/memory.h"

// Área de memória para alocação dinâmica
#define HEAP_SIZE (1024 * 1024) // 1MB
static uint8_t heap[HEAP_SIZE];
static size_t heap_used = 0;

// Funções de gerenciamento de memória
void* malloc(size_t size) {
    // Alinha o tamanho para múltiplo de 4 bytes
    size = (size + 3) & ~3;
    
    if (heap_used + size > HEAP_SIZE) {
        return NULL; // Sem memória disponível
    }
    
    void* ptr = &heap[heap_used];
    heap_used += size;
    return ptr;
}

void free(void* ptr) {
    // Por simplicidade, não implementamos liberação de memória
    // Em um sistema real, você implementaria um gerenciador de memória adequado
    (void)ptr;
}

void init_memory(void) {
    heap_used = 0;
} 