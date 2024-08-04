ParserResult Parser_parseScope(Parser *this, ParserInStream *stream, Scope *scope, Expression *expression) {
    ParserResult result = ParserResult_Success;
    Vector expressions = Vector_new(this->allocator, sizeof(Expression));
    Vector_init(&expressions, 8);

    while (1) {
        ParserChar c = ParserInStream_getc(stream);
        if (c == PARSER_EOF) {
            break;
        }

        if (!ParserChar_isWhitespace(c)) {
            goto parse_expression;
        }

        continue;
        parse_expression: {
            ParserInStream_ungetc(stream, c);

            Expression expression = Expression_NULL;

            result = Parser_parseExpression(this, stream, scope, &expression);

            if (!ParserResult_isSuccess(result)) {
                goto error;
            }

            if (!Expression_isNull(expression)) {
                *(Expression*)Vector_push(&expressions) = expression;
            }
        };
    }

    SequenceExpression *seq = (SequenceExpression*)Allocator_malloc(this->allocator, sizeof(SequenceExpression));
    *seq = (SequenceExpression) {
        .size = expressions.size,
        .items = (Expression*)expressions.data
    };

    *expression = SequenceExpression_upcast(seq);

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

