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
            return -1;
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
    return 1;
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

    uint8_t acc[8] = {0};
    Size bits;

    Parser_convertNumericLiteralLE(Buffer_view(buffer), acc, 8, &bits);

    fprintf(stderr, "numeric literal %d %llu\n", *(int*)acc, bits);

    return ParserResult_construct_INTERNAL_ERROR(this, stream);
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