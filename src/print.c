#include "../include/print.h"
#include "../include/common.h"

// Variáveis globais para controle do terminal
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t current_color = 0x07; // Branco sobre preto

// Função para mover o cursor
static void move_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

// Função para rolar a tela
static void scroll() {
    if (cursor_y >= VGA_HEIGHT) {
        // Move todas as linhas uma posição para cima
        for (int i = 0; i < VGA_HEIGHT - 1; i++) {
            for (int j = 0; j < VGA_WIDTH; j++) {
                VGA_MEMORY[i * VGA_WIDTH + j] = VGA_MEMORY[(i + 1) * VGA_WIDTH + j];
            }
        }
        
        // Limpa a última linha
        for (int j = 0; j < VGA_WIDTH; j++) {
            VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + j] = (current_color << 8) | ' ';
        }
        
        cursor_y--;
    }
}

// Função para imprimir um caractere
void print_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        scroll();
    }
    else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
        }
        else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
        }
        VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = (current_color << 8) | ' ';
    }
    else if (c >= ' ') {
        VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = (current_color << 8) | c;
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
            scroll();
        }
    }
    move_cursor();
}

// Função para imprimir uma string
void print_msg(const char* msg) {
    while (*msg) {
        print_char(*msg++);
    }
}

// Função para imprimir texto colorido
void print_colored(const char* msg, uint8_t foreground, uint8_t background) {
    uint8_t old_color = current_color;
    current_color = (background << 4) | (foreground & 0x0F);
    print_msg(msg);
    current_color = old_color;
}

// Função para apagar o último caractere
void print_backspace() {
    print_char('\b');
}

// Função para limpar a tela
void clear_screen() {
    for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        VGA_MEMORY[i] = (current_color << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}
