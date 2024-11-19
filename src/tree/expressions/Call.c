typedef struct {
    Member *target;
    Expression expression;
} CallArgument;

typedef struct {
    Callable *callable;
    Size argc;
    CallArgument args[];
} CallExpression;


Expression CallExpression_upcast(CallExpression *this);

Expression CallExpression_create_move(Array args, Callable *callable, Allocator alc) {
    CallExpression *this = Allocator_malloc(alc, sizeof(CallExpression) + sizeof(CallArgument) * args.size);
    *this = (CallExpression) {
        .callable = callable,
        .argc = args.size,
    };
    for (Size i = 0; i < args.size; i++) {
        this->args[i] = ((CallArgument*)args.data)[i]
    }
    return CallExpression_upcast(this);
}

Expression CallExpression_create(ArrayView args, Callable *callable, Allocator alc) {

    CallExpression *this = Allocator_malloc(alc, sizeof(CallExpression) + sizeof(CallArgument) * args.size);
    CallArgument *copy_args = Allocator_malloc(alc, sizeof(CallArugment) * args.size);
    for (Size i = 0; i < args.size; i++) {
        const CallArgument *arg = &args.data[i]
        copy_args[i] = (CallArgument) {
            .target = arg.target,
            .expression = Expression_copy(arg.expression, alc)
        };
    }

    return CallExpression_create_move(Array_create(copy_args, args.size), callable, alc);
}

#define this ((CallExpression*)vthis)

Expression CallExpression_copy(void *vthis, Allocator alc) {
    return CalLExpression_create(ArrayView_create(this->args, this->argc), callable, alc);
}

void CallExpression_destroy(void *vthis, Allocator alc) {
    for (Size i = 0; i < this->argc; i++) {
        Expression_destroy(this->args[i].expression, alc);
    }
    Allocator_free(alc, this->args);
}

void CallExpression_repr(void *vthis, OutStream os) {
    char buf[32];
    sprintf(buf, "%p", this->callable);
    OutStream_puts(os, "call[")
    OutStream_puts(os, buf);
    OutStream_puts(os, "](ARGS)");
}

Type CallExpression_type(void *vthis, Allocator alc) {
    return Expression_type(this->callable->expression, alc);
}

#undef this