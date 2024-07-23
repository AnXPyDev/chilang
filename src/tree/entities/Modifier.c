#define this ((Modifier*)vthis)

void *ModifierEntityObject_copy(void *vthis, Allocator allocator) {
    Modifier *modifier = Allocator_malloc(allocator, sizeof(Modifier));
    *modifier = Modifier_copy(*this, allocator);
    return (void*)modifier;
}

void ModifierEntityObject_destroy(void *vthis, Allocator allocator) {
    Modifier_destroy(*this, allocator);
    Allocator_free(allocator, this);
}

void ModifierEntityObject_repr(void *vthis, OutStream os) {
    Modifier_repr(*this, os);
}

#undef this

const IEntityObject IModifierEntityObject = {
    .destroy = &ModifierEntityObject_destroy,
    .copy = &ModifierEntityObject_copy,
    .repr = &ModifierEntityObject_repr
};