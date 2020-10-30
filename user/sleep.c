#include "user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        write(2, "Error message", strlen("Error message"));
    }

    int num = atoi(argv[1]);

    sleep(num);

    exit();
}