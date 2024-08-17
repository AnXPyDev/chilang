typedef struct {
    Type type;
    StringBuffer token;
} GetExpression;

Expression GetExpresison_upcast(GetExpression *this);

Expression GetExpression_create(Type type, StringView token, Allocator allocator) {
    GetExpression *this = Allocator_malloc(allocator, sizeof(GetExpression));
    *this = (GetExpression) {
        .type = Type_copy(type, allocator),
        .token = Buffer_copy(token, allocator)
    };
    return GetExpresison_upcast(this);
}

#define this ((GetExpression*)vthis)

void GetExpression_destroy(void *vthis, Allocator allocator) {
    Type_destroy(this->type, allocator);
    Buffer_free(this->token, allocator);
    Allocator_free(allocator, vthis);
}

Expression GetExpression_copy(void *vthis, Allocator allocator) {
    return GetExpression_create(this->type, Buffer_view(this->token), allocator);
}

void GetExpression_repr(void *vthis, OutStream os) {
    OutStream_write(os, Buffer_view(this->token));
}

Type GetExpression_type(void *vthis, Allocator allocator) {
    return Type_copy(this->type, allocator);
}

#undef this

const IExpression IGetExpression = {
    .destroy = &GetExpression_destroy,
    .repr = &GetExpression_repr,
    .type = &GetExpression_type,
    .copy = &GetExpression_copy
};

Expression GetExpresison_upcast(GetExpression *this) {
    return (Expression) {
        .interface = &IGetExpression,
        .object = this
    };
}