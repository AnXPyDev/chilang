#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t size;
    char *data;
} Buffer;

#define LocalBuffer(sz) ((Buffer) { .size=(sz), .data=(char[sz]) { 0 }})


int main() {

    Buffer b = LocalBuffer(12);

    strcpy(b.data, "cigan");

    fprintf(stderr, "%s\n", b.data);

    return 0;
}