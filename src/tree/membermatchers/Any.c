bool AnyMemberMatcher_match(void *vthis, Member *member) {
    return true;
}

const IMemberMatcher IAnyMemberMatcher = {
    .match = &AnyMemberMatcher_match
};

const MemberMatcher AnyMemberMatcher = {
    .interface = &IAnyMemberMatcher,
    .object = NULL
};