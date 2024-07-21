typedef struct {
    Allocator allocator;
    Namespace namespace;
    Expression root;
} Unit;

void Unit_create(Unit *this, Allocator allocator) {
    this->allocator = allocator;
    this->root = Expression_NULL;
    Namespace_create(&this->namespace, allocator);
}