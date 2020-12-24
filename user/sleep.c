#include "user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: sleep (num)\n");
        exit();
    }

    int num = atoi(argv[1]);

    sleep(num);
    printf("(nothing happens for a little while)\n");

    exit();
}