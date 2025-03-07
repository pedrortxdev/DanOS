#include "../include/commands.h"
#include "../include/print.h"
#include "../include/shell.h"
#include "../include/string.h"
#include "../include/common.h"
#include "../include/memory.h"

// Sistema de arquivos simulado
typedef struct {
    char name[256];
    int is_directory;
    char* content;
    unsigned long size;
} fs_entry_t;

#define MAX_ENTRIES 1024
fs_entry_t filesystem[MAX_ENTRIES];
int num_entries = 0;

// Inicializa o sistema de arquivos
void init_filesystem(void) {
    // Cria diretórios padrão
    create_directory("/");
    create_directory("/bin");
    create_directory("/etc");
    create_directory("/home");
    create_directory("/usr");
    create_directory("/var");
    
    // Cria alguns arquivos de exemplo
    write_file("/etc/hostname", "danos\n", 6);
    write_file("/etc/version", "1.0\n", 4);
    write_file("/README.txt", "DanOS - Sistema Operacional Educacional\n", 39);
}

// Funções auxiliares
int find_entry(const char* path) {
    for (int i = 0; i < num_entries; i++) {
        if (strcmp(filesystem[i].name, path) == 0) {
            return i;
        }
    }
    return -1;
}

// Implementação das funções do sistema de arquivos
int create_directory(const char* path) {
    if (find_entry(path) >= 0) {
        return -1; // Já existe
    }
    
    if (num_entries >= MAX_ENTRIES) {
        return -1; // Sistema de arquivos cheio
    }
    
    strcpy(filesystem[num_entries].name, path);
    filesystem[num_entries].is_directory = 1;
    filesystem[num_entries].content = NULL;
    filesystem[num_entries].size = 0;
    num_entries++;
    
    return 0;
}

int remove_file(const char* path, int recursive) {
    int idx = find_entry(path);
    if (idx < 0) {
        return -1; // Não encontrado
    }
    
    // Se for diretório, verifica se está vazio (a menos que seja recursivo)
    if (filesystem[idx].is_directory && !recursive) {
        for (int i = 0; i < num_entries; i++) {
            if (i != idx && strstr(filesystem[i].name, path) == filesystem[i].name) {
                return -1; // Diretório não vazio
            }
        }
    }
    
    // Remove o arquivo/diretório
    if (filesystem[idx].content) {
        free(filesystem[idx].content);
    }
    
    // Remove entradas recursivamente
    if (recursive) {
        for (int i = 0; i < num_entries; i++) {
            if (i != idx && strstr(filesystem[i].name, path) == filesystem[i].name) {
                if (filesystem[i].content) {
                    free(filesystem[i].content);
                }
                // Move as entradas restantes
                for (int j = i; j < num_entries - 1; j++) {
                    filesystem[j] = filesystem[j + 1];
                }
                num_entries--;
                i--; // Ajusta o índice
            }
        }
    }
    
    // Move as entradas restantes
    for (int i = idx; i < num_entries - 1; i++) {
        filesystem[i] = filesystem[i + 1];
    }
    num_entries--;
    
    return 0;
}

int change_directory(const char* path) {
    if (strcmp(path, "/") == 0) {
        return 0; // Raiz sempre existe
    }
    
    if (strcmp(path, ".") == 0) {
        return 0; // Diretório atual
    }
    
    if (strcmp(path, "..") == 0) {
        // Se já estiver na raiz, não faz nada
        if (strcmp(current_dir, "/") == 0) {
            return 0;
        }
        
        // Remove o último diretório do caminho
        char* last_slash = strrchr(current_dir, '/');
        if (last_slash != current_dir) {
            *last_slash = '\0';
        } else {
            current_dir[1] = '\0';
        }
        return 0;
    }
    
    int idx = find_entry(path);
    if (idx < 0 || !filesystem[idx].is_directory) {
        return -1;
    }
    
    return 0;
}

int list_directory(const char* path, dir_entry_t** entries, int* count) {
    // Aloca espaço para as entradas
    *entries = (dir_entry_t*)malloc(sizeof(dir_entry_t) * MAX_ENTRIES);
    *count = 0;
    
    // Adiciona . e ..
    strcpy((*entries)[*count].name, ".");
    (*entries)[*count].is_directory = 1;
    (*entries)[*count].size = 0;
    (*count)++;
    
    strcpy((*entries)[*count].name, "..");
    (*entries)[*count].is_directory = 1;
    (*entries)[*count].size = 0;
    (*count)++;
    
    // Lista entradas no diretório
    int path_len = strlen(path);
    for (int i = 0; i < num_entries; i++) {
        if (strstr(filesystem[i].name, path) == filesystem[i].name) {
            // Pula o próprio diretório
            if (strcmp(filesystem[i].name, path) == 0) {
                continue;
            }
            
            // Verifica se é uma entrada direta (não em subdiretório)
            const char* subpath = filesystem[i].name + path_len;
            if (*subpath == '/') subpath++;
            if (strchr(subpath, '/') != NULL) {
                continue; // Está em um subdiretório
            }
            
            strcpy((*entries)[*count].name, subpath);
            (*entries)[*count].is_directory = filesystem[i].is_directory;
            (*entries)[*count].size = filesystem[i].size;
            (*count)++;
        }
    }
    
    return 0;
}

int read_file(const char* path, char* buffer, int size) {
    int idx = find_entry(path);
    if (idx < 0 || filesystem[idx].is_directory) {
        return -1;
    }
    
    unsigned int copy_size = filesystem[idx].size < (unsigned int)size ? filesystem[idx].size : (unsigned int)size;
    memcpy(buffer, filesystem[idx].content, copy_size);
    return (int)copy_size;
}

int write_file(const char* path, const char* buffer, int size) {
    int idx = find_entry(path);
    if (idx >= 0) {
        if (filesystem[idx].is_directory) {
            return -1;
        }
        // Atualiza arquivo existente
        free(filesystem[idx].content);
        char* new_content = (char*)malloc(size);
        if (!new_content) {
            return -1;
        }
        filesystem[idx].content = new_content;
        memcpy(filesystem[idx].content, buffer, size);
        filesystem[idx].size = size;
        return size;
    }
    
    if (num_entries >= MAX_ENTRIES) {
        return -1; // Sistema de arquivos cheio
    }
    
    // Cria novo arquivo
    strcpy(filesystem[num_entries].name, path);
    filesystem[num_entries].is_directory = 0;
    char* new_content = (char*)malloc(size);
    if (!new_content) {
        return -1;
    }
    filesystem[num_entries].content = new_content;
    memcpy(filesystem[num_entries].content, buffer, size);
    filesystem[num_entries].size = size;
    num_entries++;
    
    return size;
} 