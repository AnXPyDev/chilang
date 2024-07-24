#include <lib/include.h>
#include <tree/include.h>
#include <language/include.h>

int main(int argc, char **argv) {
    std_streams_init();

    Scope scope;
    Scope_create(&scope, standardAllocator, NULL, NULL);

    GlobalScope_init(&scope);

    Scope_repr(&scope, os_stderr);
    OutStream_putc(os_stderr, '\n');
}