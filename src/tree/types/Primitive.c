typedef enum {
    // META

    TYPE_UNIT,
    TYPE_NAMESPACE,
    TYPE_ALIAS,
    TYPE_FUNCTION,
    TYPE_INTERFACE,
    TYPE_TEMPLATE,
    TYPE_TYPE,
    TYPE_STRUCT,
    TYPE_UNION,

    // SIMPLE

    TYPE_U8,
    TYPE_U32,
    TYPE_I8,
    TYPE_I32,
    TYPE_BOOL,
    TYPE_PTR,
    TYPE_FPTR,

    TYPE__END
} EPrimitiveType;

const char *PrimitiveType_REPRS[TYPE__END] = {
    [TYPE_UNIT] = "<unit>",
    [TYPE_NAMESPACE] = "<namespace>",
    [TYPE_ALIAS] = "<alias>",
    [TYPE_FUNCTION] = "<function>",
    [TYPE_INTERFACE] = "<interface>",
    [TYPE_TEMPLATE] = "<template>",
    [TYPE_TYPE] = "<type>",
    [TYPE_STRUCT] = "<struct>",
    [TYPE_UNION] = "<union>",
    [TYPE_U8] = "u8",
    [TYPE_U32] = "u32",
    [TYPE_I8] = "i8",
    [TYPE_I32] = "i32",
    [TYPE_BOOL] = "bool",
    [TYPE_PTR] = "ptr",
    [TYPE_FPTR] = "fptr",
};

const TypeInfo PrimitiveType_INFOS[TYPE__END] = {
    [TYPE_UNIT] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_NAMESPACE] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_ALIAS] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_FUNCTION] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_INTERFACE] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_TEMPLATE] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_TYPE] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_STRUCT] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    [TYPE_UNION] = { .valid = true, .meta = true, .size_known = false, .size = 0 },
    
    [TYPE_I8] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(int8_t) },
    [TYPE_I32] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(int32_t) },
    [TYPE_U8] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(uint8_t) },
    [TYPE_U32] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(uint32_t) },
    [TYPE_BOOL] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(bool) },

    [TYPE_PTR] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(void*) },
    [TYPE_FPTR] = { .valid = true, .meta = false, .size_known = true, .size = sizeof(void(*)()) }
};

typedef struct {
    EPrimitiveType type;
} PrimitiveType;

PrimitiveType PrimitiveType_TYPES[TYPE__END] = {
    [TYPE_UNIT] = { .type = TYPE_UNIT },
    [TYPE_NAMESPACE] = { .type = TYPE_NAMESPACE },
    [TYPE_ALIAS] = { .type = TYPE_ALIAS },
    [TYPE_FUNCTION] = { .type = TYPE_FUNCTION },
    [TYPE_TEMPLATE] = { .type = TYPE_FUNCTION },
    [TYPE_FUNCTION] = { .type = TYPE_FUNCTION },
    [TYPE_TYPE] = { .type = TYPE_TYPE },
    [TYPE_STRUCT] = { .type = TYPE_STRUCT },
    [TYPE_UNION] = { .type = TYPE_UNION },
    
    [TYPE_I8] = { .type = TYPE_I8 },
    [TYPE_I32] = { .type = TYPE_I32 },
    [TYPE_U8] = { .type = TYPE_U8 },
    [TYPE_U32] = { .type = TYPE_U32 },

    [TYPE_BOOL] = { .type = TYPE_BOOL },

    [TYPE_PTR] = { .type = TYPE_PTR },
    [TYPE_FPTR] = { .type = TYPE_FPTR }
};

#define this ((PrimitiveType*)vthis)

void PrimitiveType_repr(void *vthis, OutStream os) {
    OutStream_puts(os, PrimitiveType_REPRS[this->type]);
}

TypeInfo PrimitiveType_info(void *vthis) {
    return PrimitiveType_INFOS[this->type];
}

void PrimitiveType_destroy(void *vthis, Allocator allocator) {}

#undef this

const IType IPrimitiveType = {
    .repr = &PrimitiveType_repr,
    .info = &PrimitiveType_info,
    .destroy = &PrimitiveType_destroy,
};

bool PrimitiveType_is(Type this, EPrimitiveType type) {
    if (this.interface != &IPrimitiveType) {
        return false;
    }

    return ((PrimitiveType*)this.object)->type == type;
}

Type PrimitiveType_Type(EPrimitiveType type) {
    return (Type) {
        .interface = &IPrimitiveType,
        .object = &PrimitiveType_TYPES[type]
    };
}