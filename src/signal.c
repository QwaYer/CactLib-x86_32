#include "signal.h"
#include "syscall.h"
#include <stdint.h>

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
    return (int)syscall(SYS_SIGPROCMASK, (uintptr_t)how, (uintptr_t)set, (uintptr_t)oldset);
}

int sigpending(sigset_t *set) {
    return (int)syscall(SYS_SIGPENDING, (uintptr_t)set, 0, 0);
}

int sigsuspend(const sigset_t *mask) {
    return (int)syscall(SYS_SIGSUSPEND, (uintptr_t)mask, 0, 0);
}

unsigned int alarm(unsigned int seconds) {
    return (unsigned int)syscall(SYS_ALARM, (uintptr_t)seconds, 0, 0);
}

int setitimer(int which, const struct itimerval *new_value,
              struct itimerval *old_value) {
    return (int)syscall(SYS_SETITIMER, (uintptr_t)which, (uintptr_t)new_value, (uintptr_t)old_value);
}

sighandler_t signal(int signum, sighandler_t handler) {
    return (sighandler_t)(uintptr_t)syscall(SYS_SIGNAL, (uintptr_t)signum, (uintptr_t)handler, 0);
}

int kill(pid_t pid, int sig) {
    return (int)syscall(SYS_KILL, (uintptr_t)pid, (uintptr_t)sig, 0);
}