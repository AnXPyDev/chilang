ParserResult Parser_parseScope(Parser *this, ParserInStream *stream, Scope *scope, Expression *expression) {
    ParserResult result = ParserResult_Success;
    Vector expressions = Vector_new(this->allocator, sizeof(Expression));
    Vector_init(&expressions, 8);

    while (1) {
        ParserChar c = ParserInStream_getc(stream);
        if (c == PARSER_EOF || ParserChar_isBlockEnd(c)) {
            break;
        }

        if (!ParserChar_isWhitespace(c)) {
            goto parse_expression;
        }

        continue;
        parse_expression: {
            ParserInStream_ungetc(stream, c);

            Expression expression = Expression_NULL;

            result = Parser_parseExpression(this, stream, scope, Type_NULL, &expression);

            if (!ParserResult_isSuccess(result)) {
                goto error;
            }

            if (!Expression_isNull(expression)) {
                *(Expression*)Vector_push(&expressions) = expression;
            }
        };
    }

    switch (expressions.size) {
        default:;
            SequenceExpression *seq = Allocator_new(this->allocator, SequenceExpression);
            *seq = (SequenceExpression) {
                .size = expressions.size,
                .items = (Expression*)expressions.data
            };
            *expression = SequenceExpression_upcast(seq);
            break;
        case 1:;
            *expression = *(Expression*)Vector_get(&expressions, 0);
        case 0:;
            Vector_destroy(&expressions);
    }

    return result;

    error: {
        Expression *end = (Expression*)Vector_end(&expressions);
        for (Expression *exp = (Expression*)Vector_begin(&expressions); exp < end; exp++) {
            Expression_destroy(*exp, this->allocator);
        }

        Vector_destroy(&expressions);
        return result;
    };
}

