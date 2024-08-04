#include <lib/include.h>
#include <tree/include.h>
#include <parser/include.h>
#include <language/include.h>

int main ( int argc, char **argv ) {
    std_streams_init();

    OutStream logStream = os_stderr;

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

    ParserResult result = Parser_parseUnit(&parser, is_stdin, strview("\\stdin"), &unit);

    if (!ParserResult_isSuccess(result)) {
        OutStream_puts(logStream, "Parser failed: ");
        OutStream_puts(logStream, ParserCode_REPRS[result.code]);
        OutStream_putc(logStream, '\n');
        return 1;
    }

    Scope_repr(&unit.scope, logStream);
    OutStream_putc(logStream, '\n');

    return 0;
}