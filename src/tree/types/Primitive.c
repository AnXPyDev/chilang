typedef enum {
    // META

    TYPE_UNIT,
    TYPE_NAMESPACE,
    TYPE_ALIAS,
    TYPE_INTERFACE,
    TYPE_TEMPLATE,
    TYPE_TYPE,
    TYPE_MODIFIER,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_KEYWORD,
    TYPE_INFER,
    TYPE_DISCARD,
    TYPE_ANY,
    TYPE_TOKEN,

    // SIMPLE

    TYPE_VOID,
    TYPE_BOOL,
    TYPE_U8,
    TYPE_U32,
    TYPE_I8,
    TYPE_I32,

    TYPE__END
} EPrimitiveType;

const char *PrimitiveType_REPRS[TYPE__END] = {
    [TYPE_UNIT] = "<unit>",
    [TYPE_NAMESPACE] = "<namespace>",
    [TYPE_ALIAS] = "<alias>",
    [TYPE_INTERFACE] = "<interface>",
    [TYPE_TEMPLATE] = "<template>",
    [TYPE_TYPE] = "<type>",
    [TYPE_MODIFIER] = "<modifier>",
    [TYPE_STRUCT] = "<struct>",
    [TYPE_UNION] = "<union>",
    [TYPE_KEYWORD] = "<keyword>",
    [TYPE_INFER] = "<infer>",
    [TYPE_DISCARD] = "<discard>",
    [TYPE_ANY] = "<any>",
    [TYPE_TOKEN] = "<token>",

    [TYPE_VOID] = "void",
    [TYPE_BOOL] = "bool",
    [TYPE_U8] = "u8",
    [TYPE_U32] = "u32",
    [TYPE_I8] = "i8",
    [TYPE_I32] = "i32",
};

const TypeInfo PrimitiveType_INFOS[TYPE__END] = {
    [TYPE_UNIT] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_NAMESPACE] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_ALIAS] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_INTERFACE] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_TEMPLATE] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_TYPE] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_MODIFIER] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_STRUCT] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_UNION] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_KEYWORD] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_INFER] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_DISCARD] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_ANY] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_TOKEN] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    
    [TYPE_VOID] = { .valid = true, .meta = false, .size_known = true, .size = 0 },
    [TYPE_BOOL] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(bool) },
    [TYPE_I8] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(int8_t) },
    [TYPE_I32] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(int32_t) },
    [TYPE_U8] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(uint8_t) },
    [TYPE_U32] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(uint32_t) },
};

typedef struct {
    EPrimitiveType type;
} PrimitiveType;

PrimitiveType PrimitiveType_TYPES[TYPE__END] = {
    [TYPE_UNIT] = { .type = TYPE_UNIT },
    [TYPE_NAMESPACE] = { .type = TYPE_NAMESPACE },
    [TYPE_ALIAS] = { .type = TYPE_ALIAS },
    [TYPE_TEMPLATE] = { .type = TYPE_TEMPLATE },
    [TYPE_INTERFACE] = { .type = TYPE_INTERFACE },
    [TYPE_TYPE] = { .type = TYPE_TYPE },
    [TYPE_MODIFIER] = { .type = TYPE_MODIFIER },
    [TYPE_STRUCT] = { .type = TYPE_STRUCT },
    [TYPE_UNION] = { .type = TYPE_UNION },
    [TYPE_KEYWORD] = { .type = TYPE_KEYWORD },
    [TYPE_INFER] = { .type = TYPE_INFER },
    [TYPE_DISCARD] = { .type = TYPE_DISCARD },
    [TYPE_ANY] = { .type = TYPE_ANY },
    [TYPE_TOKEN] = { .type = TYPE_TOKEN },
    
    [TYPE_VOID] = { .type = TYPE_VOID },
    [TYPE_BOOL] = { .type = TYPE_BOOL },
    [TYPE_I8] = { .type = TYPE_I8 },
    [TYPE_I32] = { .type = TYPE_I32 },
    [TYPE_U8] = { .type = TYPE_U8 },
    [TYPE_U32] = { .type = TYPE_U32 },
};

Type PrimitiveType_upcast(EPrimitiveType type);

#define this ((PrimitiveType*)vthis)

void PrimitiveType_repr(void *vthis, OutStream os) {
    OutStream_puts(os, PrimitiveType_REPRS[this->type]);
}

TypeInfo PrimitiveType_info(void *vthis) {
    return PrimitiveType_INFOS[this->type];
}

void PrimitiveType_destroy(void *vthis, Allocator allocator) {}

Type PrimitiveType_copy(void *vthis, Allocator allocator) {
    return PrimitiveType_upcast(this->type);
}

#define other ((PrimitiveType*)vother)

bool PrimitiveType_equal(void *vthis, void *vother) {
    return this->type == other->type;
}

#undef other
#undef this

const IType IPrimitiveType = {
    .repr = &PrimitiveType_repr,
    .info = &PrimitiveType_info,
    .destroy = &PrimitiveType_destroy,
    .copy = &PrimitiveType_copy,
    .equal = &PrimitiveType_equal
};

Type PrimitiveType_upcast(EPrimitiveType type) {
    return (Type) {
        .interface = &IPrimitiveType,
        .object = &PrimitiveType_TYPES[type]
    };
}

bool Type_isPrimitive(Type this) {
    return this.interface == &IPrimitiveType;
}


bool PrimitiveType_is(PrimitiveType *this, EPrimitiveType type) {
    return this->type == type;
}

bool Type_isPrimitiveS(Type this, EPrimitiveType type) {
    return Type_isPrimitive(this) && PrimitiveType_is((PrimitiveType*)this.object, type);
}