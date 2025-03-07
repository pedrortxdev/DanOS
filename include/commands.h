#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "shell.h"

// Inicializa o sistema de arquivos
void init_filesystem(void);

// Funções de manipulação de arquivos e diretórios
int create_directory(const char* path);
int remove_file(const char* path, int recursive);
int change_directory(const char* path);
int list_directory(const char* path, dir_entry_t** entries, int* count);
int read_file(const char* path, char* buffer, int size);
int write_file(const char* path, const char* buffer, int size);

#endif 