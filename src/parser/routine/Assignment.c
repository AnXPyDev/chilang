ParserResult Parser_parseAssignmentExpression(
    Parser *this,
    ParserInStream *stream, 
    ParserFrame *frame,
    StringView token,
    Expression *out_expression
) {
    Expression expression;

    Member *member = ParserFrame_getMember(frame, token, AnyMemberMatcher, SmartTypeMatcher_upcast(&PTYPE_ANY));

    if (member == NULL) {
        return ParserResult_construct_INTERNAL_ERROR(this, stream);
    }

    ParserResult result = Parser_parseExpression(this, stream, frame, member->type, &expression);
    if (!ParserResult_isSuccess(result)) {
        return result;
    }

    *out_expression = AssignmentExpression_create_move(member, expression, this->allocator);

    return ParserResult_Success;
}