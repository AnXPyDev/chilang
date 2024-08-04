void Scope_add_type(Scope *scope, StringView token, Type type) {
    Member *memb = Scope_add_member(scope, token);
    TypedObject to = TypeObject_create(type, scope->allocator);
    memb->type = to.type;
    memb->object = to.object;
    memb->qualifiers = MemberQualifiers_NULL;
    memb->qualifiers._const = true;
}

void Scope_add_keyword(Scope *scope, StringView token, EKeyword keyword) {
    Member *memb = Scope_add_member(scope, token);
    TypedObject to = KeywordObject_create(keyword);
    memb->type = to.type;
    memb->object = to.object;
    memb->qualifiers = MemberQualifiers_NULL;
    memb->qualifiers._const = true;
}