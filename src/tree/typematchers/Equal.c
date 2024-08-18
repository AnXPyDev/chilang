#define this ((Type*)vthis)

bool EqualTypeMatcher_match(void *vthis, Type type) {
    return Type_equal(*this, type); 
}

#undef this

const ITypeMatcher IEqualTypeMatcher = {
    .match = &EqualTypeMatcher_match
};

TypeMatcher EqualTypeMatcher_upcast(Type *type) {
    return (TypeMatcher) {
        .interface = &IEqualTypeMatcher,
        .object = type
    };
}