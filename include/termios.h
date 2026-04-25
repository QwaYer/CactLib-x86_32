#ifndef TERMIOS_H
#define TERMIOS_H

#include <unistd.h>

#define TCGETS  0x5401
#define TCSETS  0x5402
#define TCSETSW 0x5403
#define TCSETSF 0x5404

#define ISIG   0x0001
#define ICANON 0x0002
#define ECHO   0x0008
#define ECHOE  0x0010
#define ECHOK  0x0020
#define ECHONL 0x0040
#define OPOST  0x0001

#define TCSANOW   0
#define TCSADRAIN 1
#define TCSAFLUSH 2

#define VMIN  6
#define VTIME 5
#define NCCS  19

typedef unsigned int  tcflag_t;
typedef unsigned char cc_t;

struct termios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t     c_cc[NCCS];
};

int tcgetattr(int fd, struct termios *t);
int tcsetattr(int fd, int action, const struct termios *t);

#endif