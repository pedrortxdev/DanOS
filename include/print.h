#ifndef _PRINT_H
#define _PRINT_H

#include "common.h"

// Constantes para cores
#define COLOR_BLACK   0
#define COLOR_BLUE    1
#define COLOR_GREEN   2
#define COLOR_CYAN    3
#define COLOR_RED     4
#define COLOR_MAGENTA 5
#define COLOR_BROWN   6
#define COLOR_WHITE   7

// Dimensões do terminal VGA
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

// Funções de impressão
void print_char(char c);
void print_msg(const char* msg);
void print_colored(const char* msg, uint8_t foreground, uint8_t background);
void print_backspace(void);
void clear_screen(void);

#endif
