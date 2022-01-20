#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <ncurses.h>

#define BOX_WIDTH 20
#define BOX_HEIGHT 5

#define BOX_OFFSETX (COLS - BOX_WIDTH) / 2
#define BOX_OFFSETY (LINES - BOX_HEIGHT) / 2

#define TEXT_OFFSETX (COLS/2-BOX_WIDTH/4) + 1
#define TEXT_OFFSETY LINES/2

WINDOW *timerWindow;

void initNcurses(void);
void drawBox(WINDOW *window);
void displayTime(int h, int m, int s);
int fdMonitor(void);
void pauseTimer(void);
int timer(int h, int m, int s);

void initNcurses(void) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    refresh();

    //drawBox(timerWindow);
}

void drawBox(WINDOW *window) {
    window = newwin(BOX_HEIGHT, BOX_WIDTH, BOX_OFFSETY, BOX_OFFSETX);

    box(window, 0 , 0);

    wrefresh(window);
    refresh();
}

void displayTime(int h, int m, int s) {
    mvprintw(TEXT_OFFSETY, TEXT_OFFSETX, "%d:%02d:%02d\n", h,m,s);
    //drawBox(timerWindow);
    refresh();
}

// TODO: only pause on certain keys
int fdMonitor(void) {
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
void pauseTimer(void) {
    while (1) {
        if (getchar() == 'p') {
            break;
        }
    }
}

int timer(int h, int m, int s) {
    int sret = 0;
    displayTime(h,m,s);

    while (h >= 0) {
        while (m >= 0) {
            while (s >= 0) {
                sret = fdMonitor();
                if (sret == 0) {
                    displayTime(h,m,s);
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

        initNcurses();
        timer(h,m,s);
        endwin();

        return 0;

    }

    printf("Too few arguments\nExpected: hours:minutes:seconds\n");
    return 1;
}
