ParserResult Parser_parseExpression(Parser *this, ParserInStream *stream, Scope *scope, Type desiredType, Expression *expression) {
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

        result = ParserResult_construct_UNEXPECTED_CHAR(this, stream, c);
        goto cleanup;

        continue;
        read_token: {

            StringBuffer buffer = LocalBuffer(PARSER_TOKEN_MAX_LEN);
            result = Parser_readToken(this, stream, scope, &buffer);

            if (!ParserResult_isSuccess(result)) {
                goto error;
            }

            *(StringBuffer*)Vector_push(&tokens) = Buffer_copy(Buffer_view(buffer), this->allocator);

            goto consume_tokens;
        };

        continue;
        consume_tokens: {
            StringBuffer *t0_ = (StringBuffer*)Vector_get(&tokens, unconsumed_index);
            if (t0_ == NULL) {
                goto end_consume_token;
            }
            StringView t0 = Buffer_view(*t0_);

            Member *m0 = Scope_get_member(scope, t0);

            // one token
            
            if (m0 == NULL) {
                result = ParserResult_construct_TOKEN_UNKNOWN(this, stream, t0);
                goto error;
            }

            if (Type_isPrimitiveS(m0->type, TYPE_KEYWORD)) {
                EKeyword keyword = *(EKeyword*)m0->object.target;
                switch (keyword) {
                    case KEYWORD_PRINT:
                        result = Parser_parsePrintExpression(this, stream, scope, expression);
                        goto cleanup;
                    default:;
                        result = ParserResult_construct_TOKEN_UNEXPECTED(this, stream, t0);
                        goto error;
                }
            } 

            StringBuffer *t1_ = (StringBuffer*)Vector_get(&tokens, unconsumed_index + 1);
            if (t1_ == NULL) {
                goto end_consume_token;
            }
            StringView t1 = Buffer_view(*t1_);
            
            Member *m1 = Scope_get_member(scope, t1);

            // second token is keyword
            if (m1 != NULL && Type_isPrimitiveS(m1->type, TYPE_KEYWORD)) {
                EKeyword keyword = *(EKeyword*)m1->object.target;
                switch (keyword) {
                    case KEYWORD_ASSIGN:
                        result = Parser_parseAssignmentExpression(this, stream, scope, t0, expression);
                        goto cleanup;
                    default:;
                        result = ParserResult_construct_TOKEN_UNEXPECTED(this, stream, t1);
                        goto error;
                }
            }

            // first token references type and second token is undeclared -> member declaration 
            if (m1 == NULL && Type_isPrimitiveS(m0->type, TYPE_TYPE)) {
                Type type = *(Type*)m0->object.target;
                Member *member = Scope_add_member(scope, t1);
                member->type = Type_copy(type, scope->allocator);
                member->object = Object_NULL;
                unconsumed_index++;
                goto end_consume_token;
            }

            if (m1 == NULL) {
                result = ParserResult_construct_TOKEN_UNKNOWN(this, stream, t1);
                goto error;
            }
            

            end_consume_token:;
        };

        continue;
        read_literal: {
            Expression literal = Expression_NULL;
            result = Parser_parseLiteral(this, stream, &literal);
            if (!ParserResult_isSuccess(result)) {
                goto error;
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

    {
        StringBuffer *t0 = Vector_get(&tokens, unconsumed_index);
        if (t0 == NULL) {
            goto cleanup;
        }

        Member *m0 = Scope_get_member(scope, Buffer_view(*t0));

        if (m0 != NULL) {
            
        }
    };

    error:;
    cleanup: {
        StringBuffer *end = (StringBuffer*)Vector_end(&tokens);
        for (StringBuffer *token = (StringBuffer*)Vector_begin(&tokens); token < end; token++) {
            Buffer_free(*token, this->allocator);
        }
        Vector_destroy(&tokens);
    };

    return result;
}

#undef TokenMember