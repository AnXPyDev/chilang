typedef struct {
    Type type;
    char data[];
} LiteralExpression;

Expression LiteralExpression_upcast(LiteralExpression *this);

Expression LiteralExpression_create(Type type, const char *data, Allocator allocator) {
    TypeInfo ti = Type_info(type);
    LiteralExpression *copy = Allocator_malloc(allocator, sizeof(LiteralExpression) + ti.size);
    copy->type = Type_copy(type, allocator);
    memcpy(copy->data, data, ti.size);
    return LiteralExpression_upcast(copy);
}

#define this ((LiteralExpression*)vthis)

void LiteralExpression_destroy(void *vthis, Allocator allocator) {
    Type_destroy(this->type, allocator);
    Allocator_free(allocator, this);
}

Type LiteralExpression_type(void *vthis, Allocator allocator) {
    return Type_copy(this->type, allocator);
}

Expression LiteralExpression_copy(void *vthis, Allocator allocator) {
    return LiteralExpression_create(this->type, this->data, allocator);
}

void LiteralExpression_repr(void *vthis, OutStream os) {
    OutStream_puts(os, "Literal(");
    Type_repr(this->type, os);
    OutStream_putc(os, ')');
}

const IExpression ILiteralExpression = {
    .copy = &LiteralExpression_copy,
    .type = &LiteralExpression_type,
    .destroy = &LiteralExpression_destroy,
    .repr = &LiteralExpression_repr,
};

Expression LiteralExpression_upcast(LiteralExpression *this) {
    return (Expression) {
        .interface = &ILiteralExpression,
        .object = (void*)this
    };
}