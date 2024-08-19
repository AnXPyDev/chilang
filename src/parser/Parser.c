typedef struct {
    Allocator allocator;
    OutStream logStream;
    MemberList *globalFrame;
} Parser;

void Parser_create(Parser *this) {
    MemberList_add_keyword(this->globalFrame, strview("="), KEYWORD_ASSIGN, this->allocator);
    MemberList_add_keyword(this->globalFrame, strview("print"), KEYWORD_PRINT, this->allocator);
}
void Parser_destroy(Parser *this) {}
