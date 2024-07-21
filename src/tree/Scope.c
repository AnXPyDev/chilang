typedef struct Scope {
    Allocator allocator;
    struct Scope *parent;
    Map entities;
} Scope;

void Scope_create(Scope *this, Allocator allocator, Scope *parent) {
    this->parent = parent;
    this->allocator = allocator;
    this->entities = Map_new(allocator, sizeof(Entity));
}