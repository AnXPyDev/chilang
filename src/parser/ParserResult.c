typedef enum {
    PARSER_CODE_SUCCESS = 0,
    PARSER_CODE_INTERNAL_ERROR,
    PARSER_CODE_UNEXPECTED_CHAR,
    PARSER_CODE_TOKEN_TOO_LONG,
    PARSER_CODE_TOKEN_UNKNOWN,
    PARSER_CODE_TOKEN_UNEXPECTED,
    PARSER_CODE_UNIMPLEMENTED,
    PARSER_CODE__END
} EParserCode;

const char *ParserCode_REPRS[PARSER_CODE__END] = {
    [PARSER_CODE_SUCCESS] = "Success",
    [PARSER_CODE_INTERNAL_ERROR] = "Internal error",
    [PARSER_CODE_UNEXPECTED_CHAR] = "Unexpected character",
    [PARSER_CODE_TOKEN_TOO_LONG] = "Token too long",
    [PARSER_CODE_TOKEN_UNKNOWN] = "Token unknown",
    [PARSER_CODE_TOKEN_UNEXPECTED] = "Token unexpected",
    [PARSER_CODE_UNIMPLEMENTED] = "Unimplemented"
};

typedef struct {
    StringView path;
    uint32_t line;
    uint32_t character;
    StringBuffer message;
} ParserResultDetails;

typedef struct {
    int code;
    ParserResultDetails *details;
} ParserResult;

ParserResult ParserResult_construct(Parser *parser, ParserInStream *stream, EParserCode code, StringView message) {
    ParserResultDetails *details = Allocator_new(parser->allocator, ParserResultDetails);
    *details = (ParserResultDetails) {
        .path = stream->path,
        .line = stream->line,
        .character = stream->character,
        .message = Buffer_copy(message, parser->allocator)
    };

    return (ParserResult) {
        .code = code,
        .details = details
    };
}

void ParserResult_destroy(ParserResult this, Parser *parser) {
    if (this.details == NULL) {
        return;
    }

    Buffer_free(this.details->message, parser->allocator);
    Allocator_free(parser->allocator, this.details);
}

const ParserResult ParserResult_Success = {
    .code = PARSER_CODE_SUCCESS
};

bool ParserResult_isSuccess(ParserResult result) {
    return result.code == PARSER_CODE_SUCCESS;
}

void ParserResult_repr(ParserResult result, OutStream os) {
    if (result.details == NULL) {
        OutStream_puts(os, ParserCode_REPRS[result.code]);
        return;
    }

    char location[16];
    snprintf(location, ARRSIZE(location), ":%u:%u: ", result.details->line, result.details->character);
    OutStream_write(os, result.details->path);
    OutStream_puts(os, location);
    if (!Buffer_isNull(result.details->message)) {
        OutStream_write(os, Buffer_view(result.details->message));
    } else {
        OutStream_puts(os, ParserCode_REPRS[result.code]);
    }
}