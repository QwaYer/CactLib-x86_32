#include "syscall.h"
#include <stdint.h>

int dns_resolve(const char *name, uint32_t *out_ip_host) {
    return (int)__syscall2(SYS_DNS_RESOLVE, (uintptr_t)name, (uintptr_t)out_ip_host);
}
