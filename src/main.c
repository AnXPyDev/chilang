#include <lib/include.h>
#include <tree/include.h>
#include <parser/include.h>
#include <language/include.h>

int main ( int argc, char **argv ) {
    std_streams_init();

    OutStream logStream = os_stderr;

    OutStream_puts(logStream, "chi <3\n");

    Allocator allocator = standardAllocator;

    Scope globalScope;
    Scope_create(&globalScope, allocator, NULL, NULL);
    GlobalScope_init(&globalScope);

    Parser parser = {
        .allocator = allocator,
        .logStream = os_stderr,
        .globalScope = &globalScope
    };

    Parser_create(&parser);

    Unit unit;

    ParserResult result = Parser_parseUnit(&parser, is_stdin, strview("stdin"), &unit);

    if (ParserResult_isSuccess(result)) {
        Scope_repr(&unit.scope, logStream);
        OutStream_putc(logStream, '\n');
        Expression_repr(unit.root, logStream);
        OutStream_putc(logStream, '\n');
    } else {
        ParserResult_repr(result, logStream);
        OutStream_putc(logStream, '\n');
    }


    ParserResult_destroy(result, &parser);
    Parser_destroy(&parser);

    return 0;
}