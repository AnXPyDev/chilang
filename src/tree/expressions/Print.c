typedef struct {
    Expression expression;
} PrintExpression;


Expression PrintExpression_upcast(PrintExpression *this);

Expression PrintExpression_create_move(Expression expression, Allocator allocator) {
    PrintExpression *this = Allocator_malloc(allocator, sizeof(PrintExpression));
    this->expression = expression;
    return PrintExpression_upcast(this);
}

Expression PrintExpression_create(Expression expression, Allocator allocator) {
    return PrintExpression_create_move(Expression_copy(expression, allocator), allocator);
}


#define this ((PrintExpression*)vthis)

void PrintExpression_repr(void *vthis, OutStream os) {
    OutStream_puts(os, "Print(");
    Expression_repr(this->expression, os);
    OutStream_putc(os, ')');
}

Expression PrintExpression_copy(void *vthis, Allocator allocator) {
    return PrintExpression_create(this->expression, allocator);
}

void PrintExpression_destroy(void *vthis, Allocator allocator) {
    Expression_destroy(this->expression, allocator);
    Allocator_free(allocator, vthis);
}

Type PrintExpression_type(void *vthis, Allocator allocator) {
    return PrimitiveType_upcast(TYPE_VOID);
}

#undef this

const IExpression IPrintExpression = {
    .id = EXPRESSION_PRINT,
    .repr = &PrintExpression_repr,
    .destroy = &PrintExpression_destroy,
    .copy = &PrintExpression_copy,
    .type = &PrintExpression_type
};

Expression PrintExpression_upcast(PrintExpression *this) {
    return (Expression) {
        .interface = &IPrintExpression,
        .object = (void*)this
    };
}