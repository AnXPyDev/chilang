extern const IObject ITypeObject;

#define this ((Type*)vthis)

Object TypeObject_copy(void *vthis, Allocator allocator) {
    Type *type = Allocator_malloc(allocator, sizeof(Type));
    *type = Type_copy(*this, allocator);
    return (Object) {
        .interface = &ITypeObject,
        .target = (void*)type
    };
}

void TypeObject_destroy(void *vthis, Allocator allocator) {
    Type_destroy(*this, allocator);
    Allocator_free(allocator, this);
}

void TypeObject_repr(void *vthis, OutStream os) {
    Type_repr(*this, os);
}

#undef this

const IObject ITypeObject = {
    .copy = &TypeObject_copy,
    .destroy = &TypeObject_destroy,
    .repr = &TypeObject_repr
};

TypedObject TypeObject_create(Type type, Allocator allocator) {
    Type *copy = Allocator_malloc(allocator, sizeof(Type));
    *copy = Type_copy(type, allocator);

    return (TypedObject) {
        .type = PrimitiveType_upcast(TYPE_TYPE),
        .object = (Object) {
            .interface = &ITypeObject,
            .target = (void*)copy
        }
    };
}