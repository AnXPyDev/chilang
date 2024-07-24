#define this ((Modifier*)vthis)

void *ModifierObject_copy(void *vthis, Allocator allocator) {
    Modifier *modifier = Allocator_malloc(allocator, sizeof(Modifier));
    *modifier = Modifier_copy(*this, allocator);
    return (void*)modifier;
}

void ModifierObject_destroy(void *vthis, Allocator allocator) {
    Modifier_destroy(*this, allocator);
    Allocator_free(allocator, this);
}

void ModifierObject_repr(void *vthis, OutStream os) {
    Modifier_repr(*this, os);
}

#undef this

const IObject IModifierObject = {
    .destroy = &ModifierObject_destroy,
    .copy = &ModifierObject_copy,
    .repr = &ModifierObject_repr
};