#ifndef _STDINT_H
#define _STDINT_H

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;

typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

#define UINTPTR_MAX 0xFFFFFFFFu

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

#define INT32_MIN  (-2147483647-1)
#define INT32_MAX  2147483647
#define UINT32_MAX 0xFFFFFFFFu

#endif
