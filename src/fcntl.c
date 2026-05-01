#include "fcntl.h"
#include "syscall.h"
#include <stdarg.h>
#include <stdint.h>

int open(const char *pathname, int flags, ...) {
    return (int)syscall(SYS_OPEN, (uintptr_t)pathname, (uintptr_t)flags, 0);
}

int fcntl(int fd, int cmd, ...) {
    va_list ap;
    va_start(ap, cmd);
    int arg = va_arg(ap, int);
    va_end(ap);
    return (int)syscall(SYS_FCNTL, (uintptr_t)fd, (uintptr_t)cmd, (uintptr_t)arg);
}
