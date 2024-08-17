typedef struct {
    Type result;
    Size argc;
    Type arguments[];
} CallableType;

Type CallableType_upcast(CallableType *this);

Type CallableType_create_move(Type result, Array arguments, Allocator alc) {
    CallableType *this = Allocator_malloc(alc, sizeof(CallableType) + sizeof(Type) * arguments.size);
    *this = (CallableType) {
        .result = result,
        .argc = arguments.size
    };
    memcpy(this->arguments, arguments.data, arguments.size * sizeof(Type));
    return CallableType_upcast(this);
}

Type CallableType_create(Type result, ArrayView arguments, Allocator alc) {
    CallableType *this = Allocator_malloc(alc, sizeof(CallableType) + sizeof(Type) * arguments.size);
    *this = (CallableType) {
        .result = result,
        .argc = arguments.size
    };

    for (Size i = 0; i < arguments.size; i++) {
        this->arguments[i] = Type_copy(arguments[i], alc);
    }

    return CallableType_upcast(this);
}

#define this ((CallableType*)vthis)

void CallableType_repr(void *vthis, OutStream os) {
    Type_repr(this->result, os);
    OutStream_putc(os, '(');
    DelimOS(os_delim, os, ", ");
    for (Size i = 0; i < this->argc; i++) {
        OutStream_beginItem(os_delim);
        Type_repr(this->arguments[i], os_delim);
        OutStream_endItem(os_delim);
    }
    OutStream_putc(os, ')');
}

void CallableType_copy(void *vthis, Allocator alc) {
    return CallableType_create(this->result, (ArrayView) {
        .size = this->argc,
        .data = this->arguments
    });
}

void CallableType_destroy(void *vthis, Allocator alc) {
    Type_destroy(this->result);

    for (Size i = 0; i < this->argc; i++) {
        Type_destroy(this->arguments[i], alc);
    }

    Allocator_free(alc, this);
}

bool CallableType_equal(void *vthis, void *other) {
    // TODO implement
    return false;
}

TypeInfo CallableType_info(void *vthis) {
    return (TypeInfo) {
        .valid = true,
        .size = 0,
        .size_known = true,
        .meta = true
    };
}


#undef this

const IType ICallableType = {
    .repr = &CallableType_repr,
    .copy = &CallableType_copy,
    .destroy = &CallableType_destroy,
    .equal = &CallableType_equal,
    .info = &CallableType_info
};

Type CallableType_upcast(CallableType *this) {
    return (Type) {
        .interface = &ICallableType,
        .object = this
    };
}
