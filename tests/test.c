#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    int i = atoi(argv[1]);

    void *x = &i;

    return 0;
}