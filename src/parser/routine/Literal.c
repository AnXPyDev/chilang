#define PARSER_MAX_STRING_LENGTH

int Parser_convertNumericLiteralLE(StringView literal, uint8_t *target, Size capacity, Size *out_bits) {
    capacity *= 8;
    uint8_t buf[PARSER_NUMERIC_LITERAL_MAX_LEN];
    for (Size i = 0; i < literal.size; i++) {
        buf[i] = (uint8_t)(literal.data[i] - '0');
    }


    Size bitOffset = 0;
    Size begin = 0;
    while (1) {
        if (buf[begin] == 0) {
            begin++;
        }

        if (begin == literal.size) {
           break; 
        }

        if (bitOffset >= capacity) {
            return 1;
        }

        uint8_t carry = 0;
        for (Size i = begin; i < literal.size; i++) {
            uint8_t val = buf[i];
            buf[i] = (uint8_t)(val / 2 + carry * 5); 
            carry = val % 2;
        }

        target[bitOffset / 8] |= (carry << (bitOffset % 8));

        bitOffset++;
    }

    *out_bits = bitOffset;
    return 0;
}

ParserResult Parser_parseNumericLiteral(
    Parser *this,
    ParserInStream *stream,
    Type desiredType,
    Expression *out_expression
) {
    StringBuffer buffer = LocalBuffer(PARSER_NUMERIC_LITERAL_MAX_LEN);
    char *bp = buffer.data;
    Size s = 0;

    while (1) {
        ParserChar c = ParserInStream_getc(stream);
        if (c == PARSER_EOF) {
           return ParserResult_construct(this, stream, PARSER_CODE_UNEXPECTED_EOF, BufferView_NULL);
        }

        if (!ParserChar_isNumericLiteral(c)) {
            ParserInStream_ungetc(stream, c);
            break;
        }

        if (s + 1 > buffer.size) {
            return ParserResult_construct(this, stream, PARSER_CODE_LITERAL_TOO_LONG, BufferView_NULL);
        }

        *bp = (char)c;
        s++;
        bp++;
    }

    buffer.size = s;

    uint8_t value[8] = {0};
    Size size;

    if (Parser_convertNumericLiteralLE(Buffer_view(buffer), value, 8, &size)) {
        return ParserResult_construct(this, stream, PARSER_CODE_INTERNAL_ERROR, strview("convert failed"));
    }

    if (!Type_isUnsignedInteger(desiredType)) {
        return ParserResult_construct(this, stream, PARSER_CODE_INTERNAL_ERROR, strview("bad type"));
    }

    TypeInfo ti = Type_info(desiredType);
    size = size / 8 + (size % 8 > 0);

    if (size > ti.size) {
        return ParserResult_construct(this, stream, PARSER_CODE_INTERNAL_ERROR, strview("integer literal too large for type"));
    }
    
    *out_expression = LiteralExpression_create(desiredType, (char*)value, this->allocator);

    return ParserResult_Success;
}


ParserResult Parser_parseLiteral(
    Parser *this,
    ParserInStream *stream,
    Type desiredType,
    Expression *out_expression
) {
    ParserChar c = ParserInStream_getc(stream);
    if (c == PARSER_EOF) {
        return ParserResult_construct_INTERNAL_ERROR(this, stream);
    }

    if (ParserChar_isNumericLiteralBegin(c)) {
        ParserInStream_ungetc(stream, c);
        return Parser_parseNumericLiteral(this, stream, desiredType, out_expression);
    }

    return ParserResult_construct_INTERNAL_ERROR(this, stream);
}