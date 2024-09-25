typedef struct {
    Allocator allocator;
    OutStream logStream;
    MemberList *globalFrame;

    Expression literal_true;
    Expression literal_false;
} Parser;

#define ADD_KEYWORD(token, kw) MemberList_add_keyword(this->globalFrame, strview(token), kw, this->allocator)


void Parser_create(Parser *this) {
    ADD_KEYWORD("=", KEYWORD_ASSIGN);
    ADD_KEYWORD("print", KEYWORD_PRINT);
    ADD_KEYWORD("if", KEYWORD_IF);
    ADD_KEYWORD("else", KEYWORD_ELSE);
    ADD_KEYWORD("true", KEYWORD_TRUE);
    ADD_KEYWORD("false", KEYWORD_FALSE);
    ADD_KEYWORD("fn", KEYWORD_CALLABLE);

    bool valTrue = true;
    bool valFalse = false;

    this->literal_true = LiteralExpression_create(PrimitiveType_upcast(TYPE_BOOL), (char*)&valTrue, this->allocator);
    this->literal_false = LiteralExpression_create(PrimitiveType_upcast(TYPE_BOOL), (char*)&valFalse, this->allocator);
}

#undef ADD_KEYWORD

void Parser_destroy(Parser *this) {}
