TYPE_DECL(struct, SimResult);

typedef enum {
    SIM_OK = 0,
    SIM_ERROR,
    SIM_UNSUPPORTED_EXPRESSION,
    SIM_FRAME_MEMBER_NOT_FOUND,
    SIM_INVALID_LITERAL,
} ESimResult;

struct SimResult {
    ESimResult code;
    char *message;
    void *payload;
};

SimResult SimResult_create(ESimResult code, char *message, void *payload) {
    return (SimResult) {
        .code = code,
        .message = message,
        .payload = payload
    };
}

const SimResult SimResult_OK = {
    .code = SIM_OK,
    .message = NULL,
    .payload = NULL
};

bool SimResult_isSuccess(SimResult this) {
    return this.code == SIM_OK;
}