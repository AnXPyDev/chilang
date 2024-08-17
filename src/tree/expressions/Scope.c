typedef struct {
    Scope scope;
    Expression expression; 
} ScopeExpression;

Expression ScopeExpression_upcast(ScopeExpression *expression);

ScopeExpression *ScopeExpression_create_move(Expression expression, Allocator alc) { 
    ScopeExpression *this = Allocator_malloc(sizeof(ScopeExpression));
    Scope_create(&this->scope);
}