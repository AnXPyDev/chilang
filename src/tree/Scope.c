typedef struct Scope {
    Allocator allocator;
    struct Scope *global;
    struct Scope *parent;
    Map entities;
} Scope;

void Scope_create(Scope *this, Allocator allocator, Scope *parent) {
    this->parent = parent;
    this->allocator = allocator;
    this->entities = Map_new(allocator, sizeof(Entity));
}

Entity *Scope_get_entity(Scope *this, StringView token) {
    Entity *entity = Map_get(&this->entities, token);
    
    if (entity == NULL && this->parent != NULL) {
        entity = Scope_get_entity(this->parent, token);
    }

    if (entity == NULL && this->global != NULL) {
        entity = Scope_get_entity(this->global, token);
    }
    
    return entity;
}

Entity *Scope_add_entity(Scope *this, StringView token) {
    return Map_add(&this->entities, token);
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

void *Scope_print_entity(CDataBuffer token, void *item, void *payload) {
    OutStream os = *(OutStream*)payload;
    Entity *entity = (Entity*)item;
    OutStream_puts(os, "  ");
    Type_repr(entity->type, os);
    OutStream_putc(os, ' ');
    OutStream_write(os, token);
    OutStream_puts(os, " = ");
    Entity_repr(entity, os);
    OutStream_putc(os, '\n');

    return NULL;
}

void Scope_print(Scope *this, OutStream os) {
    OutStream_puts(os, "{\n");
    Map_foreach_kv(&this->entities, &Scope_print_entity, (void*)&os);
    OutStream_puts(os, "}\n");
}

