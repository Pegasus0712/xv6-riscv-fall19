#include "user.h"

void filter(int *pin) {
    int n, buf, pout[2];
    pipe(pout);
    if (read(pin[0], &n, sizeof(int))) {
        printf("prime %d\n", n);
        close(pin[1]);
        if (!fork()) {
            filter(pout);
            exit();
        } else {
            close(pout[0]);
            while (read(pin[0], &buf, sizeof(int))) {
                if (buf % n != 0) {
                    write(pout[1], &buf, sizeof(int));
                }
            }
            close(pout[1]);
            wait();
        }
    }
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);

    if (!fork()) {
        filter(p);
        exit();
    } else {
        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait();
    }

    exit();
}