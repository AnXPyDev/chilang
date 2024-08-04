typedef struct {
    Allocator allocator;
    OutStream logStream;
    Scope *globalScope;
} Parser;

void Parser_create(Parser *this) {}