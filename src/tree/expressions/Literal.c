typedef struct {
    Type type;
} LiteralExpression;

#define this ((LiteralExpression*)vthis)

Type LiteralExpression_type(void *vthis, Allocator allocator) {
    return Type_copy(this->type, allocator);
}

void LiteralExpression_destroy(void *vthis, Allocator allocator) {
    Type_destroy(this->type, allocator);
    Allocator_free(allocator, this);
}

void LiteralExpression
