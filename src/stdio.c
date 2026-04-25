#include "stdio.h"
#include "syscall.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include <stdarg.h>


void kprint(const char *s) {
    syscall(SYS_PRINT, (int)s, 0, 0);
}

int putchar(int c) {
    char buf = (char)c;
    write(STDOUT_FILENO, &buf, 1);
    return c;
}

int puts(const char *str) {
    int len = strlen(str);
    write(STDOUT_FILENO, str, len);
    char nl = '\n';
    write(STDOUT_FILENO, &nl, 1);
    return 0;
}

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    int count = 0;
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            if (format[i] == 'd') {
                int val = va_arg(args, int);
                char buf[32];
                itoa(val, buf);
                int len = strlen(buf);
                write(STDOUT_FILENO, buf, len);
                count += len;
            } else if (format[i] == 's') {
                char *val = va_arg(args, char *);
                int len = strlen(val);
                write(STDOUT_FILENO, val, len);
                count += len;
            } else if (format[i] == 'x') {
                unsigned int val = va_arg(args, unsigned int);
                char buf[32];
                hex_to_ascii(val, buf);
                int len = strlen(buf);
                write(STDOUT_FILENO, buf, len);
                count += len;
            } else if (format[i] == 'c') {
                int val = va_arg(args, int);
                putchar(val);
                count++;
            } else if (format[i] == '%') {
                putchar('%');
                count++;
            }
        } else {
            putchar(format[i]);
            count++;
        }
    }

    va_end(args);
    return count;
}

int rename(const char *oldpath, const char *newpath) {
    return syscall(SYS_RENAME, (int)oldpath, (int)newpath, 0);
}