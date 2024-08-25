typedef struct {
    Member *member;
    Expression expression;
} AssignmentExpression;

Expression AssignmentExpression_upcast(AssignmentExpression *this);

Expression AssignmentExpression_create_move(Member *member, Expression expression, Allocator alc) {
    AssignmentExpression *e = Allocator_malloc(alc, sizeof(AssignmentExpression));
    e->member = member;
    e->expression = expression;
    return AssignmentExpression_upcast(e);
}

Expression AssignmentExpression_create(Member *member, Expression expression, Allocator alc) {
    return AssignmentExpression_create_move(member, Expression_copy(expression, alc), alc);
}

#define this ((AssignmentExpression*)vthis)

void AssignmentExpression_repr(void *vthis, OutStream os) {
    OutStream_write(os, Buffer_view(this->member->token));
    OutStream_putc(os, '=');
    Expression_repr(this->expression, os);
}

Expression AssignmentExpression_copy(void *vthis, Allocator alc) {
    return AssignmentExpression_create(this->member, this->expression, alc);
}

void AssignmentExpression_destroy(void *vthis, Allocator alc) {
    Expression_destroy(this->expression, alc);
    Allocator_free(alc, vthis);
}

Type AssignmentExpression_type(void *vthis, Allocator alc) {
    return Expression_type(this->expression, alc);
}

#undef this

const IExpression IAssignmentExpression = {
    .id = EXPRESSION_ASSIGNMENT,
    .copy = &AssignmentExpression_copy,
    .destroy = &AssignmentExpression_destroy,
    .repr = &AssignmentExpression_repr,
    .type = &AssignmentExpression_type,
};

Expression AssignmentExpression_upcast(AssignmentExpression *this) {
    return (Expression) {
        .interface = &IAssignmentExpression,
        .object = (void*)this
    };
}