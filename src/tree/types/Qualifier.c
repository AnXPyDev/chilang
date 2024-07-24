const uint32_t TYPEQUALIFIER_CONST = 1 << 0;

typedef struct {
    bool _const : 1;
} TypeQualifiers;

const TypeQualifiers TypeQualifiers_NULL = {
    ._const = false
};

typedef struct {
    TypeQualifiers qualifiers;
    Type type;
} QualifierType;

TypeQualifiers TypeQualifiers_merge(TypeQualifiers q1, TypeQualifiers q2) {
    return (TypeQualifiers) {
        ._const = q1._const || q2._const
    };
}

TypeQualifiers TypeQualifiers_make(uint32_t flags) {
    return (TypeQualifiers) {
        ._const = flags & TYPEQUALIFIER_CONST
    };
}

Type QualifierType_wrap_copy(TypeQualifiers qualifiers, Type type, Allocator allocator);

#define this ((QualifierType*)vthis)

void QualifierType_repr(void *vthis, OutStream os) {
    if (this->qualifiers._const) {
        OutStream_puts(os, "const ");
    }

    Type_repr(this->type, os);
}

TypeInfo QualifierType_info(void *vthis) {
    return Type_info(this->type);
}

void QualifierType_destroy(void *vthis, Allocator allocator) {
    Type_destroy(this->type, allocator);
    Allocator_free(allocator, this);
}

Type QualifierType_copy(void *vthis, Allocator allocator) {
    return QualifierType_wrap_copy(this->qualifiers, this->type, allocator); 
}

#undef this

const IType IQualifierType = {
    .repr = &QualifierType_repr,
    .destroy = &QualifierType_destroy,
    .info = &QualifierType_info,
    .copy = &QualifierType_copy
};

bool Type_isQualifier(Type type) {
    return type.interface == &IQualifierType;
}

Type QualifierType_wrap(TypeQualifiers qualifiers, Type type, Allocator allocator) {
    if (Type_isQualifier(type)) {
        QualifierType *other = (QualifierType*)type.object;
        other->qualifiers = TypeQualifiers_merge(qualifiers, other->qualifiers);
        return type;
    }

    QualifierType *this = Allocator_malloc(allocator, sizeof(QualifierType));

    this->qualifiers = qualifiers;
    this->type = type;

    return (Type) {
        .interface = &IQualifierType,
        .object = this
    };
}

Type QualifierType_wrap_copy(TypeQualifiers qualifiers, Type type, Allocator allocator) {
    QualifierType *this = Allocator_malloc(allocator, sizeof(QualifierType));

    if (Type_isQualifier(type)) {
        QualifierType *other = (QualifierType*)type.object;
        this->qualifiers = TypeQualifiers_merge(qualifiers, other->qualifiers);
        this->type = Type_copy(other->type, allocator);
    } else {
        this->qualifiers = qualifiers;
        this->type = Type_copy(type, allocator);
    }

    return (Type) {
        .interface = &IQualifierType,
        .object = this
    };
}