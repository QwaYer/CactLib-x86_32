#include "stat.h"
#include "syscall.h"

int stat(const char *path, struct stat *buf) {
    return syscall(SYS_STAT, (int)path, (int)buf, 0);
}

int fstat(int fd, struct stat *buf) {
    return syscall(SYS_FSTAT, fd, (int)buf, 0);
}

int chmod(const char *path, int mode) {
    return __syscall2(SYS_CHMOD, (int)path, mode);
}

int chown(const char *path, int uid, int gid) {
    return __syscall3(SYS_CHOWN, (int)path, uid, gid);
}