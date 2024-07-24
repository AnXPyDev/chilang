typedef struct {
    Allocator allocator;
    OutStream logStream;
    Scope *globalScope;
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

void Parser_create(Parser *this) {}

bool Parser_isDelimiter(int c) {
    switch (c) {
        case ' ':
            return true;
    } 
}

ParserResult Parser_loadToken(Parser *this, Scope *this, InStream in) {
    return (ParserResult) {
        .code = 1,
        .message = "not implemented",
        .payload = NULL
    };
}

ParserResult Parser_parseUnit(Parser *this, Unit *unit, InStream in) {
    OutStream_puts(this->logStream, "Parsing unit\n");
    Unit_create(unit, this->allocator, this->globalScope);

    ParserResult r = Parser_loadToken

    
    return ParserResult_Success;
}