struct IExpression;

typedef struct {
    const struct IExpression *interface;
    void *object;
} Expression;

typedef struct IExpression {
    Expression (*copy)(void *this, Allocator allocator);
    void (*destroy)(void *this, Allocator allocator);
    void (*repr)(void *this, OutStream stream);
    Type (*type)(void *this, Allocator allocator);
} IExpression;

Expression Expression_copy(Expression this, Allocator allocator) {
    return this.interface->copy(this.object, allocator);
}

void Expression_destroy(Expression this, Allocator allocator) {
    this.interface->destroy(this.object, allocator);
}

void Expression_repr(Expression this, OutStream stream) {
    this.interface->repr(this.object, stream);
}

Type Expression_type(Expression this, Allocator allocator) {
    return this.interface->type(this.object, allocator);     
}

const Expression Expression_NULL = {
    .interface = NULL,
    .object = NULL
};

bool Expression_isNull(Expression this) {
    return this.interface == NULL;
}