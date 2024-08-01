typedef struct {
    Allocator allocator;
    OutStream logStream;
    Scope *globalScope;
} Parser;

void Parser_create(Parser *this) {

}

ParserResult Parser_readToken(Parser *this, ParserInStream *stream, Scope *scope, DataBuffer *buffer) {
    char *bp = buffer->data;
    Size s = 0;

    while (1) {
        ParserChar c = ParserInStream_getc(stream);

        if (c == PARSER_EOF || !ParserUtil_isTokenChar(c)) {
            ParserInStream_ungetc(stream, c);
            break;
        }

        if (s + 1 > PARSER_TOKEN_MAX_LEN) {
            return (ParserResult) {
                .code = 1,
                .message = "Token too large",
                .payload = NULL
            };
        }

        *bp = (char)c;
        bp++;
        s++;
    }

    buffer->size = s;

    return ParserResult_Success;
}

ParserResult Parser_parseExpression(Parser *this, ParserInStream *stream, Scope *scope, Expression *expression) {
    ParserResult result = ParserResult_Success;

    Vector tokens = Vector_new(this->allocator, sizeof(StringView));
    Vector_init(&tokens, 16);

    while (1) {
        ParserChar c = ParserInStream_getc(stream);
        if (c == PARSER_EOF) {
            break;
        }

        if (ParserUtil_isTokenBeginChar(c)) {
            ParserInStream_ungetc(stream, c);
            goto read_token; 
        }

        if (ParserUtil_isExpressionEndChar(c)) {
            break;
        }

        continue;
        read_token: {
            StringBuffer buffer = (StringBuffer) {
                .size = 0,
                .data = (char*)Allocator_malloc(this->allocator, PARSER_TOKEN_MAX_LEN)
            };

            result = Parser_readToken(this, stream, scope, &buffer);

            if (!ParserResult_isSuccess(result)) {
                goto cleanup;
            }

            *(StringBuffer*)Vector_push(&tokens) = buffer;
        };
    }

    
    {
        OutStream_puts(this->logStream, "Found Tokens: ");
        StringBuffer *end = (StringBuffer*)Vector_end(&tokens);
        for (StringBuffer *token = (StringBuffer*)Vector_begin(&tokens); token < end; token++) {
            OutStream_write(this->logStream, Buffer_view(*token));
            OutStream_puts(this->logStream, " ");
        }
    }

    cleanup: {
        StringBuffer *end = (StringBuffer*)Vector_end(&tokens);
        for (StringBuffer *token = (StringBuffer*)Vector_begin(&tokens); token < end; token++) {
            Allocator_free(this->allocator, token->data);
        }
    };

    return result;
}

ParserResult Parser_parseScope(Parser *this, ParserInStream *stream, Scope *scope, Expression *expression) {
    ParserResult result = ParserResult_Success;
    Vector expressions = Vector_new(this->allocator, sizeof(Expression));
    Vector_init(&expressions, 8);

    while (1) {
        ParserChar c = ParserInStream_getc(stream);
        if (c == PARSER_EOF) {
            break;
        }

        if (!ParserUtil_isWhitespace(c)) {
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

ParserResult Parser_parseUnit(Parser *this, InStream is, StringView path, Unit *unit) {
    ParserInStream stream = {
        .parent = NULL,
        .stream = is,
        .path = path
    };

    ParserInStream_init(&stream, this->allocator);

    Unit_create(unit, this->allocator, this->globalScope);

    ParserResult result = Parser_parseScope(this, &stream, &unit->scope, &unit->root);

    if (!ParserResult_isSuccess(result)) {
        OutStream_putc(this->logStream, '\n');
        ParserInStream_describe(&stream, this->logStream);
        OutStream_putc(this->logStream, '\n');
    }


    ParserInStream_destroy(&stream);

    return result;
}
