#include "shm.h"
#include "syscall.h"
#include <stdint.h>

int shmget(int key, unsigned int size, int flags) {
    return (int)syscall(SYS_SHMGET, (uintptr_t)key, (uintptr_t)size, (uintptr_t)flags);
}

void* shmat(int shmid, const void* shmaddr, int flags) {
    intptr_t ret = syscall(SYS_SHMAT, (uintptr_t)shmid, (uintptr_t)shmaddr, (uintptr_t)flags);
    if (ret == -1) return (void*)-1;
    return (void*)(uintptr_t)ret;
}

int shmdt(const void* shmaddr) {
    return (int)syscall(SYS_SHMDT, (uintptr_t)shmaddr, 0, 0);
}

int shmctl(int shmid, int cmd, void* buf) {
    return (int)syscall(SYS_SHMCTL, (uintptr_t)shmid, (uintptr_t)cmd, (uintptr_t)buf);
}