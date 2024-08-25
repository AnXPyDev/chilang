#include <lib/include.h>
#include <tree/include.h>
#include <parser/include.h>
#include <language/include.h>
#include <simulator/include.h>

int main ( int argc, char **argv ) {
    int exitCode = 0;

    std_streams_init();

    OutStream logStream = os_stderr;

    OutStream_puts(logStream, "chi <3\n");

    ArenaAllocator arenaAllocator;
    ArenaAllocator_create(&arenaAllocator, standardAllocator, 1000000);

    Allocator allocator = ArenaAllocator_upcast(&arenaAllocator);

    MemberList globalFrame;
    MemberList_create(&globalFrame, allocator);
    GlobalFrame_init(&globalFrame, allocator);

    Parser parser = {
        .allocator = allocator,
        .logStream = logStream,
        .globalFrame = &globalFrame
    };

    Parser_create(&parser);

    Expression rootExpression = Expression_NULL;

    HPTimer_Measurement timer_start = HPTimer_now();

    ParserResult parserResult = Parser_parseUnit(&parser, is_stdin, strview("stdin"), &rootExpression);
    
    HPTimer_Duration timer = HPTimer_elapsed(timer_start);

    fprintf(stderr, "Parsing took %lfms\n", HPTimer_millis(timer));

    if (!ParserResult_isSuccess(parserResult)) {
        ParserResult_repr(parserResult, logStream);
        OutStream_putc(logStream, '\n');

        exitCode = 1;
        goto cleanup;
    }
    
    ParserResult_destroy(parserResult, &parser);
    Parser_destroy(&parser);

    Simulator simulator = {
        .allocator = allocator,
        .logStream = logStream,
        .os_stdout = os_stdout,
    };

    Simulator_create(&simulator, &globalFrame);
    SimValue *retVal = NULL;

    SimResult simResult = Simulator_evaluate(&simulator, rootExpression, &retVal);
    fprintf(stderr, "sim code: %d\n", simResult.code);

    if (!SimResult_isSuccess(simResult)) {
        exitCode = 1;
        goto cleanup;
    }

    cleanup:;
    ArenaAllocator_destroy(&arenaAllocator);

    return exitCode;
}