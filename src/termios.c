#include "termios.h"
#include "unistd.h"

int tcgetattr(int fd, struct termios *t) {
    return ioctl(fd, TCGETS, t);
}

int tcsetattr(int fd, int action, const struct termios *t) {
    unsigned long cmd;
    if (action == TCSADRAIN)
        cmd = TCSETSW;
    else if (action == TCSAFLUSH)
        cmd = TCSETSF;
    else
        cmd = TCSETS;
    return ioctl(fd, cmd, (void *)t);
}