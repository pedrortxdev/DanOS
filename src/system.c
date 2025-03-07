#include "../include/stdio.h"
#include "../include/system.h"
#include "../include/print.h"
#include "../include/string.h"
#include "../include/common.h"

// Variáveis globais
static system_info_t sys_info;

// Detecta informações da CPU
void detect_cpu() {
    // Por enquanto, apenas valores fixos
    strcpy(sys_info.cpu_vendor, "DanOS CPU");
    sys_info.cpu_cores = 1;
}

// Detecta informações da memória
void detect_memory() {
    // Por enquanto, apenas valores fixos
    sys_info.total_memory = 64 * 1024 * 1024; // 64 MB
}

// Formata o tamanho da memória em uma string legível
void format_memory_size(uint32_t bytes, char* buffer) {
    static const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double size = bytes;
    
    while (size >= 1024 && unit < 4) {
        size /= 1024;
        unit++;
    }
    
    if (size == (uint32_t)size) {
        sprintf(buffer, "%u %s", (uint32_t)size, units[unit]);
    } else {
        sprintf(buffer, "%.2f %s", size, units[unit]);
    }
}

// Função para inicializar o terminal
void init_terminal(void) {
    // Por enquanto, não faz nada
    // Será implementado quando adicionarmos suporte a VGA
}

// Mostra informações do sistema
void show_system_info() {
    char cores_str[8];
    char mem_buffer[32];
    
    sprintf(cores_str, "%u", sys_info.cpu_cores);
    format_memory_size(sys_info.total_memory, mem_buffer);
    
    print_colored("\nInformações do Sistema:\n", COLOR_CYAN, COLOR_BLACK);
    print_colored("CPU: ", COLOR_WHITE, COLOR_BLACK);
    print_msg(sys_info.cpu_vendor);
    print_msg(" (");
    print_msg(cores_str);
    print_msg(" núcleo");
    if (sys_info.cpu_cores > 1) print_msg("s");
    print_msg(")\n");
    
    print_colored("Memória: ", COLOR_WHITE, COLOR_BLACK);
    print_msg(mem_buffer);
    print_msg("\n\n");
}

void get_system_info(system_info_t* info) {
    strcpy(info->cpu_vendor, "DanOS CPU");
    // ... existing code ...
} 