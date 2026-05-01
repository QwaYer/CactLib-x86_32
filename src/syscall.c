#include "syscall.h"

intptr_t syscall(int num, uintptr_t p1, uintptr_t p2, uintptr_t p3) {
    intptr_t ret;
    __asm__ volatile(
        "int $0x80"
        : "=a"(ret)
        : "a"(num), "b"(p1), "c"(p2), "d"(p3)
        : "memory"
    );
    return ret;
}
