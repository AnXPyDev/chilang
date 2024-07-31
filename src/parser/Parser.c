typedef struct {
    Allocator allocator;
    OutStream logStream;
    Scope *globalScope;
} Parser;

void Parser_create(Parser *this) {

}

ParserResult Parser_readToken(Parser *this, ParserInStream *stream, Scope *scope, Vector *buffer) {
    while (1) {
        ParserChar c = ParserInStream_getc(stream);

        if (c == PARSER_EOF || !ParserUtil_isTokenChar(c)) {
            ParserInStream_ungetc(stream, c);
            break;
        }

        *(char*)Vector_push(buffer) = (char)c;
    }

    return ParserResult_Success;
}

ParserResult Parser_parseExpression(Parser *this, ParserInStream *stream, Scope *scope, Expression *expression) {
    ParserResult result = ParserResult_Success;

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
            char _buffer[PARSER_TOKEN_MAX_LEN];
            Vector buffer = Vector_static(_buffer);
            Object object = Object_NULL;

            result = Parser_readToken(this, stream, scope, &buffer);

            if (!ParserResult_isSuccess(result)) {
                return result;
            }

            StringView token = CArray_CDataBuffer(Vector_view(&buffer));

            Member *member = Scope_get_member(scope, token);

            if (member) {
                OutStream_puts(this->logStream, "found member: ");
                Type_repr(member->type, this->logStream);
                OutStream_putc(this->logStream, ' ');
                OutStream_write(this->logStream, token);
                OutStream_putc(this->logStream, '\n');
            } else {
                OutStream_puts(this->logStream, "found token: ");
                OutStream_write(this->logStream, token);
                OutStream_putc(this->logStream, '\n');
            }
        };
    }

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
                goto cleanup;
            }

            if (!Expression_isNull(expression)) {
                *(Expression*)Vector_push(&expressions) = expression;
            }

            OutStream_puts(this->logStream, "found expression\n");
        };
    }

    cleanup:;

    Vector_destroy(&expressions);

    return result;
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
