ParserResult Parser_parseAssignmentExpression(
    Parser *this,
    ParserInStream *stream, 
    Scope *scope,
    StringView token,
    Expression *out_expression
) {
    Expression expression;

    Member *member = Scope_get_member(scope, token);

    if (member == NULL) {
        return ParserResult_construct_INTERNAL_ERROR(this, stream);
    }

    ParserResult result = Parser_parseExpression(this, stream, scope, member->type, &expression);
    if (!ParserResult_isSuccess(result)) {
        return result;
    }

    *out_expression = AssignmentExpression_create_move(token, expression, this->allocator);

    return ParserResult_Success;
}