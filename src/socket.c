#include "socket.h"
#include "syscall.h"
#include <stdint.h>

int socket(int domain, int type, int protocol) {
    return (int)__syscall3(SYS_SOCKET, (uintptr_t)domain, (uintptr_t)type, (uintptr_t)protocol);
}

int bind(int fd, const struct sockaddr_in *addr, uint32_t addrlen) {
    return (int)__syscall3(SYS_BIND, (uintptr_t)fd, (uintptr_t)addr, (uintptr_t)addrlen);
}

int connect(int fd, const struct sockaddr_in *addr, uint32_t addrlen) {
    return (int)__syscall3(SYS_CONNECT, (uintptr_t)fd, (uintptr_t)addr, (uintptr_t)addrlen);
}

int listen(int fd, int backlog) {
    return (int)__syscall2(SYS_LISTEN, (uintptr_t)fd, (uintptr_t)backlog);
}

int accept(int fd, struct sockaddr_in *peer, uint32_t *addrlen) {
    return (int)__syscall3(SYS_ACCEPT, (uintptr_t)fd, (uintptr_t)peer, (uintptr_t)addrlen);
}

int send(int fd, const void *buf, uint32_t len, int flags) {
    /* flags not yet honoured by kernel — pass len in edx, flags ignored */
    (void)flags;
    return (int)__syscall3(SYS_SEND, (uintptr_t)fd, (uintptr_t)buf, (uintptr_t)len);
}

int recv(int fd, void *buf, uint32_t len, int flags) {
    (void)flags;
    return (int)__syscall3(SYS_RECV, (uintptr_t)fd, (uintptr_t)buf, (uintptr_t)len);
}

int sendto(int fd, const void *buf, uint32_t len, int flags,
           const struct sockaddr_in *dest, uint32_t addrlen) {
    sendto_args_t args = {
        .fd     = fd,
        .buf    = buf,
        .len    = len,
        .flags  = flags,
        .dest   = dest,
        .addrlen= addrlen,
    };
    return (int)__syscall1(SYS_SENDTO, (uintptr_t)&args);
}

int recvfrom(int fd, void *buf, uint32_t len, int flags,
             struct sockaddr_in *src, uint32_t *addrlen) {
    recvfrom_args_t args = {
        .fd      = fd,
        .buf     = buf,
        .len     = len,
        .flags   = flags,
        .src     = src,
        .addrlen = addrlen,
    };
    return (int)__syscall1(SYS_RECVFROM, (uintptr_t)&args);
}

int shutdown(int fd, int how) {
    return (int)__syscall2(SYS_SHUTDOWN, (uintptr_t)fd, (uintptr_t)how);
}

int setsockopt(int fd, int level, int optname,
               const void *optval, uint32_t optlen) {
    setsockopt_args_t args = {
        .fd      = fd,
        .level   = level,
        .optname = optname,
        .optval  = optval,
        .optlen  = optlen,
    };
    return (int)__syscall1(SYS_SETSOCKOPT, (uintptr_t)&args);
}

int getsockopt(int fd, int level, int optname,
               void *optval, uint32_t *optlen) {
    getsockopt_args_t args = {
        .fd      = fd,
        .level   = level,
        .optname = optname,
        .optval  = optval,
        .optlen  = optlen,
    };
    return (int)__syscall1(SYS_GETSOCKOPT, (uintptr_t)&args);
}
