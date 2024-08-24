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
    StringBuffer token;
} Member;

void Member_destroy(Member *this, Allocator allocator) {
    Type_destroy(this->type, allocator);
    Object_destroy_maybe(this->object, allocator);
    Buffer_free(this->token, allocator);
}

typedef struct {
    Vector members;
    Map member_map;
} MemberList;

void MemberList_create(MemberList *this, Allocator allocator) {
    Vector_create(&this->members, allocator, sizeof(Member));
    Vector_init(&this->members, 8);
    Map_create(&this->member_map, allocator, sizeof(Member*));
}

void *MemberList_destroy_member(void *item, void *payload) {
    Member_destroy((Member*)item, *(Allocator*)payload);
    return NULL;
}

void MemberList_destroy(MemberList *this, Allocator allocator) {
    Member *end = Vector_end(&this->members);
    for (Member *m = Vector_begin(&this->members); m < end; m++) {
        Member_destroy(m, allocator);
    }
    Vector_destroy(&this->members);
    Map_destroy(&this->member_map);
}

Member *MemberList_getFirst(MemberList *this, StringView token) {
    return *(Member**)Map_get(&this->member_map, token);
}

#include "MemberMatcher.c"

struct MemberList_args_getMatching_loop {
    MemberMatcher mm;
    TypeMatcher tm;
};

void *MemberList_callback_getMatching_loop(void *item, void *payload) {
    Member *member = *(Member**)item;
    struct MemberList_args_getMatching_loop *args = payload;

    if (
        MemberMatcher_match(args->mm, member) &&
        TypeMatcher_match(args->tm, member->type)
    ) {
        return member;
    }

    return NULL;
}

Member *MemberList_getMatching(MemberList *this, StringView token, MemberMatcher mm, TypeMatcher tm) {
    struct MemberList_args_getMatching_loop args = {
        .mm = mm,
        .tm = tm,
    };
    return Map_foreach_matching(&this->member_map, token,
        &MemberList_callback_getMatching_loop,
        &args
    );
}

Member *MemberList_add(MemberList *this, StringView token, Allocator alc) {
    Member *member = Vector_push(&this->members);
    *(Member**)Map_add(&this->member_map, token) = member;
    member->token = Buffer_copy(token, alc);
    return member;
}

void *MemberList_member_repr(StringView token, void *item, void *payload) {
    OutStream os = *(OutStream*)payload;
    Member *member = *(Member**)item;

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
    Map_foreach_stable_kv(&this->member_map, &MemberList_member_repr, &os);
}

#undef PRINT_PAYLOAD

Size MemberList_getRuntimeSize(MemberList *this) {
    return 0;
}

bool MemberList_hasMember(MemberList *this, Member *member) {
    return member >= (Member*)Vector_begin(&this->members) && member < (Member*)Vector_end(&this->members);
}