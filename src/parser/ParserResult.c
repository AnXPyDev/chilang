typedef enum {
    PARSER_CODE_SUCCESS = 0,
    PARSER_CODE_UNEXPECTED_CHAR,
    PARSER_CODE_TOKEN_TOO_LONG,
    PARSER_CODE__END
} EParserCode;

const char *ParserCode_REPRS[PARSER_CODE__END] = {
    [PARSER_CODE_SUCCESS] = "Success",
    [PARSER_CODE_UNEXPECTED_CHAR] = "Unexpected character",
    [PARSER_CODE_TOKEN_TOO_LONG] = "Token too long"
};

typedef struct {
    int code;
    const char *message;
    void *payload;
} ParserResult;


const ParserResult ParserResult_Success = {
    .code = PARSER_CODE_SUCCESS,
    .message = NULL,
    .payload = NULL
};

bool ParserResult_isSuccess(ParserResult result) {
    return result.code == PARSER_CODE_SUCCESS;
}