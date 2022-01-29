#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void displayTime(int h, int m, int s);
void *stopTimer();
int timer(int h, int m, int s);

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int stop = 0;

void displayTime(int h, int m, int s) {
    printf("%1d:%1d:%1d\n", h, m, s);
}

int timer(int h, int m, int s) {
    displayTime(h,m,s);

    while (h >= 0) {
        while (m >= 0) {
            while (s >= 0) {
                // pause listens to shared variable from another thread
                pthread_mutex_lock(&lock);
                if (stop) {
                    pthread_cond_wait(&cond, &lock);
                }
                pthread_mutex_unlock(&lock);

                displayTime(h,m,s);
                sleep(1);
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

void *stopTimer() {
    while (1) {
        if(getchar() == '\n') {
            pthread_mutex_lock(&lock);

            if (stop) {
                stop = 0;
            } else {
                stop = 1;
                printf("Paused\n");
            }

            pthread_mutex_unlock(&lock);
            pthread_cond_signal(&cond);
        }
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

        pthread_t t1;
        pthread_create(&t1, NULL, &stopTimer, NULL);

        timer(h,m,s);

        return 0;
    }

    printf("Too few arguments\nExpected: hours:minutes:seconds\n");
    return 1;
}
