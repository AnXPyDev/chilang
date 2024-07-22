typedef struct {
    Allocator allocator;
    Scope scope;
} Namespace;

void Namespace_create(Namespace *this, Scope *parent, Allocator allocator) {
    this->allocator = allocator;
    Scope_create(&this->scope, allocator, parent);
}

void Namespace_destroy(Namespace *this) {
    Scope_destroy(&this->scope);
}