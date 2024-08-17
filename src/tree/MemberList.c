typedef struct {
    bool _static : 1;
    bool _private : 1;
    bool _public : 1;
    bool _const : 1;
    bool _reserve : 1;
} MemberQualifiers;

const MemberQualifiers MemberQualifiers_NULL = {
    ._static = false,
    ._private = false,
    ._public = false,
    ._const = false,
    ._reserve = false
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
    if (qualifiers._reserve) {
        OutStream_puts(os, "reserve ");
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

Member *MemberList_getFirst(MemberList *this, StringView token) {
    return Map_get(&this->members, token);
}

struct MemberList_args_getMatching_loop {
    TypeMatcher matcher;
    Type type;
};

void *MemberList_callback_getMatching_loop(void *item, void *payload) {
    Member *member = item;
    struct MemberList_args_getMatching_loop *args = payload;

    if (TypeMatcher_match(args->matcher, member->type, args->type)) {
        return item;
    }

    return NULL;
}

Member *MemberList_getMatching(MemberList *this, StringView token, TypeMatcher matcher, Type type) {
    struct MemberList_args_getMatching_loop args = {
        .matcher = matcher,
        .type = type
    };
    return Map_foreach_matching(&this->members, token,
        &MemberList_callback_getMatching_loop,
        &args
    );
}

Member *MemberList_add(MemberList *this, StringView token) {
    return Map_add(&this->members, token);
}

void *MemberList_member_repr(StringView token, void *item, void *payload) {
    OutStream os = *(OutStream*)payload;
    Member *member = (Member*)item;

    OutStream_beginItem(os);

    MemberQualifiers_repr(member->qualifiers, os);
    
    Type_repr(member->type, os);
    OutStream_putc(os, ' ');
    OutStream_write(os, token);

    if (!Object_isNull(member->object)) {
        OutStream_puts(os, " = ");
        Object_repr(member->object, os);
    }

    OutStream_endItem(os);

    return NULL;
}

void MemberList_repr(MemberList *this, OutStream os) {
    Map_foreach_stable_kv(&this->members, &MemberList_member_repr, &os);
}

#undef PRINT_PAYLOAD

