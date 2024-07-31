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