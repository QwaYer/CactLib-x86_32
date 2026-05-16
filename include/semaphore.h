#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

typedef volatile int sem_t;

#define sem_init(s, p, v) (*(s) = (v), 0)
#define sem_wait(s)       (--*(s), 0)
#define sem_post(s)       (++*(s), 0)
#define sem_destroy(s)    (0)

#endif
