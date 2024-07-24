struct IObject;

typedef struct {
    const struct IObject *interface;
    void *target;
} Object;

const Object Object_NULL = {
    .interface = NULL,
    .target = NULL
};

bool Object_isNull(Object this) {
    return this.interface == NULL;    
}

typedef struct IObject {
    void (*destroy)(void *this, Allocator allocator);
    Object (*copy)(void *this, Allocator allocator);
    void (*repr)(void *this, OutStream os);
} IObject;


void Object_destroy(Object this, Allocator allocator) {
    this.interface->destroy(this.target, allocator);
}

void Object_destroy_maybe(Object this, Allocator allocator) {
    if (Object_isNull(this)) {
        return;
    }
    Object_destroy(this, allocator);
}

Object Object_copy(Object this, Allocator allocator) {
    return this.interface->copy(this.target, allocator);
}

Object Object_copy_maybe(Object this, Allocator allocator) {
    if (Object_isNull(this)) {
        return Object_NULL;
    }
    return Object_copy(this, allocator);
}

void Object_repr(Object this, OutStream os) {
    this.interface->repr(this.target, os);
}

typedef struct {
    Type type;
    Object object;
} TypedObject;

const TypedObject TypedObject_NULL = {
    .type = Type_NULL,
    .object = Object_NULL
};

TypedObject TypedObject_copy(TypedObject this, Allocator allocator) {
    return (TypedObject) {
        .type = Type_copy(this.type, allocator),
        .object = Object_copy(this.object, allocator)
    };
}

void TypedObject_destroy(TypedObject this, Allocator allocator) {
    Type_destroy(this.type, allocator);
    Object_destroy(this.object, allocator);
}