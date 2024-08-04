typedef enum {
    KEYWORD_ASSIGN = 0,
    KEYWORD_PRINT,
    KEYWORD__END
} EKeyword;

const EKeyword Keyword_KEYWORDS[KEYWORD__END] = {
    [KEYWORD_ASSIGN] = KEYWORD_ASSIGN,
    [KEYWORD_PRINT] = KEYWORD_PRINT
};

const char *Keyword_REPRS[KEYWORD__END] = {
    [KEYWORD_ASSIGN] = "<keyword:assign>",
    [KEYWORD_PRINT] = "<keyword:print>"
};

extern const IObject IKeywordObject;

Object KeywordObject_create_(EKeyword keyword) {
    return (Object) {
        .interface = &IKeywordObject,
        .target = (void*)&Keyword_KEYWORDS[keyword]
    };
}

#define this ((EKeyword*)vthis)

void KeywordObject_destroy(void *vthis, Allocator allocator) {}

void KeywordObject_repr(void *vthis, OutStream stream) {
    OutStream_puts(stream, Keyword_REPRS[*this]);
}

Object KeywordObject_copy(void *vthis, Allocator allocator) {
    return KeywordObject_create_(*this);
}

#undef this

const IObject IKeywordObject = {
    .repr = &KeywordObject_repr,
    .destroy = &KeywordObject_destroy,
    .copy = &KeywordObject_copy
};

TypedObject KeywordObject_create(EKeyword keyword) {
    return (TypedObject) {
        .type = PrimitiveType_upcast(TYPE_KEYWORD),
        .object = KeywordObject_create_(keyword)
    };
}