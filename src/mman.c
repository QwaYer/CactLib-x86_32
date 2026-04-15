#include "sys/mman.h"
#include "syscall.h"

/* mmap argument structure — must match kernel's mmap_args_t */
typedef struct {
    unsigned int addr;
    unsigned int length;
    int          prot;
    int          flags;
    int          fd;
    unsigned int offset;
} mmap_args_t;

void *mmap(void *addr, size_t length, int prot, int flags, int fd, unsigned int offset)
{
    mmap_args_t args;
    args.addr   = (unsigned int)addr;
    args.length = (unsigned int)length;
    args.prot   = prot;
    args.flags  = flags;
    args.fd     = fd;
    args.offset = offset;
    int ret = __syscall1(SYS_MMAP, (int)&args);
    if (ret < 0)
        return MAP_FAILED;
    return (void *)ret;
}

int munmap(void *addr, size_t length)
{
    return __syscall2(SYS_MUNMAP, (int)addr, (int)length);
}

int mprotect(void *addr, size_t length, int prot)
{
    return __syscall3(SYS_MPROTECT, (int)addr, (int)length, prot);
}
