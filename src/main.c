#include <lib/include.h>
#include <tree/include.h>
#include <parser/include.h>
#include <language/include.h>

int main ( int argc, char **argv ) {
    std_streams_init();

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

    ParserResult result = Parser_parseUnit(&parser, &unit, os_stdin);

    if (!ParserResult_isSuccess(result)) {
        OutStream_puts(logStream, "Parser failed: ");
        OutStream_puts(logStream, result.message);
        OutStream_putc(logStream, '\n');
    }

    return 0;
}