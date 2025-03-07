#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "common.h"

// Estrutura para informações do sistema
typedef struct {
    char cpu_vendor[13];
    uint32_t cpu_cores;
    uint32_t total_memory;
} system_info_t;

// Funções do sistema
void detect_cpu(void);
void detect_memory(void);
void format_memory_size(uint32_t bytes, char* buffer);
void show_system_info(void);

#endif 