typedef struct {
    Allocator allocator;
    Scope scope;
} Namespace;

void Namespace_create(Namespace *this, Allocator allocator, Scope *global, Scope *parent) {
    this->allocator = allocator;
    Scope_create(&this->scope, allocator, global, parent);
}

void Namespace_destroy(Namespace *this) {
    Scope_destroy(&this->scope);
}