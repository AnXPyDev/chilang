#include <lib/include.h>
#include <tree/include.h>
#include <language/include.h>

int main(int argc, char **argv) {
    std_streams_init();

    Scope scope;
    Scope_create(&scope, standardAllocator, NULL);

    GlobalScope_init(&scope);

    Scope_print(&scope, os_stderr);
}