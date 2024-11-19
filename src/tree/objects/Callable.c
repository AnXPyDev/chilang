extern const IObject ICallableObject;

#define this ((Callable*)vthis)

Object CallableObject_copy(void *vthis, Allocator alc) {
    // TODO
    return Object_NULL;
}

void CallableObject_destroy(void *vthis, Allocator alc) {
    // TODO 
}

void CallableObject_repr(void *vthis, Allocator alc) {
    Callable_repr(this);
}

#undef this

const IObject ICallableObject = {
    .copy = &CallableObject_copy,
    .destroy = &CallableObject_destroy,
    .repr = &CallableObject_repr
};

Object CallableObject_downcast(Callable *callable) {
    return (Object) {
        .interface = &ICallableObject,
        .target = (void*)callable
    };
}