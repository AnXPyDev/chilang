#include <stdio.h>

typedef struct {
    int x, y;
} Cigan;

Cigan newCigan() {
    return (Cigan) {
        .x = 5, .y = 10
    };
}

#define TEMP_LOCAL(T, expr) (((struct { T val; }) {.val = (expr)}).val)
#define CONST_CIGAN TEMP_LOCAL(Cigan, newCigan())

void ciganTest(Cigan *c) {
    fprintf(stderr, "%d %d\n", c->x, c->y);
}

int main() {
    ciganTest(&CONST_CIGAN);
}