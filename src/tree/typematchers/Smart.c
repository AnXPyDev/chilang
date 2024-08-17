bool SmartTypeMatcher_match(void *this, Type t1, Type t2) {
    if (Type_isPrimitiveS(t1, TYPE_ANY)) {
        return true;
    }
    return Type_equal(t1, t2); 
}

const ITypeMatcher ISmartTypeMatcher = {
    .match = &SmartTypeMatcher_match
};

const TypeMatcher SmartTypeMatcher = {
    .interface = &ISmartTypeMatcher,
    .object = NULL
};