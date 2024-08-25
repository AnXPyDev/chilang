typedef struct {
    Member *member;
} GetExpression;

Expression GetExpresison_upcast(GetExpression *this);

Expression GetExpression_create(Member *member, Allocator allocator) {
    GetExpression *this = Allocator_malloc(allocator, sizeof(GetExpression));
    *this = (GetExpression) {
        .member = member
    };
    return GetExpresison_upcast(this);
}

#define this ((GetExpression*)vthis)

void GetExpression_destroy(void *vthis, Allocator allocator) {
    Allocator_free(allocator, vthis);
}

Expression GetExpression_copy(void *vthis, Allocator allocator) {
    return GetExpression_create(this->member, allocator);
}

void GetExpression_repr(void *vthis, OutStream os) {
    OutStream_write(os, Buffer_view(this->member->token));
}

Type GetExpression_type(void *vthis, Allocator allocator) {
    return Type_copy(this->member->type, allocator);
}

#undef this

const IExpression IGetExpression = {
    .id = EXPRESSION_GET,
    .destroy = &GetExpression_destroy,
    .repr = &GetExpression_repr,
    .type = &GetExpression_type,
    .copy = &GetExpression_copy,
};

Expression GetExpresison_upcast(GetExpression *this) {
    return (Expression) {
        .interface = &IGetExpression,
        .object = this
    };
}