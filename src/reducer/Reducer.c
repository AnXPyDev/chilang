typedef struct {
    Allocator allocator; 
} Reducer;

void Reducer_create(Reducer *this, Allocator allocator) {
    this->allocator = allocator;
}

Expression Reducer_reduceExpression(Reducer *this, Expresison expression) {
    
}