#ifndef _ASSERT_H
#define _ASSERT_H

#include <stdio.h>

#ifndef NDEBUG
#define assert(x) do { if (!(x)) { fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", #x, __FILE__, __LINE__); abort(); } } while(0)
#else
#define assert(x) ((void)0)
#endif

#endif
