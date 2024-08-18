ParserResult Parser_parseExpression(
    Parser *this,
    ParserInStream *stream,
    ParserFrame *frame,
    Type desiredType,
    Expression *out_expression
) {
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
            result = Parser_readToken(this, stream, &buffer);

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

            Member *m0 = ParserFrame_getMember(frame, t0, AnyMemberMatcher, SmartTypeMatcher_upcast(&PTYPE_ANY));

            // one token
            
            if (m0 == NULL) {
                result = ParserResult_construct_TOKEN_UNKNOWN(this, stream, t0);
                goto error;
            }

            if (Type_isPrimitiveS(m0->type, TYPE_KEYWORD)) {
                EKeyword keyword = *(EKeyword*)m0->object.target;
                switch (keyword) {
                    case KEYWORD_PRINT:
                        result = Parser_parsePrintExpression(this, stream, frame, out_expression);
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
            
            Member *m1 = ParserFrame_getMember(frame, t1, AnyMemberMatcher, SmartTypeMatcher_upcast(&PTYPE_ANY));

            // second token is keyword
            if (m1 != NULL && Type_isPrimitiveS(m1->type, TYPE_KEYWORD)) {
                EKeyword keyword = *(EKeyword*)m1->object.target;
                switch (keyword) {
                    case KEYWORD_ASSIGN:
                        result = Parser_parseAssignmentExpression(this, stream, frame, t0, out_expression);
                        goto cleanup;
                    default:;
                        result = ParserResult_construct_TOKEN_UNEXPECTED(this, stream, t1);
                        goto error;
                }
            }

            // first token references type and second token is undeclared -> member declaration 
            if (m1 == NULL && Type_isPrimitiveS(m0->type, TYPE_TYPE)) {
                Type type = *(Type*)m0->object.target;
                Member *member = ParserFrame_addMember(frame, t1);
                member->type = Type_copy(type, this->allocator);
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
            result = Parser_parseLiteral(this, stream, desiredType, out_expression);
            if (!ParserResult_isSuccess(result)) {
                goto error;
            }
        };
    }

    {
        StringBuffer *t0_ = Vector_get(&tokens, unconsumed_index);
        if (t0_ == NULL) {
            goto cleanup;
        }

        StringView t0 = Buffer_view(*t0_);

        Member *m0 = ParserFrame_getMember(frame, t0, AnyMemberMatcher, SmartTypeMatcher_upcast(&desiredType));

        if (m0 == NULL) {
            result = ParserResult_construct_TOKEN_UNKNOWN(this, stream, t0);
            goto error;
        }

        *out_expression = GetExpression_create(m0->type, t0, this->allocator);
        goto cleanup;
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