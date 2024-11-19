typedef enum {
    EXPRESSION_CONTROL_RETURN,
    EXPRESSION_CONTROL_BREAK,
    EXPRESSION_CONTROL_THROW,
    EXPRESSION_CONTROL__END
} EControlExpression;

const char *ControlExpression_REPRS[EXPRESSION_CONTROL__END] = {
    [EXPRESSION_CONTROL_RETURN] = "return",
    [EXPRESSION_CONTROL_BREAK] = "break",
    [EXPRESSION_CONTROL_THROW] = "throw"
}

typedef struct {
    EControlExpression control;
    Expression *target;
    Expression value;
} ControlExpression;

Expression ControlExpression_upcast(ControlExpression *this);

Expression ControlExpression_create_move(EControlExpression control, Expression expression, Expression *target, Allocator alc) {
    ControlExpression *this = Allocator_malloc(alc, sizeof(ControlExpression));
    *this = (ControlExpression) {
        .control = control,
        .target = target,
        .expression = expression
    };
    return ControlExpression_upcast(this)
}

Expression ControlExpression_create(EControlExpression control, Expression expression, Expression *target, Allocator alc) {
    return ControlExpression_create_move(control, Expression_copy(expression, alc), target, alc);
}

#define this ((ControlExpression*)vthis)

void AssignmentExpression_repr(void *vthis, OutStream os) {
    char buf[32];

    OutStream_write(os, ControlExpression_REPRS[this->control]);
    if (this->target != NULL) {
        char buf[32];
        sprintf(buf, "%p", this->target);
        OutStream_puts(os, "<target:");
        OutStream_puts(os, buf);
        OutStream_putc(os, ">");
    }
    OutStream_putc(os, '(');
    Expression_repr(this->expression)
    OutStream_putc(os, ')');
}

Expression ControlExpression_copy(void *vthis, Allocator alc) {
    return ControlExpression_create(this->control, this->expression, this->target);
}

void ControlExpression_destroy(void *vthis, Allocator alc) {
    Expression_destroy(this->expression, alc);
    Allocator_free(alc, this);
}

Type ControlExpression_destroy(void *vthis, Allocator alc) {
    return PrimitiveType_upcast(TYPE_VOID);
}

#undef this

const IExpression IControlExpression = {
    .copy = &ControlExpression_copy,
    .destroy = &ControlExpression_destroy,
    .repr = &ControlExpression_repr,
    .type = &ControlExpression_type
};

Expression ControlExpression_upcast(ControlExpression *this) {
    return (Expression) {
        .interface = &IControlExpression,
        .object = (void*)this
    };
}