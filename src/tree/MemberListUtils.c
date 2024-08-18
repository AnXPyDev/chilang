void MemberList_add_type(MemberList *this, StringView token, Type type, Allocator alc) {
    Member *memb = MemberList_add(this, token);
    TypedObject to = TypeObject_create(type, alc);
    memb->type = to.type;
    memb->object = to.object;
    memb->qualifiers = MemberQualifiers_NULL;
    memb->qualifiers._const = true;
}

void MemberList_add_keyword(MemberList *this, StringView token, EKeyword keyword) {
    Member *memb = MemberList_add(this, token);
    TypedObject to = KeywordObject_create(keyword);
    memb->type = to.type;
    memb->object = to.object;
    memb->qualifiers = MemberQualifiers_NULL;
    memb->qualifiers._const = true;
}