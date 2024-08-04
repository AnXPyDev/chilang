ParserResult Parser_parseExpression(Parser *this, ParserInStream *stream, Scope *scope, Expression *expression) {
    ParserResult result = ParserResult_Success;

    Vector tokens = Vector_new(this->allocator, sizeof(StringBuffer));
    Vector_init(&tokens, 16);

    Size unconsumed_index = 0;    

    while (1) {
        ParserChar c = ParserInStream_getc(stream);
        if (c == PARSER_EOF) {
            break;
        }

        if (ParserChar_isTokenBegin(c)) {
            ParserInStream_ungetc(stream, c);
            goto read_token; 
        }

        if (ParserChar_isLiteralBegin(c)) {
            ParserInStream_ungetc(stream, c);
            goto read_literal;
        }

        if (ParserChar_isExpressionEndInclusive(c)) {
            break;
        }

        if (ParserChar_isExpressionEndExclusive(c)) {
            ParserInStream_ungetc(stream, c);
            break;    
        }

        if (ParserChar_isWhitespace(c)) {
            continue;
        }

        result = (ParserResult) { .code = PARSER_CODE_UNEXPECTED_CHAR };
        goto cleanup;

        continue;
        read_token: {

            StringBuffer buffer = LocalBuffer(PARSER_TOKEN_MAX_LEN);
            result = Parser_readToken(this, stream, scope, &buffer);

            if (!ParserResult_isSuccess(result)) {
                goto cleanup;
            }

            *(StringBuffer*)Vector_push(&tokens) = Buffer_copy(buffer, this->allocator);

            goto consume_tokens;
        };

        continue;
        consume_tokens: {
            StringBuffer *t0 = (StringBuffer*)Vector_get(&tokens, unconsumed_index);
            if (t0 == NULL) {
                continue;
            }

            Member *m0 = Scope_get_member(scope, Buffer_view(*t0));

            // first token references type member
            if (m0 != NULL && Type_isPrimitiveS(m0->type, TYPE_TYPE)) {
                StringBuffer *t1 = (StringBuffer*)Vector_get(&tokens, unconsumed_index + 1);
                if (t1 == NULL) {
                    continue;
                }

                Member *m1 = Scope_get_local_member(scope, Buffer_view(*t1));

                // second token is unused -> member declaration
                if (m1 == NULL) {
                    Type type = *(Type*)m0->object.target;
                    Member *member = Scope_add_member(scope, Buffer_view(*t1));
                    member->type = Type_copy(type, scope->allocator);
                    member->object = Object_NULL;
                    unconsumed_index++;
                }

            }
        };

        continue;
        read_literal: {
            Expression literal = Expression_NULL;
            result = Parser_parseLiteral(this, stream, &literal);
            if (!ParserResult_isSuccess(result)) {
                goto cleanup;
            }
        };
    }

    /*
    {
        OutStream_puts(this->logStream, "Found Tokens: ");
        TokenMember *end = (TokenMember*)Vector_end(&tokens);
        for (TokenMember *token = (TokenMember*)Vector_begin(&tokens); token < end; token++) {
            OutStream_write(this->logStream, Buffer_view(token->token));
            if (token->member != NULL) {
                OutStream_putc(this->logStream, '(');
                Type_repr(token->member->type, this->logStream);
                OutStream_putc(this->logStream, ')');
            }
            OutStream_puts(this->logStream, " ");
        }
    }
    */

    cleanup: {
        StringBuffer *end = (StringBuffer*)Vector_end(&tokens);
        for (StringBuffer *token = (StringBuffer*)Vector_begin(&tokens); token < end; token++) {
            Buffer_free(*token, this->allocator);
        }
    };

    return result;
}

#undef TokenMember