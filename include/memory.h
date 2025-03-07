#ifndef _MEMORY_H
#define _MEMORY_H

#include "common.h"

// Funções de gerenciamento de memória
void* malloc(size_t size);
void free(void* ptr);
void init_memory(void);

#endif // _MEMORY_H 