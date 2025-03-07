#ifndef _STDIO_H
#define _STDIO_H

#include "common.h"

// Função para formatar e escrever em um buffer
int sprintf(char* str, const char* format, ...);

// Função para formatar números
void format_int(char* buffer, int value);
void format_uint(char* buffer, unsigned int value);
void format_long(char* buffer, long value);
void format_ulong(char* buffer, unsigned long value);
void format_float(char* buffer, double value, int precision);

void* malloc(size_t size);
void free(void* ptr);

#define NULL ((void*)0)

#endif 