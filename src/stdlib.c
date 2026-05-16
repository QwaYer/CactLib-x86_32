#include "stdlib.h"
#include "syscall.h"
#include "string.h"
#include <stdint.h>

void exit(int status) {
    syscall(SYS_EXIT, status, 0, 0);
    while(1);
}


#define BLOCK_MAGIC 0xA110CA7E
#define ALIGN8(x)  (((x) + 7) & ~7)
#define BLOCK_SIZE  sizeof(struct block_header)

struct block_header {
    unsigned int magic;
    unsigned int size;
    unsigned int is_free;
    struct block_header *next;
};

static struct block_header *free_list = 0;
static int heap_initialized = 0;

static void heap_init(void) {
    if (heap_initialized) return;
    heap_initialized = 1;
    free_list = 0;
}

static struct block_header *find_free_block(unsigned int size) {
    struct block_header *cur = free_list;
    while (cur) {
        if (cur->is_free && cur->size >= size)
            return cur;
        cur = cur->next;
    }
    return 0;
}

static struct block_header *request_space(unsigned int size) {
    unsigned int total = BLOCK_SIZE + size;
    unsigned int request = total;
    if (request < 4096) request = 4096;

    uintptr_t cur_brk = (uintptr_t)syscall(SYS_BRK, 0, 0, 0);
    if (cur_brk == UINTPTR_MAX) return 0;

    uintptr_t req_brk = cur_brk + (uintptr_t)request;
    if (req_brk < cur_brk) return 0;
    uintptr_t new_brk = (uintptr_t)syscall(SYS_BRK, req_brk, 0, 0);
    if (new_brk == UINTPTR_MAX) return 0;

    struct block_header *block = (struct block_header *)cur_brk;
    block->magic   = BLOCK_MAGIC;
    block->size    = request - BLOCK_SIZE;
    block->is_free = 0;
    block->next    = 0;

    if (block->size > size + BLOCK_SIZE + 16) {
        struct block_header *remainder = (struct block_header *)
            ((char *)block + BLOCK_SIZE + size);
        remainder->magic   = BLOCK_MAGIC;
        remainder->size    = block->size - size - BLOCK_SIZE;
        remainder->is_free = 1;
        remainder->next    = 0;
        block->size = size;
        block->next = remainder;

        if (!free_list) {
            free_list = block;
        } else {
            struct block_header *last = free_list;
            while (last->next) last = last->next;
            last->next = block;
        }
        return block;
    }

    if (!free_list) {
        free_list = block;
    } else {
        struct block_header *last = free_list;
        while (last->next) last = last->next;
        last->next = block;
    }
    return block;
}

void *malloc(size_t size) {
    if (size == 0) return 0;
    heap_init();

    unsigned int aligned = ALIGN8((unsigned int)size);

    struct block_header *block = find_free_block(aligned);
    if (block) {
        if (block->size > aligned + BLOCK_SIZE + 16) {
            struct block_header *remainder = (struct block_header *)
                ((char *)block + BLOCK_SIZE + aligned);
            remainder->magic   = BLOCK_MAGIC;
            remainder->size    = block->size - aligned - BLOCK_SIZE;
            remainder->is_free = 1;
            remainder->next    = block->next;
            block->size = aligned;
            block->next = remainder;
        }
        block->is_free = 0;
        return (void *)((char *)block + BLOCK_SIZE);
    }

    block = request_space(aligned);
    if (!block) return 0;
    block->is_free = 0;
    return (void *)((char *)block + BLOCK_SIZE);
}

void free(void *ptr) {
    if (!ptr) return;
    struct block_header *block = (struct block_header *)
        ((char *)ptr - BLOCK_SIZE);
    if (block->magic != BLOCK_MAGIC) return;
    block->is_free = 1;

    struct block_header *cur = free_list;
    while (cur) {
        if (cur->is_free && cur->next && cur->next->is_free) {
            cur->size += BLOCK_SIZE + cur->next->size;
            cur->next = cur->next->next;
            continue; 
        }
        cur = cur->next;
    }
}

void *calloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    void *ptr = malloc(total);
    if (ptr) memset(ptr, 0, total);
    return ptr;
}

void *realloc(void *ptr, size_t size) {
    if (!ptr) return malloc(size);
    if (size == 0) { free(ptr); return 0; }

    struct block_header *block = (struct block_header *)
        ((char *)ptr - BLOCK_SIZE);
    if (block->magic != BLOCK_MAGIC) return 0;
    if (block->size >= (unsigned int)size) return ptr;

    void *new_ptr = malloc(size);
    if (!new_ptr) return 0;
    memcpy(new_ptr, ptr, block->size);
    free(ptr);
    return new_ptr;
}

