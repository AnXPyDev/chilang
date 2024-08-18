typedef struct {
    bool (*match)(void *this, Type type);
} ITypeMatcher;

typedef struct {
    const ITypeMatcher *interface;
    void *object;
} TypeMatcher;

bool TypeMatcher_match(TypeMatcher this, Type type) {
    return this.interface->match(this.object, type);
}