typedef struct {
    Allocator allocator;
    OutStream logStream;
    Scope globalScope;
} Parser;

typedef struct {
    int code;
    const char *message;
    void *payload;
} ParserResult;


const ParserResult ParserResult_Success = {
    .code = 0,
    .message = NULL,
    .payload = NULL
};

bool ParserResult_isSuccess(ParserResult result) {
    return result.code == 0;
}

void Parser_create(Parser *this) {
}

ParserResult Parser_parseUnit(Parser *this, Unit *unit, InStream in) {
    String buffer = String_new(this->allocator);
    String_blank(&buffer, 32);

    OutStream_puts(this->logStream, "Parsing unit\n");

    return ParserResult_Success;
}

ParserResult Parser_parseExpression(Parser *this, Namespace *unit) {
    return (ParserResult) {
        .code = 1,
        .message = "not implemented",
        .payload = NULL
    };
}