typedef struct {
    Size size;
    Expression *items;
} SequenceExpression;

Expression SequenceExpression_upcast(SequenceExpression *this);

#define this ((SequenceExpression*)vthis)

void SequenceExpression_destroy(void *vthis, Allocator allocator) {
    for (Size i = 0; i < this->size; i++) {
        Expression_destroy(this->items[i], allocator);
    }
    Allocator_free(allocator, this);
}

void SequenceExpression_repr(void *vthis, OutStream os) {
    for (Size i = 0; i < this->size; i++) {
        OutStream_begin_item(os);
        Expression_repr(this->items[i], os);
        OutStream_end_item(os);
    }
}

Expression SequenceExpression_copy(void *vthis, Allocator allocator) {
    SequenceExpression *copy = Allocator_malloc(allocator, sizeof(SequenceExpression));
    copy->size = this->size;
    copy->items = Allocator_malloc(allocator, sizeof(Expression) * this->size);

    for (Size i = 0; i < this->size; i++) {
        copy->items[i] = Expression_copy(this->items[i], allocator);
    }

    return SequenceExpression_upcast(copy);
}

#undef this

const IExpression ISequenceExpression = {
    .destroy = &SequenceExpression_destroy,
    .copy = &SequenceExpression_copy,
    .repr = &SequenceExpression_repr
};

Expression SequenceExpression_upcast(SequenceExpression *this) {
    return (Expression) {
        .interface = &ISequenceExpression,
        .object = (void*)this 
    };
}

