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