#include "user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        exit();
    }
    
    char buf[32][32];
    char line[512];
    char *params[32];

    for (int i = 0; i < 32; i++) {
        params[i] = buf[i];
    }

    for (int i = 1; i < argc; i++) {
        strcpy(buf[i-1], argv[i]);
    }

    int n;
    while ((n = read(0, line, sizeof(line))) > 0) {
        int pos = argc - 1;
        char *c = buf[pos];
        for (int i = 0; i < n; i++) {
            if (line[i] == ' ' || line[i] == '\n') {
                *c = '\0';
                pos++;
                c = buf[pos];
            } else {
                *c++ = line[i];
            }
        }
        params[pos] = 0;

        if (!fork()) {
            exec(params[0], params);
        } else {
            wait();
        }
    }

    if (n < 0) {
        printf("xargs: read error\n");
        exit();
    }

    exit();
}