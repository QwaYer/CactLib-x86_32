#include "dirent.h"
#include "syscall.h"
#include <stdint.h>

int getdents(int fd, struct dirent *buf, unsigned int count) {
    return (int)syscall(SYS_GETDENTS, (uintptr_t)fd, (uintptr_t)buf, (uintptr_t)count);
}
