typedef struct {
    Allocator allocator;
    Scope scope;
    Expression root;
} Unit;

void Unit_create(Unit *this, Allocator allocator, Scope *global) {
    this->allocator = allocator;
    this->root = Expression_NULL;
    Scope_create(&this->scope, allocator, global, NULL);
}

void Unit_destroy(Unit *this) {
    Scope_destroy(&this->scope);
    if (!Expression_isNull(this->root)) {
       Expression_destroy(this->root, this->allocator);
    }
}