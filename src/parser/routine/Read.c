ParserResult Parser_readToken(Parser *this, ParserInStream *stream, Scope *scope, StringBuffer *buffer) {
    char *bp = buffer->data;
    Size s = 0;

    while (1) {
        ParserChar c = ParserInStream_getc(stream);

        if (c == PARSER_EOF || !ParserChar_isToken(c)) {
            ParserInStream_ungetc(stream, c);
            break;
        }

        if (s + 1 > buffer->size) {
            return ParserResult_construct(this, stream, PARSER_CODE_TOKEN_TOO_LONG, BufferView_NULL);
        }

        *bp = (char)c;
        bp++;
        s++;
    }

    buffer->size = s;

    return ParserResult_Success;
}