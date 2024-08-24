typedef struct ParserInStream {
    struct ParserInStream *parent;
    InStream stream;
    StringView path;
    Vector buffer;
    Vector lineLengths;
    uint32_t line;
    uint32_t character;
} ParserInStream;

void ParserInStream_init(ParserInStream *this, Allocator allocator) {
    this->line = 1;
    this->character = 0;

    Vector_create(&this->buffer, allocator, sizeof(ParserChar));
    Vector_init(&this->buffer, 32);

    Vector_create(&this->lineLengths, allocator, sizeof(uint32_t));
    Vector_init(&this->lineLengths, 32);
}

void ParserInStream_destroy(ParserInStream *this) {
    Vector_destroy(&this->buffer);
    Vector_destroy(&this->lineLengths);
}

ParserChar ParserInStream_getc(ParserInStream *this) {
    ParserChar c;

    ParserChar *buf = (ParserChar*)Vector_pop(&this->buffer);
    if (buf != NULL) {
        c = *buf;
    } else {
        c = (ParserChar)InStream_getc(this->stream);
    }

    if (ParserChar_isNewline(c)) {
        *(uint32_t*)Vector_push(&this->lineLengths) = this->character;
        this->line++;
        this->character = 0;
    } else {
        this->character++;
    }

    return c;
}

void ParserInStream_ungetc(ParserInStream *this, ParserChar c) {
    if (c == PARSER_EOF) {
        return;
    }

    if (ParserChar_isNewline(c)) {
        this->line--;
        this->character = *(uint32_t*)Vector_pop(&this->lineLengths);
    } else {
        this->character--;
    }

    *(ParserChar*)Vector_push(&this->buffer) = c;
}

bool ParserInStream_end(ParserInStream *this) {
    if (this->buffer.size > 0) {
        return false;
    }
    return InStream_end(this->stream);
}

void ParserInStream_describe(ParserInStream *this, OutStream os) {
    char buf[128];
    snprintf(buf, 128, ": L%uC%u -%zu", this->line, this->character, this->buffer.size);
    OutStream_writes(os, this->path);
    OutStream_puts(os, buf);
}
