#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include "tc.h"

void display_time(int h, int m, int s) {
    printf("%d:%02d:%02d\n", h,m,s);
}

int fd_monitor() {
    int fd, sret;
    fd = 0;

    fd_set readfds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    return sret = select(8, &readfds, NULL, NULL, &timeout);
}

// TODO: find a better way to pause and unpause this
void pauseTimer() {
    while (1) {
        if (getchar() == 'p') {
            break;
        }
    }
}

int timer(int h, int m, int s) {
    int sret = 0;

    while (h >= 0) {
        while (m >= 0) {
            while (s >= 0) {
                sret = fd_monitor();
                if (sret == 0) {
                    display_time(h,m,s);
                } else {
                    pauseTimer();
                }
                s--;
            }
            s = 59;
            m--;
        }
        m = 59;
        h--;
    }
    return 0;
}

// TODO: handle negative number correctly
int main(int argc, char **argv) {
    int h,m,s = 0;

    if( argc >= 4 ) {
        int h = atoi(argv[1]);
        int m = atoi(argv[2]);
        int s = atoi(argv[3]);

        if ((m < 0 && 60 <= m) || (s < 0 && 60 <= s)) {
            printf("Minutes/Seconds out of bounds\n");
            return 1;
        }

        return timer(h,m,s);

    }

    printf("Too few arguments\nExpected: hours:minutes:seconds\n");
    return 1;
}
