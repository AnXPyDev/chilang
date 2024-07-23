typedef struct {
    void (*destroy)(void *this, Allocator allocator);
    void *(*copy)(void *this, Allocator allocator);
    void (*repr)(void *this, OutStream os);
} IEntityObject;

typedef struct {
    Type type;
    bool mutable;
    const IEntityObject *object_interface;
    void *object;
} Entity;

const Entity Entity_NULL = {
    .type = Type_NULL,
    .mutable = false,
    .object_interface = NULL,
    .object = NULL
};

void Entity_destroy(Entity *this, Allocator allocator) {
    Type_destroy(this->type, allocator);
    if (this->object_interface == NULL) {
        return;
    }
    this->object_interface->destroy(this->object, allocator);
}

void Entity_copy(Entity *this, Entity *dst, Allocator allocator) {
}

Size Entity_size(Entity *this) {
    TypeInfo ti = Type_info(this->type);
    return ti.size;
}

void Entity_repr(Entity *this, OutStream os) {
    this->object_interface->repr(this->object, os);
}