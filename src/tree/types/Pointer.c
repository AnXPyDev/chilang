typedef struct {
    Type type; 
} PointerType;

Type PointerType_wrap_copy(Type type, Allocator allocator);

#define this ((PointerType*)vthis)

void PointerType_repr(void *vthis, OutStream os) {
    OutStream_putc(os, '@');
    Type_repr(this->type, os);
}

void PointerType_destroy(void *vthis, Allocator allocator) {
    Type_destroy(this->type, allocator);
    Allocator_free(allocator, this);
}

Type PointerType_copy(void *vthis, Allocator allocator) {
   return PointerType_wrap_copy(this->type, allocator);
}

TypeInfo PointerType_info(void *vthis) {
    TypeInfo info = {
        .valid = true, .meta = false, .size_known = true, .size = sizeof(void*)
    };

    TypeInfo inner_info = Type_info(this->type);

    info.valid = inner_info.valid && !inner_info.meta;
    info.meta = inner_info.meta;
    
    return info;    
}

#define other ((PointerType*)vother)
bool PointerType_equal(void *vthis, void *vother) {
    return Type_equal(this->type, other->type);
}
#undef other

#undef this

const IType IPointerType = {
    .destroy = &PointerType_destroy,
    .repr = &PointerType_repr,
    .copy = &PointerType_copy,
    .info = &PointerType_info,
    .equal = &PointerType_equal
};

Type PointerType_wrap(Type type, Allocator allocator) {
    PointerType *this = Allocator_malloc(allocator, sizeof(PointerType));
    this->type = type;
    return (Type) {
        .interface = &IPointerType,
        .object = (void*)this
    };
}

Type PointerType_wrap_copy(Type type, Allocator allocator) {
    Type copy = Type_copy(type, allocator);
    return PointerType_wrap(copy, allocator);
}

bool Type_isPointer(Type type) {
    return type.interface == &IPointerType;
}