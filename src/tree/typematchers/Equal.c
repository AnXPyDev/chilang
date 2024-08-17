bool EqualTypeMatcher_match(void *this, Type t1, Type t2) {
    return Type_equal(t1, t2); 
}

const ITypeMatcher IEqualTypeMatcher = {
    .match = &EqualTypeMatcher_match
};

const TypeMatcher EqualTypeMatcher = {
    .interface = &IEqualTypeMatcher,
    .object = NULL
};