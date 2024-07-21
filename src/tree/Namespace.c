typedef struct {
    Allocator allocator;
    Scope scope;
} Namespace;

void Namespace_create(Namespace *this, Allocator allocator) {
    this->allocator = allocator;
}

