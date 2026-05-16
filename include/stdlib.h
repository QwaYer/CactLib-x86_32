#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

void exit(int status);
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

int atoi(const char *str);
void itoa(int n, char str[]);
void hex_to_ascii(unsigned int n, char str[]);
double atof(const char *str);
double strtod(const char *str, char **endptr);
unsigned long strtoul(const char *str, char **endptr, int base);
long strtol(const char *str, char **endptr, int base);
long long strtoll(const char *str, char **endptr, int base);
unsigned long long strtoull(const char *str, char **endptr, int base);
void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

#endif
int atexit(void (*function)(void));
void abort(void);
char *getenv(const char *name);
int system(const char *command);
int abs(int x);
long labs(long x);
int rand(void);
void srand(unsigned seed);
typedef struct { int quot; int rem; } div_t;
typedef struct { long quot; long rem; } ldiv_t;
div_t div(int numer, int denom);
ldiv_t ldiv(long numer, long denom);
char *realpath(const char *path, char *resolved_path);
