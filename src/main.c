#include "lib/include.h"
#include "tree/include.h"
#include "parser/include.h"

int main ( int argc, char **argv ) {
    fprintf(stderr, "Hello, Chi <3\n");

    FileOutStream _logStream = FileOutStream_new(stderr);
    FileInStream _inStream = FileInStream_new(stdin);
    OutStream logStream = FileOutStream_OutStream(&_logStream);
    InStream inStream = FileInStream_InStream(&_inStream);

    Parser parser = {
        .allocator = standardAllocator,
        .logStream = logStream
    };

    Parser_create(&parser);

    Unit unit;

    ParserResult result = Parser_parseUnit(&parser, &unit, inStream);

    if (!ParserResult_isSuccess(result)) {
        OutStream_puts(logStream, "Failed to parse expression: ");
        OutStream_puts(logStream, result.message);
        OutStream_putc(logStream, '\n');
    }

    return 0;
}