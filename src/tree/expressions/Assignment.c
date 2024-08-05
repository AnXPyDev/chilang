typedef struct {
    StringBuffer token;
    Expression expression;
} AssignmentExpression;

Expression AssignmentExpression_upcast(AssignmentExpression *this);

Expression AssignmentExpression_create_move(StringView token, Expression expression, Allocator alc) {
    AssignmentExpression *e = Allocator_malloc(alc, sizeof(AssignmentExpression));
    e->token = Buffer_copy(token, alc);
    e->expression = expression;
    return AssignmentExpression_upcast(e);
}

Expression AssignmentExpression_create(StringView token, Expression expression, Allocator alc) {
    return AssignmentExpression_create_move(token, Expression_copy(expression, alc), alc);
}

#define this ((AssignmentExpression*)vthis)

void AssignmentExpression_repr(void *vthis, OutStream os) {
    OutStream_write(os, Buffer_view(this->token));
    OutStream_putc(os, '=');
    Expression_repr(this->expression, os);
}

Expression AssignmentExpression_copy(void *vthis, Allocator alc) {
    return AssignmentExpression_create(Buffer_view(this->token), this->expression, alc);
}

void AssignmentExpression_destroy(void *vthis, Allocator alc) {
    Buffer_free(this->token, alc);
    Expression_destroy(this->expression, alc);
    Allocator_free(alc, vthis);
}

Type AssignmentExpression_type(void *vthis, Allocator alc) {
    return Expression_type(this->expression, alc);
}

#undef this

const IExpression IAssignmentExpression = {
    .copy = &AssignmentExpression_copy,
    .destroy = &AssignmentExpression_destroy,
    .repr = &AssignmentExpression_repr,
    .type = &AssignmentExpression_type
};

Expression AssignmentExpression_upcast(AssignmentExpression *this) {
    return (Expression) {
        .interface = &IAssignmentExpression,
        .object = (void*)this
    };
}