typedef struct Scope {
    Allocator allocator;
    struct Scope *global;
    struct Scope *parent;
    MemberList members;
} Scope;

void Scope_create(Scope *this, Allocator allocator, Scope *global, Scope *parent) {
    this->global = global;
    this->parent = parent;
    this->allocator = allocator;
    MemberList_create(&this->members, allocator);
}

Member *Scope_get_member(Scope *this, StringView token) {
    Member *member = MemberList_get(&this->members, token);
    
    if (member == NULL && this->parent != NULL) {
        member = Scope_get_member(this->parent, token);
    }

    if (member == NULL && this->global != NULL) {
        member = Scope_get_member(this->global, token);
    }
    
    return member;
}

Member *Scope_get_local_member(Scope *this, StringView token) {
    return MemberList_get(&this->members, token);
}

Member *Scope_add_member(Scope *this, StringView token) {
    return MemberList_add(&this->members, token);
}

void Scope_destroy(Scope *this) {
    MemberList_destroy(&this->members, this->allocator);
}

void Scope_repr(Scope *this, OutStream os) {
    StringView delim = strview("\n  ");
    DelimOutStream dos = DelimOutStream_new(os, delim);
    OutStream_puts(os, "Scope {");
    OutStream_write(os, delim);
    MemberList_repr(&this->members, DelimOutStream_upcast(&dos));
    OutStream_puts(os, "\n}");
}