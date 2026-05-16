#ifndef LIBC_H
#define LIBC_H

#include <stddef.h>

int strcmp(const char* s1, const char* s2);
int compare_string(const char* s1, const char* s2);
int strlen(const char* s);
char* strcat(char* dest, const char* src);
char* strcpy(char* dest, const char* src);
char* copy_string(char* dest, const char* src);


void* memory_set(void* dest, int val, int len);
void* memory_copy(void* dest, const void* src, int len);
int memory_compare(const void* s1, const void* s2, int n);

void* memset(void* dest, int val, unsigned int len);
void* memcpy(void* dest, const void* src, unsigned int len);
void* memmove(void* dest, const void* src, unsigned int len);
int   memcmp(const void* s1, const void* s2, unsigned int n);

int  strncmp(const char* a, const char* b, unsigned int n);
void strncpy(char* dst, const char* src, int n);
char* strchr(const char* s, int c);
char* strrchr(const char* s, int c);
size_t strspn(const char* s, const char* accept);
size_t strcspn(const char* s, const char* reject);
char* strstr(const char* haystack, const char* needle);
char* strpbrk(const char* s, const char* accept);
int  buf_append(char* buf, int pos, int max, const char* s);
int  buf_append_int(char* buf, int pos, int max, int n);

#endif