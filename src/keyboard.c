#include "../include/keyboard.h"
#include "../include/print.h"
#include "../include/common.h"

// Portas de E/S do teclado
#define KEYBOARD_DATA_PORT    0x60
#define KEYBOARD_STATUS_PORT  0x64

// Flags de status do teclado
#define KEYBOARD_OUTPUT_FULL  0x01
#define KEYBOARD_INPUT_FULL   0x02

// Flags de controle
#define LEFT_CTRL_PRESSED   0x1D
#define LEFT_CTRL_RELEASED  0x9D

// Estado das teclas de controle
static int ctrl_pressed = 0;

// Mapa de teclas (simplificado)
static const char scancode_to_char[] = {
    0,   // 0x00 - Erro
    0,   // 0x01 - Esc
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', // 0x0F - Tab
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,   // 0x1D - Control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   // 0x2A - Left Shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,   // 0x36 - Right Shift
    '*', // 0x37 - Keypad *
    0,   // 0x38 - Alt
    ' ', // 0x39 - Space
};

// Inicializa o controlador de teclado
void init_keyboard() {
    ctrl_pressed = 0;
}

// Lê um caractere do teclado
char get_keyboard_char() {
    // Verifica se há dados disponíveis
    if ((inb(KEYBOARD_STATUS_PORT) & 1) == 0) {
        return 0;
    }
    
    // Lê o scancode
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    // Atualiza o estado do Ctrl
    if (scancode == LEFT_CTRL_PRESSED) {
        ctrl_pressed = 1;
        return 0;
    }
    if (scancode == LEFT_CTRL_RELEASED) {
        ctrl_pressed = 0;
        return 0;
    }
    
    // Verifica se é uma tecla pressionada (não solta)
    if (scancode & 0x80) {
        return 0;
    }
    
    // Se Ctrl está pressionado, retorna o caractere de controle
    if (ctrl_pressed && scancode < sizeof(scancode_to_char)) {
        char c = scancode_to_char[scancode];
        if (c >= 'a' && c <= 'z') {
            return c - 'a' + 1; // Converte para código de controle (1-26)
        }
    }
    
    // Converte o scancode para caractere normal
    if (scancode < sizeof(scancode_to_char)) {
        return scancode_to_char[scancode];
    }
    
    return 0;
} 