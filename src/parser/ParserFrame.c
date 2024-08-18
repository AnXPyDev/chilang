typedef struct ParserFrame {
    struct ParserFrame *parent;
    MemberList *frame;
} ParserFrame;

Member *ParserFrame_getLocalMember(ParserFrame *this, StringView token, MemberMatcher mm, TypeMatcher tm) {
    return MemberList_getMatching(this->frame, token, mm, tm);
}

Member *ParserFrame_getMember(ParserFrame *this, StringView token, MemberMatcher mm, TypeMatcher tm) {
    Member *member = ParserFrame_getLocalMember(this, token, mm, tm);
    if (member == NULL && this->parent != NULL) {
        return ParserFrame_getMember(this->parent, token, mm, tm);
    }
    return member;
}

Member *ParserFrame_addMember(ParserFrame *this, StringView token) {
    return MemberList_add(this->frame, token);
}