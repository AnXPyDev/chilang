#include <stdio.h>

int main() {

    int x = 10737418;

    char *xp = (char*)&x;

    for (int i = 0; i < sizeof(x); i++) {
        fprintf(stdout, "%02X\n", (unsigned char)xp[i]);
    }

    fprintf(stdout, "\n");

}
