#include "unistd.h"
#include "syscall.h"
#include "select.h"
#include "poll.h"
#include <stdint.h>

ssize_t read(int fd, void *buf, size_t count) {
    return (ssize_t)syscall(SYS_READ, (uintptr_t)fd, (uintptr_t)buf, (uintptr_t)count);
}

ssize_t write(int fd, const void *buf, size_t count) {
    return (ssize_t)syscall(SYS_WRITE, (uintptr_t)fd, (uintptr_t)buf, (uintptr_t)count);
}

int close(int fd) {
    return (int)syscall(SYS_CLOSE, (uintptr_t)fd, 0, 0);
}

pid_t fork(void) {
    return (pid_t)syscall(SYS_FORK, 0, 0, 0);
}

int execve(const char *pathname, char *const argv[], char *const envp[]) {
    return (int)syscall(SYS_EXEC, (uintptr_t)pathname, (uintptr_t)argv, (uintptr_t)envp);
}

pid_t getpid(void) {
    return (pid_t)syscall(SYS_GETPID, 0, 0, 0);
}

pid_t getppid(void) {
    return (pid_t)syscall(SYS_GETPPID, 0, 0, 0);
}

off_t lseek(int fd, off_t offset, int whence) {
    return (off_t)syscall(SYS_LSEEK, (uintptr_t)fd, (uintptr_t)offset, (uintptr_t)whence);
}

pid_t waitpid(pid_t pid, int *status, int options) {
    (void)options;
    return (pid_t)syscall(SYS_WAITPID, (uintptr_t)pid, (uintptr_t)status, 0);
}

unsigned int sleep(unsigned int seconds) {
    syscall(SYS_SLEEP, (uintptr_t)(seconds * 1000), 0, 0);
    return 0;
}

int usleep(unsigned int usec) {
    unsigned int ms = (usec + 999) / 1000;
    if (ms == 0) ms = 1;
    return (int)syscall(SYS_SLEEP, (uintptr_t)ms, 0, 0);
}

void *sbrk(int increment) {
    uintptr_t cur = (uintptr_t)syscall(SYS_BRK, 0, 0, 0);
    if (cur == UINTPTR_MAX) return (void*)-1;
    if (increment == 0) return (void*)cur;
    uintptr_t req = cur + (uintptr_t)increment;
    if (req < cur) return (void*)-1;
    uintptr_t new_brk = (uintptr_t)syscall(SYS_BRK, req, 0, 0);
    if (new_brk == UINTPTR_MAX) return (void*)-1;
    return (void*)cur;
}

int brk(void *addr) {
    uintptr_t ret = (uintptr_t)syscall(SYS_BRK, (uintptr_t)addr, 0, 0);
    return (ret == UINTPTR_MAX) ? -1 : 0;
}

char *getcwd(char *buf, int size) {
    intptr_t ret = syscall(SYS_GETCWD, (uintptr_t)buf, (uintptr_t)size, 0);
    if (ret == -1) return 0;
    return buf;
}

int chdir(const char *path) {
    return (int)syscall(SYS_CHDIR, (uintptr_t)path, 0, 0);
}

int ioctl(int fd, unsigned long cmd, void *arg) {
    return (int)syscall(SYS_IOCTL, (uintptr_t)fd, (uintptr_t)cmd, (uintptr_t)arg);
}

int mkdir(const char *pathname, int mode) {
    (void)mode;
    return (int)syscall(SYS_MKDIR, (uintptr_t)pathname, 0, 0);
}

int rmdir(const char *pathname) {
    return (int)syscall(SYS_RMDIR, (uintptr_t)pathname, 0, 0);
}

int pipe(int pipefd[2]) {
    return (int)syscall(SYS_PIPE, (uintptr_t)pipefd, 0, 0);
}

int dup2(int oldfd, int newfd) {
    return (int)syscall(SYS_DUP2, (uintptr_t)oldfd, (uintptr_t)newfd, 0);
}

uid_t getuid(void) {
    return (uid_t)__syscall0(SYS_GETUID);
}

uid_t geteuid(void) {
    return (uid_t)__syscall0(SYS_GETEUID);
}

gid_t getgid(void) {
    return (gid_t)__syscall0(SYS_GETGID);
}

gid_t getegid(void) {
    return (gid_t)__syscall0(SYS_GETEGID);
}

int setuid(uid_t uid) {
    return (int)__syscall1(SYS_SETUID, (uintptr_t)uid);
}

int setgid(gid_t gid) {
    return (int)__syscall1(SYS_SETGID, (uintptr_t)gid);
}

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout) {
    select_args_t args;
    args.nfds     = nfds;
    args.readfds  = readfds;
    args.writefds = writefds;
    args.exceptfds = exceptfds;
    args.timeout  = timeout;
    return (int)__syscall1(SYS_SELECT, (uintptr_t)&args);
}

int poll(struct pollfd *fds, int nfds, int timeout_ms) {
    return (int)__syscall3(SYS_POLL, (uintptr_t)fds, (uintptr_t)nfds, (uintptr_t)timeout_ms);
}