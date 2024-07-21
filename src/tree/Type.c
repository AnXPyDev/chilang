typedef struct {
    bool valid : 1;
    bool meta : 1;
    bool size_known : 1;
    size_t size;
} TypeInfo;

typedef struct {
    void (*destroy)(void *this, Allocator allocator);
    void (*repr)(void *this, OutStream stream);
    TypeInfo (*info)(void *this);
} IType;

typedef struct {
    const IType *interface;
    void *object;
} Type;

void Type_destroy(Type this, Allocator allocator) {
    this.interface->destroy(this.object, allocator);
}

void Type_repr(Type this, OutStream stream) {
    this.interface->repr(this.object, stream);
}

TypeInfo Type_info(Type this) {
    return this.interface->info(this.object);
}