#include "dlfcn.h"

void *dlopen(const char *filename, int flags) {
    (void)filename; (void)flags;
    return 0;
}

int dlclose(void *handle) {
    (void)handle;
    return -1;
}

void *dlsym(void *handle, const char *symbol) {
    (void)handle; (void)symbol;
    return 0;
}

char *dlerror(void) {
    return "CactOS: dlopen not supported";
}
