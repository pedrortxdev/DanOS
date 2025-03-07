#include "include/stdio.h"
#include "include/print.h"
#include "include/keyboard.h"
#include "include/shell.h"
#include "include/commands.h"
#include "include/system.h"
#include "include/common.h"
#include "include/memory.h"

#define COMMAND_BUFFER_SIZE 256

static const char* logo[] = {
    "  ____              ___  ____  \n",
    " |  _ \\  __ _ _ __ / _ \\/ ___| \n",
    " | | | |/ _` | '_ \\ | | \\___ \\ \n",
    " | |_| | (_| | | | | |_| |___) |\n",
    " |____/ \\__,_|_| |_|\\___/|____/ \n",
    "\n"
};

static void show_logo() {
    for (int i = 0; i < 6; i++) {
        print_colored(logo[i], COLOR_CYAN, COLOR_BLACK);
    }
}

void show_prompt() {
    print_colored("noob@os> ", COLOR_GREEN, COLOR_BLACK);
}

void kernel_main() {
    init_memory();
    init_keyboard();
    init_filesystem();
    init_shell();
    
    detect_cpu();
    detect_memory();
    
    clear_screen();
    show_logo();
    show_system_info();
    
    char command_buffer[COMMAND_BUFFER_SIZE];
    int buffer_pos = 0;
    command_args_t args = {0};
    
    show_prompt();
    
    while (1) {
        char c = get_keyboard_char();
        
        if (c == '\n') {
            command_buffer[buffer_pos] = '\0';
            print_msg("\n");
            
            if (buffer_pos > 0) {
                add_to_history(command_buffer);
                parse_command(command_buffer, &args);
                execute_command(&args);
            }
            
            buffer_pos = 0;
            show_prompt();
        }
        else if (c == '\b' && buffer_pos > 0) {
            buffer_pos--;
            print_backspace();
        }
        else if (c >= ' ' && c <= '~' && buffer_pos < COMMAND_BUFFER_SIZE - 1) {
            command_buffer[buffer_pos++] = c;
            print_char(c);
        }
    }
}
