typedef struct {
    bool valid : 1;
    bool meta : 1;
    bool size_known : 1;
    size_t size;
} TypeInfo;

struct Type;

typedef struct {
    void (*destroy)(void *this, Allocator allocator);
    struct Type (*copy)(void *this, Allocator allocator);
    void (*repr)(void *this, OutStream stream);
    TypeInfo (*info)(void *this);
} IType;

typedef struct Type {
    const IType *interface;
    void *object;
} Type;

const Type Type_NULL = {
    .interface = NULL,
    .object = NULL
};

void Type_destroy(Type this, Allocator allocator) {
    this.interface->destroy(this.object, allocator);
}

void Type_repr(Type this, OutStream stream) {
    this.interface->repr(this.object, stream);
}

Type Type_copy(Type this, Allocator allocator) {
    return this.interface->copy(this.object, allocator);
}

TypeInfo Type_info(Type this) {
    return this.interface->info(this.object);
}