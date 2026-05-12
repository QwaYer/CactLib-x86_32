#ifndef _SHM_H
#define _SHM_H

#include <stdint.h>

#define IPC_CREAT    0x0200
#define IPC_EXCL     0x0400
#define IPC_PRIVATE  0

/* Must match kernel SYSV-ish values in Cact rust_mm (shm_ctl). */
#define IPC_RMID     0
#define IPC_STAT     2

#define SHM_RDONLY   0x1000
#define SHM_RND      0x2000

int      shmget(int key, unsigned int size, int flags);
void*    shmat(int shmid, const void* shmaddr, int flags);
int      shmdt(const void* shmaddr);
int      shmctl(int shmid, int cmd, void* buf);

#endif