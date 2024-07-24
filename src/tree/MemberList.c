typedef struct {
    bool _static : 1;
    bool _private : 1;
    bool _public : 1;
    bool _const : 1;
} MemberQualifiers;

const MemberQualifiers MemberQualifiers_NULL = {
    ._static = false,
    ._private = false,
    ._public = false,
    ._const = false
};

void MemberQualifiers_repr(MemberQualifiers qualifiers, OutStream os) {
    if (qualifiers._static) {
        OutStream_puts(os, "static ");
    }
    if (qualifiers._private) {
        OutStream_puts(os, "private ");
    }
    if (qualifiers._public) {
        OutStream_puts(os, "public ");
    }
    if (qualifiers._const) {
        OutStream_puts(os, "const ");
    }
}

typedef struct {
    MemberQualifiers qualifiers;
    Type type;
    Object object;
} Member;

void Member_copy(Member *this, Member *dst, Allocator allocator) {
    dst->qualifiers = this->qualifiers;
    dst->type = Type_copy(this->type, allocator);
    dst->object = Object_copy_maybe(this->object, allocator);
}

void Member_destroy(Member *this, Allocator allocator) {
    Type_destroy(this->type, allocator);
    Object_destroy_maybe(this->object, allocator);
}

typedef struct {
    Map members;
} MemberList;

void MemberList_create(MemberList *this, Allocator allocator) {
    Map_create(&this->members, allocator, sizeof(Member));
}

void *MemberList_destroy_member(void *item, void *payload) {
    Member_destroy((Member*)item, *(Allocator*)payload);
    return NULL;
}

void MemberList_destroy(MemberList *this, Allocator allocator) {
    Map_foreach(&this->members, &MemberList_destroy_member, (void*)&allocator);
    Map_destroy(&this->members);
}

Member *MemberList_get(MemberList *this, StringView token) {
    return Map_get(&this->members, token);
}

Member *MemberList_add(MemberList *this, StringView token) {
    return Map_add(&this->members, token);
}

void *MemberList_member_repr(StringView token, void *item, void *payload) {
    OutStream os = *(OutStream*)payload;
    Member *member = (Member*)item;

    OutStream_begin_item(os);

    MemberQualifiers_repr(member->qualifiers, os);
    
    Type_repr(member->type, os);
    OutStream_putc(os, ' ');
    OutStream_write(os, token);

    if (!Object_isNull(member->object)) {
        OutStream_puts(os, " = ");
        Object_repr(member->object, os);
    }

    OutStream_end_item(os);

    return NULL;
}

void MemberList_repr(MemberList *this, OutStream os) {
    Map_foreach_stable_kv(&this->members, &MemberList_member_repr, (void*)&os);
}

#undef PRINT_PAYLOAD

