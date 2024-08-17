#include <stdio.h>

int main() {
    int a = ({
        int accum = 1; 
        for (int i = 0; i < 5; i++) {
            accum += i;
            if (accum > 5) {
                goto block_break;            
            }
        }

        block_break:;
        accum;
    });

    fprintf(stderr, "%d\n", a);
}