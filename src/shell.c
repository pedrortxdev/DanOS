#include "../include/shell.h"
#include "../include/print.h"
#include "../include/commands.h"
#include "../include/string.h"
#include "../include/common.h"
#include "../include/memory.h"
#include "../include/keyboard.h"

// Variáveis globais
char current_dir[MAX_PATH] = "/";
command_history_t history = {0};

// Estrutura para mapear comandos para suas funções
typedef struct {
    const char* name;
    int (*func)(command_args_t*);
    const char* help;
} command_t;

// Lista de comandos disponíveis
static command_t commands[] = {
    {"cd", cmd_cd, "Muda o diretório atual"},
    {"ls", cmd_ls, "Lista arquivos e diretórios"},
    {"mkdir", cmd_mkdir, "Cria um novo diretório"},
    {"rm", cmd_rm, "Remove arquivos ou diretórios"},
    {"cat", cmd_cat, "Mostra o conteúdo de um arquivo"},
    {"echo", cmd_echo, "Exibe uma mensagem"},
    {"pwd", cmd_pwd, "Mostra o diretório atual"},
    {"clear", cmd_clear, "Limpa a tela"},
    {"help", cmd_help, "Mostra esta ajuda"},
    {"history", cmd_history, "Mostra o histórico de comandos"},
    {"lscpu", cmd_lscpu, "Mostra informações da CPU"},
    {"free", cmd_free, "Mostra informações da memória"},
    {"nano", cmd_nano, "Editor de texto simples"},
    {"speedtest", cmd_speedtest, "Teste de velocidade da rede"},
    {NULL, NULL, NULL}
};

// Inicializa o shell
void init_shell(void) {
    history.count = 0;
    history.current = 0;
}

// Adiciona um comando ao histórico
void add_to_history(const char* command) {
    if (history.count < MAX_HISTORY) {
        history.commands[history.count] = strdup(command);
        history.count++;
    } else {
        // Remove o comando mais antigo
        free(history.commands[0]);
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            history.commands[i] = history.commands[i + 1];
        }
        history.commands[MAX_HISTORY - 1] = strdup(command);
    }
    history.current = history.count;
}

// Obtém o comando anterior do histórico
char* get_previous_command(void) {
    if (history.current > 0) {
        history.current--;
        return history.commands[history.current];
    }
    return NULL;
}

// Obtém o próximo comando do histórico
char* get_next_command(void) {
    if (history.current < history.count - 1) {
        history.current++;
        return history.commands[history.current];
    }
    return NULL;
}

// Função auxiliar para verificar se um caractere é um delimitador
static int is_delimiter(char c, const char* delimiters) {
    while (*delimiters) {
        if (c == *delimiters) {
            return 1;
        }
        delimiters++;
    }
    return 0;
}

// Nossa própria implementação de strtok
static char* my_strtok(char* str, const char* delimiters) {
    static char* next_token = NULL;
    char* token_start;
    
    // Se str não é NULL, começamos uma nova tokenização
    if (str != NULL) {
        next_token = str;
    }
    
    // Se next_token é NULL, não há mais tokens
    if (next_token == NULL) {
        return NULL;
    }
    
    // Pula delimitadores iniciais
    while (*next_token && is_delimiter(*next_token, delimiters)) {
        next_token++;
    }
    
    // Se chegamos ao fim da string, não há mais tokens
    if (*next_token == '\0') {
        next_token = NULL;
        return NULL;
    }
    
    // Marca o início do token
    token_start = next_token;
    
    // Encontra o fim do token
    while (*next_token && !is_delimiter(*next_token, delimiters)) {
        next_token++;
    }
    
    // Se encontramos um delimitador, marca o fim do token
    if (*next_token) {
        *next_token = '\0';
        next_token++;
    } else {
        next_token = NULL;
    }
    
    return token_start;
}

// Parse do comando em argumentos
void parse_command(const char* input, command_args_t* args) {
    args->num_args = 0;
    char* token = my_strtok((char*)input, " ");
    
    while (token && args->num_args < MAX_ARGS) {
        args->args[args->num_args++] = token;
        token = my_strtok(NULL, " ");
    }
    
    args->command = args->num_args > 0 ? args->args[0] : NULL;
}

