ParserResult Parser_parseAssignmentExpression(
    Parser *this,
    ParserInStream *stream, 
    Scope *scope
    StringView token,
    Expression *out_expression
) {
    Expression expression;
    ParserResult result = Parser_parseExpression(this, stream, scope, &expression);
    if (!ParserResult_isSuccess(result)) {
        return result;
    }

    
}