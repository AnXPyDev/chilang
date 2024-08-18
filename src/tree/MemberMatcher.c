typedef struct {
    bool (*match)(void *this, Member *member);
} IMemberMatcher;

typedef struct {
    const IMemberMatcher *interface;
    void *object;
} MemberMatcher;

bool MemberMatcher_match(MemberMatcher this, Member *member) {
    return this.interface->match(this.object, member);
}