ParserResult Parser_parseUnit(Parser *this, InStream is, StringView path, Expression *out_expression) {
    ParserInStream stream = {
        .parent = NULL,
        .stream = is,
        .path = path
    };
    
    ParserInStream_init(&stream, this->allocator);

    ParserFrame global = {
        .parent = NULL,
        .frame = this->globalFrame
    };

    ParserResult result = Parser_parseFrame(this, &stream, &global, out_expression);

    if (!ParserResult_isSuccess(result)) {
    }

    ParserInStream_destroy(&stream);

    return result;
}