#include "signal.h"
#include "syscall.h"
#include <stdint.h>

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) {
    (void)oldact;
    if (!act || signum <= 0 || signum >= KERNEL_NSIG) return -1;
    return (int)syscall(SYS_SIGACTION, (uintptr_t)(unsigned)signum,
                        (uintptr_t)act->sa_handler, 0);
}

int sigreturn(void) {
    return (int)syscall(SYS_SIGRETURN, 0, 0, 0);
}

/* POSIX номера или «сырой» индекс 1..12; либо маска ровно с одним битом (как SIGINT). */
static int sig_number_to_sigindex(int signum) {
    unsigned u = (unsigned)signum;
    if (u > 0 && (u & (u - 1u)) == 0u) {
        int bit = 0;
        unsigned t = u;
        while (t > 1u) {
            t >>= 1;
            bit++;
        }
        if (bit >= 1 && bit < KERNEL_NSIG) return bit;
        return -1;
    }
    switch (signum) {
    case 1:  return 10;
    case 2:  return 11;
    case 3:  return 12;
    case 13: return 4;
    case 14: return 5;
    case 15: return 1;
    case 17: return 6;
    case 18: return 3;
    case 19: return 2;
    default:
        if (signum >= 1 && signum < KERNEL_NSIG) return signum;
        return -1;
    }
}

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
    struct sigaction sa;
    int idx = sig_number_to_sigindex(signum);
    if (idx < 0) return SIG_ERR;
    sa.sa_handler = handler;
    if (sigaction(idx, &sa, NULL) < 0) return SIG_ERR;
    return handler;
}

int kill(pid_t pid, int sig) {
    return (int)syscall(SYS_KILL, (uintptr_t)pid, (uintptr_t)sig, 0);
}