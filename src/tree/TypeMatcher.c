typedef struct {
    bool (*match)(void *this, Type t1, Type t2);
} ITypeMatcher;

typedef struct {
    const ITypeMatcher *interface;
    void *object;
} TypeMatcher;

bool TypeMatcher_match(TypeMatcher this, Type t1, Type t2) {
    return this.interface->match(this.object, t1, t2);
}