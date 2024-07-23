typedef struct {
    ModifierApplyResult (*apply)(void *this, Entity *entity, Allocator allocator);
    void (*destroy)(void *this, Allocator allocator);
    Modifier (*copy)(void *this, Allocator allocator);
    void (*repr)(void *this, OutStream os);
} IModifier;

typedef struct {
    const IModifier *interface;
    void *object;
} Modifier;

typedef struct {
    int code;
    const char *message;
} ModifierApplyResult;

const ModifierApplyResult ModifierApplyResult_Success = {
    .code = 0, .message = NULL
};


ModifierApplyResult Modifier_apply(Modifier this, Entity *entity, Allocator allocator) {
    return this.interface->apply(this.object, entity, allocator);
}

void Modifier_destroy(Modifier this, Allocator allocator) {
    this.interface->destroy(this.object, allocator);
}

Modifier Modifier_copy(Modifier this, Allocator allocator) {
    return this.interface->copy(this.object, allocator);
}

void Modifier_repr(Modifier this, OutStream os) {
    this.interface->repr(this.object, os);
}