// Executa um comando
void execute_command(command_args_t* args) {
    if (args->num_args == 0) return;
    
    for (command_t* cmd = commands; cmd->name; cmd++) {
        if (strcmp(args->args[0], cmd->name) == 0) {
            cmd->func(args);
            return;
        }
    }
    
    print_msg("Comando não encontrado: ");
    print_msg(args->args[0]);
    print_msg("\n");
}

// Implementação dos comandos
int cmd_cd(command_args_t* args) {
    if (args->num_args < 2) {
        print_msg("Uso: cd <diretório>\n");
        return -1;
    }
    
    char new_path[MAX_PATH];
    if (args->args[1][0] == '/') {
        strcpy(new_path, args->args[1]);
    } else {
        strcpy(new_path, current_dir);
        if (new_path[strlen(new_path)-1] != '/') {
            strcat(new_path, "/");
        }
        strcat(new_path, args->args[1]);
    }
    
    if (change_directory(new_path) == 0) {
        strcpy(current_dir, new_path);
        return 0;
    }
    
    print_msg("Erro: Diretório não encontrado\n");
    return -1;
}

int cmd_ls(command_args_t* args) {
    const char* path = args->num_args > 1 ? args->args[1] : current_dir;
    dir_entry_t* entries;
    int count;
    
    if (list_directory(path, &entries, &count) == 0) {
        for (int i = 0; i < count; i++) {
            if (entries[i].is_directory) {
                print_colored(entries[i].name, COLOR_CYAN, COLOR_BLACK);
            } else {
                print_colored(entries[i].name, COLOR_WHITE, COLOR_BLACK);
            }
            print_msg("  ");
        }
        print_msg("\n");
        free(entries);
        return 0;
    }
    
    print_msg("Erro: Não foi possível listar o diretório\n");
    return -1;
}

int cmd_mkdir(command_args_t* args) {
    if (args->num_args < 2) {
        print_msg("Uso: mkdir <diretório>\n");
        return -1;
    }
    
    char path[MAX_PATH];
    if (args->args[1][0] == '/') {
        strcpy(path, args->args[1]);
    } else {
        strcpy(path, current_dir);
        if (path[strlen(path)-1] != '/') {
            strcat(path, "/");
        }
        strcat(path, args->args[1]);
    }
    
    if (create_directory(path) == 0) {
        return 0;
    }
    
    print_msg("Erro: Não foi possível criar o diretório\n");
    return -1;
}

int cmd_rm(command_args_t* args) {
    if (args->num_args < 2) {
        print_msg("Uso: rm [-r] <arquivo/diretório>\n");
        return -1;
    }
    
    int recursive = 0;
    const char* target = args->args[1];
    
    if (strcmp(args->args[1], "-r") == 0) {
        if (args->num_args < 3) {
            print_msg("Uso: rm -r <diretório>\n");
            return -1;
        }
        recursive = 1;
        target = args->args[2];
    }
    
    char path[MAX_PATH];
    if (target[0] == '/') {
        strcpy(path, target);
    } else {
        strcpy(path, current_dir);
        if (path[strlen(path)-1] != '/') {
            strcat(path, "/");
        }
        strcat(path, target);
    }
    
    if (remove_file(path, recursive) == 0) {
        return 0;
    }
    
    print_msg("Erro: Não foi possível remover o arquivo/diretório\n");
    return -1;
}

int cmd_cat(command_args_t* args) {
    if (args->num_args < 2) {
        print_msg("Uso: cat <arquivo>\n");
        return -1;
    }
    
    char path[MAX_PATH];
    if (args->args[1][0] == '/') {
        strcpy(path, args->args[1]);
    } else {
        strcpy(path, current_dir);
        if (path[strlen(path)-1] != '/') {
            strcat(path, "/");
        }
        strcat(path, args->args[1]);
    }
    
    // Debug: mostra o caminho que está tentando ler
    print_msg("Tentando ler arquivo: ");
    print_msg(path);
    print_msg("\n");
    
    char buffer[4096];
    int size = read_file(path, buffer, sizeof(buffer) - 1);
    if (size >= 0) {
        buffer[size] = '\0';
        print_msg(buffer);
        if (size > 0 && buffer[size-1] != '\n') {
            print_msg("\n");
        }
        return 0;
    }
    
    print_msg("Erro: Não foi possível ler o arquivo\n");
    return -1;
}

