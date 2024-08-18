#include <lib/include.h>
#include <tree/include.h>
#include <parser/include.h>
#include <language/include.h>

int main ( int argc, char **argv ) {
    std_streams_init();

    OutStream logStream = os_stderr;

    OutStream_puts(logStream, "chi <3\n");

    Allocator allocator = standardAllocator;

    MemberList globalFrame;
    MemberList_create(&globalFrame, allocator);
    GlobalFrame_init(&globalFrame, allocator);

    Parser parser = {
        .allocator = allocator,
        .logStream = os_stderr,
        .globalFrame = &globalFrame
    };

    Parser_create(&parser);

    Expression rootExpression = Expression_NULL;

    ParserResult result = Parser_parseUnit(&parser, is_stdin, strview("stdin"), &rootExpression);

    if (ParserResult_isSuccess(result)) {
        OutStream_puts(logStream, "\nprogram: \n");
        DelimOS(delim_os, logStream, strview("\n"));
        Expression_repr(rootExpression, delim_os);
        OutStream_putc(logStream, '\n');
    } else {
        ParserResult_repr(result, logStream);
        OutStream_putc(logStream, '\n');
    }


    ParserResult_destroy(result, &parser);
    Parser_destroy(&parser);

    return 0;
}