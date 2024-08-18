typedef struct {
    MemberList frame;
    Expression expression; 
} FrameExpression;

Expression FrameExpression_upcast(FrameExpression *expression);

FrameExpression *FrameExpression_create(Expression expression, Allocator alc) { 
    FrameExpression *this = Allocator_malloc(alc, sizeof(FrameExpression));
    MemberList_create(&this->frame, alc);
    this->expression = expression;
    return this;
}

#define this ((FrameExpression*)vthis)

Expression FrameExpression_copy(void *vthis, Allocator alc) {
    fprintf(stderr, "cannot copy frame expression\n");
    abort();
    return Expression_NULL;
}

void FrameExpression_repr(void *vthis, OutStream os) {
    MemberList_repr(&this->frame, os);
    Expression_repr(this->expression, os);
}

void FrameExpression_destroy(void *vthis, Allocator alc) {
    MemberList_destroy(&this->frame, alc);
    Expression_destroy(this->expression, alc);
    Allocator_free(alc, this);
}

Type FrameExpression_type(void *vthis, Allocator alc) {
    return Expression_type(this->expression, alc);
}

#undef this

const IExpression IFrameExpression = {
    .repr = &FrameExpression_repr,
    .destroy = &FrameExpression_destroy,
    .copy = &FrameExpression_copy,
    .type = &FrameExpression_type
};

Expression FrameExpression_upcast(FrameExpression *this) {
    return (Expression) {
        .interface = &IFrameExpression,
        .object = this
    };
}