int cmd_echo(command_args_t* args) {
    for (int i = 1; i < args->num_args; i++) {
        print_msg(args->args[i]);
        if (i < args->num_args - 1) {
            print_msg(" ");
        }
    }
    print_msg("\n");
    return 0;
}

int cmd_pwd(command_args_t* args) {
    (void)args;
    print_msg(current_dir);
    print_msg("\n");
    return 0;
}

int cmd_clear(command_args_t* args) {
    (void)args;
    clear_screen();
    return 0;
}

int cmd_help(command_args_t* args) {
    (void)args;
    print_colored("Comandos disponíveis:\n", COLOR_CYAN, COLOR_BLACK);
    
    for (command_t* cmd = commands; cmd->name; cmd++) {
        print_colored("  ", COLOR_WHITE, COLOR_BLACK);
        print_colored(cmd->name, COLOR_GREEN, COLOR_BLACK);
        print_colored(" - ", COLOR_WHITE, COLOR_BLACK);
        print_colored(cmd->help, COLOR_WHITE, COLOR_BLACK);
        print_msg("\n");
    }
    
    return 0;
}

// Função auxiliar para converter um número em string
static void itoa(int num, char* str) {
    int i = 0;
    int is_negative = 0;
    
    // Trata o caso especial de zero
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    
    // Trata números negativos
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    // Converte cada dígito
    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }
    
    // Adiciona o sinal negativo
    if (is_negative) {
        str[i++] = '-';
    }
    
    // Inverte a string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
    
    str[i] = '\0';
}

int cmd_history(command_args_t* args) {
    (void)args;
    char number[8];
    
    for (int i = 0; i < history.count; i++) {
        itoa(i + 1, number);
        print_msg(number);
        print_msg("  ");
        print_msg(history.commands[i]);
        print_msg("\n");
    }
    
    return 0;
}

// Implementação do comando lscpu
int cmd_lscpu(command_args_t* args) {
    (void)args;
    print_msg("Arquitetura:                    x86_64\n");
    print_msg("Modo(s) operacional da CPU:     32-bit, 64-bit\n");
    print_msg("Ordem dos bytes:                Little Endian\n");
    print_msg("CPU(s):                         32\n");
    print_msg("Thread(s) per núcleo:           2\n");
    print_msg("Núcleo(s) por soquete:         32\n");
    print_msg("Soquete(s):                     1\n");
    print_msg("Família da CPU:                 25\n");
    print_msg("Modelo:                         33\n");
    print_msg("Nome do modelo:                 Daniel Ryzen 11950X\n");
    print_msg("Stepping:                       2\n");
    print_msg("CPU MHz:                        6400.000\n");
    print_msg("Cache L1d:                      32K por núcleo\n");
    print_msg("Cache L1i:                      32K por núcleo\n");
    print_msg("Cache L2:                       512K por núcleo\n");
    print_msg("Cache L3:                       1.4GB compartilhado\n");
    return 0;
}

// Implementação do comando free
int cmd_free(command_args_t* args) {
    if (args->num_args > 1 && strcmp(args->args[1], "-h") == 0) {
        print_msg("               total        usada        livre      compart.    buff/cache   disponível\n");
        print_msg("Mem:          512,0Gi      24,3Gi      456,5Gi     0,0Gi       31,2Gi      487,7Gi\n");
        print_msg("Swap:           0,0Gi       0,0Gi        0,0Gi\n");
    } else {
        print_msg("               total        usada        livre      compart.    buff/cache   disponível\n");
        print_msg("Mem:           524288       24883       467456         0         31949       499405\n");
        print_msg("Swap:              0           0            0\n");
    }
    return 0;
}

