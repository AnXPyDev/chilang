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

Entity *Scope_get_entity(Scope *this, StringView token) {
    return Map_get(&this->entities, token);
}

Entity *Scope_ensure_entity(Scope *this, StringView token) {
    return Map_ensure(&this->entities, token);
}

void *Scope_destroy_entity(void *item, void *payload) {
    Scope *this = (Scope*)payload;
    Entity *entity = (Entity*)item;

    Entity_destroy(entity, this->allocator);
    return NULL;
}

void Scope_destroy(Scope *this) {
    Map_foreach(&this->entities, &Scope_destroy_entity, (void*)this);
    Map_destroy(&this->entities);
    Allocator_destroy(this->allocator);
}

