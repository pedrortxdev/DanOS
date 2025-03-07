#include "../include/string.h"
#include "../include/memory.h"

void* memcpy(void* dest, const void* src, size_t count) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for (size_t i = 0; i < count; i++) {
        d[i] = s[i];
    }
    return dest;
}

void* memset(void* dest, char val, size_t count) {
    uint8_t* d = (uint8_t*)dest;
    for (size_t i = 0; i < count; i++) {
        d[i] = val;
    }
    return dest;
}

int strlen(const char* str) {
    int len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while (*src) {
        *d++ = *src++;
    }
    *d = 0;
    return dest;
}

char* strcat(char* dest, const char* src) {
    char* d = dest + strlen(dest);
    while (*src) {
        *d++ = *src++;
    }
    *d = 0;
    return dest;
}

int strcmp(const char* str1, const char* str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

char* strdup(const char* str) {
    size_t len = strlen(str) + 1;
    char* new_str = (char*)malloc(len);
    if (new_str) {
        memcpy(new_str, str, len);
    }
    return new_str;
}

char* strchr(const char* str, int c) {
    while (*str) {
        if (*str == c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

char* strstr(const char* haystack, const char* needle) {
    if (!*needle) {
        return (char*)haystack;
    }
    
    while (*haystack) {
        const char* h = haystack;
        const char* n = needle;
        
        while (*h && *n && *h == *n) {
            h++;
            n++;
        }
        
        if (!*n) {
            return (char*)haystack;
        }
        
        haystack++;
    }
    
    return NULL;
}

char* strrchr(const char* str, int c) {
    const char* last = NULL;
    while (*str) {
        if (*str == (char)c) {
            last = str;
        }
        str++;
    }
    if (c == '\0') {
        return (char*)str;
    }
    return (char*)last;
} 