// Implementação do comando speedtest
int cmd_speedtest(command_args_t* args) {
    (void)args;
    print_msg("Iniciando teste de velocidade...\n\n");
    print_msg("Testando download:\n");
    
    int download_speed = 10;
    while (download_speed <= 800) {
        print_msg("Download: ");
        char speed_str[32];
        itoa(download_speed, speed_str);
        print_msg(speed_str);
        print_msg(" Mbps\r");
        download_speed += 50;
        // Simula um pequeno delay
        for (volatile int i = 0; i < 1000000; i++);
    }
    print_msg("\nVelocidade de Download Final: 800.0 Mbps\n\n");
    
    print_msg("Testando upload:\n");
    int upload_speed = 7;
    while (upload_speed <= 395) {
        print_msg("Upload: ");
        char speed_str[32];
        itoa(upload_speed, speed_str);
        print_msg(speed_str);
        print_msg(" Mbps\r");
        upload_speed += 25;
        // Simula um pequeno delay
        for (volatile int i = 0; i < 1000000; i++);
    }
    print_msg("\nVelocidade de Upload Final: 395.2 Mbps\n\n");
    
    print_msg("Teste completo!\n");
    print_msg("Latência: 5ms\n");
    print_msg("Jitter: 1ms\n");
    print_msg("Servidor: São Paulo, BR\n");
    
    return 0;
}

// Implementação do editor nano
#define NANO_BUFFER_SIZE 4096
#define NANO_MAX_LINES 100
#define NANO_LINE_SIZE 80

typedef struct {
    char* lines[NANO_MAX_LINES];
    int num_lines;
    int cursor_x;
    int cursor_y;
    int scroll_y;
    char filename[MAX_PATH];
    int modified;
} nano_editor_t;

static void nano_draw_status(nano_editor_t* editor) {
    // Limpa a linha de status
    print_colored("\n^X Sair  ^O Salvar  ^W Onde estou  ^K Recortar  ^U Colar", COLOR_BLACK, COLOR_WHITE);
    
    // Move o cursor de volta à posição correta
    for (int i = 0; i < VGA_WIDTH; i++) {
        print_backspace();
    }
}

static void nano_draw_screen(nano_editor_t* editor) {
    clear_screen();
    
    // Desenha o título
    print_colored("  GNU nano                    ", COLOR_BLACK, COLOR_WHITE);
    print_colored(editor->filename, COLOR_BLACK, COLOR_WHITE);
    if (editor->modified) print_colored(" [Modificado]", COLOR_BLACK, COLOR_WHITE);
    print_msg("\n\n");
    
    // Desenha o conteúdo
    for (int i = editor->scroll_y; i < editor->num_lines && i < editor->scroll_y + VGA_HEIGHT - 4; i++) {
        if (editor->lines[i]) {
            print_msg(editor->lines[i]);
        }
        print_msg("\n");
    }
    
    // Desenha a barra de status
    nano_draw_status(editor);
}

static void nano_insert_char(nano_editor_t* editor, char c) {
    if (!editor->lines[editor->cursor_y]) {
        editor->lines[editor->cursor_y] = (char*)malloc(NANO_LINE_SIZE);
        memset(editor->lines[editor->cursor_y], 0, NANO_LINE_SIZE);
    }
    
    if (editor->cursor_x < NANO_LINE_SIZE - 1) {
        // Move os caracteres para a direita
        for (int i = strlen(editor->lines[editor->cursor_y]); i > editor->cursor_x; i--) {
            editor->lines[editor->cursor_y][i] = editor->lines[editor->cursor_y][i-1];
        }
        editor->lines[editor->cursor_y][editor->cursor_x] = c;
        editor->cursor_x++;
        editor->modified = 1;
    }
}

static void nano_delete_char(nano_editor_t* editor) {
    if (editor->cursor_x > 0 && editor->lines[editor->cursor_y]) {
        char* line = editor->lines[editor->cursor_y];
        for (int i = editor->cursor_x - 1; line[i]; i++) {
            line[i] = line[i+1];
        }
        editor->cursor_x--;
        editor->modified = 1;
    }
}

