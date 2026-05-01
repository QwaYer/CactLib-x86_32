#include "stat.h"
#include "syscall.h"
#include <stdint.h>

int stat(const char *path, struct stat *buf) {
    return (int)syscall(SYS_STAT, (uintptr_t)path, (uintptr_t)buf, 0);
}

int fstat(int fd, struct stat *buf) {
    return (int)syscall(SYS_FSTAT, (uintptr_t)fd, (uintptr_t)buf, 0);
}

int chmod(const char *path, int mode) {
    return (int)syscall(SYS_CHMOD, (uintptr_t)path, (uintptr_t)mode, 0);
}

int chown(const char *path, int uid, int gid) {
    return (int)syscall(SYS_CHOWN, (uintptr_t)path, (uintptr_t)uid, (uintptr_t)gid);
}