double strtod(const char *str, char **endptr) {
    double result = 0.0;
    int sign = 1, i = 0;
    while (str[i] == ' ') i++;
    if (str[i] == '-') { sign = -1; i++; }
    else if (str[i] == '+') i++;
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10.0 + (str[i] - '0');
        i++;
    }
    if (str[i] == '.') {
        i++;
        double frac = 1.0;
        while (str[i] >= '0' && str[i] <= '9') {
            frac /= 10.0;
            result += frac * (str[i] - '0');
            i++;
        }
    }
    if (endptr) *endptr = (char*)(str + i);
    return sign * result;
}

double atof(const char *str) {
    return strtod(str, 0);
}

unsigned long strtoul(const char *str, char **endptr, int base) {
    unsigned long result = 0;
    int i = 0;
    while (str[i] == ' ') i++;
    if (base == 0) {
        if (str[i] == '0') {
            if (str[i+1] == 'x' || str[i+1] == 'X') { base = 16; i += 2; }
            else base = 8;
        } else {
            base = 10;
        }
    }
    if (base == 16 && str[i] == '0' && (str[i+1] == 'x' || str[i+1] == 'X')) i += 2;
    while (str[i]) {
        int d;
        if (str[i] >= '0' && str[i] <= '9') d = str[i] - '0';
        else if (str[i] >= 'a' && str[i] <= 'f') d = str[i] - 'a' + 10;
        else if (str[i] >= 'A' && str[i] <= 'F') d = str[i] - 'A' + 10;
        else break;
        if (d >= base) break;
        result = result * base + d;
        i++;
    }
    if (endptr) *endptr = (char*)(str + i);
    return result;
}

static void _qsort_swap(void *a, void *b, size_t size) {
    unsigned char tmp[64];
    unsigned char *pa = (unsigned char*)a, *pb = (unsigned char*)b;
    for (size_t i = 0; i < size; i += sizeof(tmp)) {
        size_t chunk = size - i;
        if (chunk > sizeof(tmp)) chunk = sizeof(tmp);
        for (size_t j = 0; j < chunk; j++) tmp[j] = pa[i+j];
        for (size_t j = 0; j < chunk; j++) pa[i+j] = pb[i+j];
        for (size_t j = 0; j < chunk; j++) pb[i+j] = tmp[j];
    }
}

void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    if (nmemb <= 1) return;
    char *arr = (char*)base;
    size_t last = nmemb - 1;
    size_t pivot = 0;
    for (size_t i = 0; i < last; i++) {
        if (compar(arr + i * size, arr + last * size) < 0) {
            _qsort_swap(arr + i * size, arr + pivot * size, size);
            pivot++;
        }
    }
    _qsort_swap(arr + pivot * size, arr + last * size, size);
    if (pivot > 1) qsort(arr, pivot, size, compar);
    if (pivot < nmemb - 1) qsort(arr + (pivot + 1) * size, nmemb - pivot - 1, size, compar);
}

long strtol(const char *str, char **endptr, int base) {
    int i = 0, sign = 1;
    while (str[i] == ' ') i++;
    if (str[i] == '-') { sign = -1; i++; }
    else if (str[i] == '+') i++;
    return (long)strtoul(str + i, endptr, base) * sign;
}
char *realpath(const char *path, char *resolved_path) {
    (void)path;
    if (resolved_path) resolved_path[0] = '\0';
    return 0;
}

char *getenv(const char *name) {
    (void)name;
    return 0;
}

unsigned long long strtoull(const char *str, char **endptr, int base) {
    unsigned long long result = 0;
    int i = 0;
    while (str[i] == ' ') i++;
    if (base == 16 && str[i] == '0' && (str[i+1] == 'x' || str[i+1] == 'X')) i += 2;
    while (str[i]) {
        int d;
        if (str[i] >= '0' && str[i] <= '9') d = str[i] - '0';
        else if (str[i] >= 'a' && str[i] <= 'f') d = str[i] - 'a' + 10;
        else if (str[i] >= 'A' && str[i] <= 'F') d = str[i] - 'A' + 10;
        else break;
        if (d >= base) break;
        result = result * base + d;
        i++;
    }
    if (endptr) *endptr = (char*)(str + i);
    return result;
}

long long strtoll(const char *str, char **endptr, int base) {
    int i = 0, sign = 1;
    while (str[i] == ' ') i++;
    if (str[i] == '-') { sign = -1; i++; }
    else if (str[i] == '+') i++;
    return (long long)strtoull(str + i, endptr, base) * sign;
}

long double strtold(const char *str, char **endptr) {
    return (long double)strtod(str, endptr);
}
