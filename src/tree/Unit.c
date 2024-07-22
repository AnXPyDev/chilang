typedef struct {
    Allocator allocator;
    Namespace namespace;
    Expression root;
} Unit;

void Unit_create(Unit *this, Allocator allocator) {
    this->allocator = allocator;
    this->root = Expression_NULL;
    Namespace_create(&this->namespace, NULL, allocator);
}

void Unit_destroy(Unit *this) {
    Namespace_destroy(&this->namespace);
    if (!Expression_isNull(this->root)) {
       Expression_destroy(this->root, this->allocator);
    }
}