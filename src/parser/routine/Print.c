ParserResult Parser_parsePrintExpression(
    Parser *this,
    ParserInStream *stream, 
    Scope *scope,
    Expression *out_expression
) {
    Expression expression;

    ParserResult result = Parser_parseExpression(this, stream, scope, PrimitiveType_upcast(TYPE_ANY), &expression);
    if (!ParserResult_isSuccess(result)) {
        return result;
    }

    *out_expression = PrintExpression_create_move(expression, this->allocator);

    return ParserResult_Success;
}