static void nano_save_file(nano_editor_t* editor) {
    // Calcula o tamanho total
    int total_size = 0;
    for (int i = 0; i < editor->num_lines; i++) {
        if (editor->lines[i]) {
            total_size += strlen(editor->lines[i]) + 1; // +1 para \n
        }
    }
    
    // Cria o buffer
    char* buffer = (char*)malloc(total_size + 1);
    char* p = buffer;
    
    // Copia as linhas
    for (int i = 0; i < editor->num_lines; i++) {
        if (editor->lines[i]) {
            int len = strlen(editor->lines[i]);
            memcpy(p, editor->lines[i], len);
            p += len;
            *p++ = '\n';
        }
    }
    *p = '\0';
    
    // Salva o arquivo
    if (write_file(editor->filename, buffer, total_size) >= 0) {
        editor->modified = 0;
    }
    
    free(buffer);
}

static void nano_load_file(nano_editor_t* editor) {
    char buffer[NANO_BUFFER_SIZE];
    int size = read_file(editor->filename, buffer, NANO_BUFFER_SIZE - 1);
    
    if (size >= 0) {
        buffer[size] = '\0';
        
        // Divide o buffer em linhas
        char* line = buffer;
        editor->num_lines = 0;
        
        while (line && editor->num_lines < NANO_MAX_LINES) {
            char* next = strchr(line, '\n');
            if (next) *next = '\0';
            
            editor->lines[editor->num_lines] = strdup(line);
            editor->num_lines++;
            
            line = next ? next + 1 : NULL;
        }
    }
    
    if (editor->num_lines == 0) {
        editor->lines[0] = strdup("");
        editor->num_lines = 1;
    }
}

// Códigos de controle
#define CTRL_X  24  // Ctrl+X (Cancel)
#define CTRL_O  15  // Ctrl+O (WriteOut)
#define CTRL_W  23  // Ctrl+W (Where Is)
#define CTRL_K  11  // Ctrl+K (Cut Text)
#define CTRL_U  21  // Ctrl+U (Paste)

int cmd_nano(command_args_t* args) {
    if (args->num_args < 2) {
        print_msg("Uso: nano <arquivo>\n");
        return -1;
    }
    
    // Inicializa o editor
    nano_editor_t editor = {0};
    editor.cursor_x = 0;
    editor.cursor_y = 0;
    editor.scroll_y = 0;
    
    // Configura o caminho do arquivo
    if (args->args[1][0] == '/') {
        strcpy(editor.filename, args->args[1]);
    } else {
        strcpy(editor.filename, current_dir);
        if (editor.filename[strlen(editor.filename)-1] != '/') {
            strcat(editor.filename, "/");
        }
        strcat(editor.filename, args->args[1]);
    }
    
    // Carrega o arquivo se existir
    nano_load_file(&editor);
    
    // Loop principal do editor
    while (1) {
        nano_draw_screen(&editor);
        
        char c = get_keyboard_char();
        if (c) {
            if (c == CTRL_X) { // Ctrl+X
                if (!editor.modified || 
                    (print_msg("\nArquivo modificado. Salvar antes de sair? (S/N) "), 
                     c = get_keyboard_char(), c == 'N' || c == 'n')) {
                    break;
                }
                if (c == 'S' || c == 's') {
                    nano_save_file(&editor);
                    break;
                }
            }
            else if (c == CTRL_O) { // Ctrl+O
                nano_save_file(&editor);
            }
            else if (c == CTRL_W) { // Ctrl+W
                print_msg("\nPosição atual: Linha ");
                char num[8];
                itoa(editor.cursor_y + 1, num);
                print_msg(num);
                print_msg(", Coluna ");
                itoa(editor.cursor_x + 1, num);
                print_msg(num);
                print_msg("\nPressione qualquer tecla para continuar...");
                get_keyboard_char();
            }
            else if (c == '\b') {
                nano_delete_char(&editor);
            }
            else if (c == '\n') {
                if (editor.num_lines < NANO_MAX_LINES - 1) {
                    editor.cursor_y++;
                    editor.cursor_x = 0;
                    if (editor.cursor_y >= editor.num_lines) {
                        editor.lines[editor.cursor_y] = strdup("");
                        editor.num_lines++;
                    }
                    editor.modified = 1;
                }
            }
            else if (c >= ' ' && c <= '~') {
                nano_insert_char(&editor, c);
            }
        }
    }
    
    // Limpa a memória
    for (int i = 0; i < editor.num_lines; i++) {
        if (editor.lines[i]) {
            free(editor.lines[i]);
        }
    }
    
    clear_screen();
    return 0;
} 