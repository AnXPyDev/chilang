ParserResult ParserResult_construct_TOKEN_UNKNOWN(Parser *parser, ParserInStream *stream, StringView token) {
    char message[256];
    snprintf(message, ARRSIZE(message), "Unknown token '%.*s'", (int)token.size, token.data);
    return ParserResult_construct(parser, stream, PARSER_CODE_TOKEN_UNKNOWN, strview(message));
}

ParserResult ParserResult_construct_TOKEN_UNEXPECTED(Parser *parser, ParserInStream *stream, StringView token) {
    char message[256];
    snprintf(message, ARRSIZE(message), "Unexpected token '%.*s'", (int)token.size, token.data);
    return ParserResult_construct(parser, stream, PARSER_CODE_TOKEN_UNEXPECTED, strview(message));
}

ParserResult ParserResult_construct_UNIMPLEMENTED(Parser *parser, ParserInStream *stream, StringView feature) {
    char message[256];
    snprintf(message, ARRSIZE(message), "Unimplemented feature '%.*s'", (int)feature.size, feature.data);
    return ParserResult_construct(parser, stream, PARSER_CODE_UNIMPLEMENTED, strview(message));    
}

ParserResult ParserResult_construct_UNEXPECTED_CHAR(Parser *parser, ParserInStream *stream, ParserChar c) {
    char message[256];
    snprintf(message, ARRSIZE(message), "Unexpected character '%c'", (char)c);
    return ParserResult_construct(parser, stream, PARSER_CODE_UNEXPECTED_CHAR, strview(message));    
}