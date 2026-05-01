#include "time.h"
#include "syscall.h"
#include <stdint.h>

int gettimeofday(struct timeval *tv, void *tz) {
    return (int)__syscall2(SYS_GETTIMEOFDAY, (uintptr_t)tv, (uintptr_t)tz);
}

int clock_gettime(int clkid, struct timespec *tp) {
    return (int)__syscall2(SYS_CLOCK_GETTIME, (uintptr_t)clkid, (uintptr_t)tp);
}

int nanosleep(const struct timespec *req, struct timespec *rem) {
    return (int)__syscall2(SYS_NANOSLEEP, (uintptr_t)req, (uintptr_t)rem);
}
