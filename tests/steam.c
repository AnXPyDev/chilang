#include <lib/include.h>

int main() {


    StringInStream sis = StringInStream_new(strview("abc"));
    InStream is = StringInStream_upcast(&sis);

    while (1) {
        int c = InStream_getc(is);
        fprintf(stderr, "%d %d\n", c, c == EOF);

        if (c == EOF) {
            break;
        }
    }

    InStream_close(is);
}