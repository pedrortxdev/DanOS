#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/memory.h"

// Funções auxiliares
static int itoa(int num, char* str, int base) {
    int i = 0;
    int is_negative = 0;
    
    // Trata o caso especial de zero
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return i;
    }
    
    // Trata números negativos
    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }
    
    // Processa os dígitos
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
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
    return i;
}

static int ultoa(uint32_t num, char* str, int base) {
    int i = 0;
    
    // Trata o caso especial de zero
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return i;
    }
    
    // Processa os dígitos
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
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
    return i;
}

// Implementação simples de sprintf para evitar dependências externas
int sprintf(char* str, const char* format, ...) {
    // Esta é uma implementação muito simplificada que suporta apenas alguns formatos básicos
    // Em um sistema real, você implementaria uma versão completa ou usaria uma biblioteca
    
    // Suporta apenas %d, %u, %x e %s por simplicidade
    char* s = str;
    const char* f = format;
    int* arg = (int*)(&format + 1); // Pega o primeiro argumento após format
    
    while (*f) {
        if (*f == '%') {
            f++;
            if (*f == 'd') {
                // Converte int para string
                s += itoa(*arg++, s, 10);
            } else if (*f == 'u') {
                // Converte uint32_t para string decimal
                s += ultoa(*(uint32_t*)arg++, s, 10);
            } else if (*f == 'x') {
                // Converte uint32_t para string hexadecimal
                s += ultoa(*(uint32_t*)arg++, s, 16);
            } else if (*f == 's') {
                // Copia string
                char* val = (char*)*arg++;
                while (*val) {
                    *s++ = *val++;
                }
            } else if (*f == '%') {
                // Escape para %
                *s++ = '%';
            }
        } else {
            *s++ = *f;
        }
        f++;
    }
    
    *s = '\0'; // Termina a string
    return s - str; // Retorna o comprimento
}