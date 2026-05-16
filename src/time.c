#include "time.h"
#include "syscall.h"
#include <stdint.h>

int gettimeofday(struct timeval *tv, void *tz) {
    return (int)__syscall2(SYS_GETTIMEOFDAY, (uintptr_t)tv, (uintptr_t)tz);
}

int clock_gettime(int clkid, struct timespec *tp) {
    return (int)__syscall2(SYS_CLOCK_GETTIME, (uintptr_t)clkid, (uintptr_t)tp);
}

int nanosleep(const struct timespec *req, struct timespec *rem) {
    return (int)__syscall2(SYS_NANOSLEEP, (uintptr_t)req, (uintptr_t)rem);
}

static struct tm __gmt;

time_t time(time_t *t) {
    struct timeval tv;
    if (gettimeofday(&tv, 0) < 0) return (time_t)-1;
    if (t) *t = tv.tv_sec;
    return tv.tv_sec;
}

struct tm *gmtime(const time_t *timep) {
    time_t t = *timep;
    int days = t / 86400;
    int rem = t % 86400;
    __gmt.tm_sec = rem % 60; rem /= 60;
    __gmt.tm_min = rem % 60; rem /= 60;
    __gmt.tm_hour = rem;
    __gmt.tm_wday = (days + 4) % 7;
    int y, m;
    for (y = 1970;; y++) {
        int leap = (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
        int d = leap ? 366 : 365;
        if (days < d) break;
        days -= d;
    }
    static const int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    for (m = 0; m < 12; m++) {
        int d = mdays[m];
        if (m == 1 && (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0))) d = 29;
        if (days < d) break;
        days -= d;
    }
    __gmt.tm_year = y - 1900;
    __gmt.tm_mon = m;
    __gmt.tm_mday = days + 1;
    __gmt.tm_yday = t / 86400;
    __gmt.tm_isdst = 0;
    return &__gmt;
}

struct tm *localtime(const time_t *timep) {
    return gmtime(timep);
}
