#ifndef _TIME_H
#define _TIME_H

typedef long time_t;

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

struct timeval {
    long tv_sec;
    long tv_usec;
};

struct timespec {
    long tv_sec;
    long tv_nsec;
};

#define CLOCK_REALTIME  0
#define CLOCK_MONOTONIC 1

time_t time(time_t *t);
struct tm *gmtime(const time_t *timep);
struct tm *localtime(const time_t *timep);
int gettimeofday(struct timeval *tv, void *tz);
int clock_gettime(int clkid, struct timespec *tp);
int nanosleep(const struct timespec *req, struct timespec *rem);

#endif
