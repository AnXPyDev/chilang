typedef struct {
    Allocator allocator;
    OutStream logStream;
    MemberList *globalFrame;
} Parser;

void Parser_create(Parser *this) {
    MemberList_add_keyword(this->globalFrame, strview("="), KEYWORD_ASSIGN);
    MemberList_add_keyword(this->globalFrame, strview("print"), KEYWORD_PRINT);
}
void Parser_destroy(Parser *this) {}
