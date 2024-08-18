#define this ((Type*)vthis)

bool SmartTypeMatcher_match(void *vthis, Type type) {
    if (Type_isPrimitiveS(*this, TYPE_ANY)) {
        return true;
    }
    return Type_equal(*this, type); 
}

#undef this

const ITypeMatcher ISmartTypeMatcher = {
    .match = &SmartTypeMatcher_match
};

TypeMatcher SmartTypeMatcher_upcast(Type *type) {
    return (TypeMatcher) {
        .interface = &ISmartTypeMatcher,
        .object = type
    };
}