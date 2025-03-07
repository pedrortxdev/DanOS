#ifndef _SHELL_H
#define _SHELL_H

#include "common.h"

#define MAX_PATH 256
#define MAX_ARGS 16
#define MAX_HISTORY 50

// Estrutura para argumentos do comando
typedef struct {
    char* command;
    char* args[16];
    int num_args;
} command_args_t;

// Estrutura para histórico de comandos
typedef struct {
    char* commands[MAX_HISTORY];
    int count;
    int current;
} command_history_t;

// Estrutura para armazenar informações do diretório
typedef struct {
    char name[256];
    int is_directory;
    unsigned long size;
} dir_entry_t;

// Funções do shell
void init_shell(void);
void add_to_history(const char* command);
char* get_previous_command(void);
char* get_next_command(void);
void parse_command(const char* input, command_args_t* args);
void execute_command(command_args_t* args);
void show_prompt(void);

// Comandos built-in
int cmd_cd(command_args_t* args);
int cmd_ls(command_args_t* args);
int cmd_mkdir(command_args_t* args);
int cmd_rm(command_args_t* args);
int cmd_cat(command_args_t* args);
int cmd_echo(command_args_t* args);
int cmd_pwd(command_args_t* args);
int cmd_clear(command_args_t* args);
int cmd_help(command_args_t* args);
int cmd_history(command_args_t* args);
int cmd_lscpu(command_args_t* args);
int cmd_free(command_args_t* args);
int cmd_nano(command_args_t* args);
int cmd_speedtest(command_args_t* args);

// Variáveis globais
extern char current_dir[MAX_PATH];
extern command_history_t history;

#endif 