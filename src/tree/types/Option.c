typedef struct {
    Type type; 
} OptionType;

Type OptionType_wrap_copy(Type type, Allocator allocator);

#define this ((OptionType*)vthis)

void OptionType_repr(void *vthis, OutStream os) {
    Type_repr(this->type, os);
    OutStream_putc(os, '?');
}

void OptionType_destroy(void *vthis, Allocator allocator) {
    Type_destroy(this->type, allocator);
    Allocator_free(allocator, this);
}

Type OptionType_copy(void *vthis, Allocator allocator) {
   return OptionType_wrap_copy(this->type, allocator);
}

TypeInfo OptionType_info(void *vthis) {
    TypeInfo info = {
        .valid = true, .meta = false, .size_known = true, .size = sizeof(void*)
    };

    TypeInfo inner_info = Type_info(this->type);

    info.valid = inner_info.valid && !inner_info.meta;
    info.meta = inner_info.meta;
    
    return info;    
}

#define other ((OptionType*)vother)
bool OptionType_equal(void *vthis, void *vother) {
    return Type_equal(this->type, other->type);
}
#undef other

#undef this

const IType IOptionType = {
    .destroy = &OptionType_destroy,
    .repr = &OptionType_repr,
    .copy = &OptionType_copy,
    .info = &OptionType_info,
    .equal = &OptionType_equal
};

Type OptionType_wrap(Type type, Allocator allocator) {
    OptionType *this = Allocator_malloc(allocator, sizeof(OptionType));
    this->type = type;
    return (Type) {
        .interface = &IOptionType,
        .object = (void*)this
    };
}

Type OptionType_wrap_copy(Type type, Allocator allocator) {
    Type copy = Type_copy(type, allocator);
    return OptionType_wrap(copy, allocator);
}

bool Type_isOption(Type type) {
    return type.interface == &IOptionType;
}