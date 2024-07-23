#define this ((Type*)vthis)

void *TypeEntityObject_copy(void *vthis, Allocator allocator) {
    Type *type = Allocator_malloc(allocator, sizeof(Type));
    *type = Type_copy(*this, allocator);
    return (void*)type;
}

void TypeEntityObject_destroy(void *vthis, Allocator allocator) {
    Type_destroy(*this, allocator);
    Allocator_free(allocator, this);
}

void TypeEntityObject_repr(void *vthis, OutStream os) {
    Type_repr(*this, os);
}

#undef this

const IEntityObject ITypeEntityObject = {
    .copy = &TypeEntityObject_copy,
    .destroy = &TypeEntityObject_destroy,
    .repr = &TypeEntityObject_repr
};

void TypeEntity_create(Entity *entity, Type type, Allocator allocator) {
    entity->type = PrimitiveType_Type(TYPE_TYPE);
    entity->object_interface = &ITypeEntityObject;
    Type *t = Allocator_malloc(allocator, sizeof(Type));
    *t = Type_copy(type, allocator);
    entity->object = (void*)t;
}