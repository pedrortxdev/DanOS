#ifndef _STRING_H
#define _STRING_H

#include "common.h"

// Implementações básicas de funções de string

void* memcpy(void* dest, const void* src, size_t count);
void* memset(void* dest, char val, size_t count);
int strlen(const char* str);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
int strcmp(const char* str1, const char* str2);
char* strdup(const char* str);
char* strchr(const char* str, int c);
char* strrchr(const char* str, int c);
char* strstr(const char* haystack, const char* needle);

#endif // _STRING_H 