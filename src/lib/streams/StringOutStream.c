typedef struct {
    Vector buffer;
} StringOutStream;

#define this ((StringOutStream*)vthis)

int StringOutStream_putc(void *vthis, int c) {
    *(char*)Vector_push(&this->buffer) = (char)c;
    return c;
}

int StringOutStream_write(void *vthis, CDataBuffer buf) {
    Vector_append(&this->buffer, buf.data, buf.size);
    return 0;
}

int StringOutStream_puts(void *vthis, const char *str) {
    return StringOutStream_write(vthis, strview(str));
}

int StringOutStream_flush(void *vthis) {
    return 0;
}

int StringOutStream_close(void *vthis) {
    Vector_destroy(&this->buffer);
    return 0;
}

#undef this

const IOutStream IStringOutStream = {
    .putc = StringOutStream_putc,
    .puts = StringOutStream_puts,
    .write = StringOutStream_write,
    .flush = StringOutStream_flush,
    .close = StringOutStream_close
};

void StringOutStream_create(StringOutStream *this, Allocator allocator) {
    this->buffer = Vector_new(allocator, sizeof(char));
    Vector_init(&this->buffer, 32);
}

StringOutStream StringOutStream_new(Allocator allocator) {
    StringOutStream this;
    StringOutStream_create(&this, allocator);
    return this;
}

OutStream StringOutStream_OutStream(StringOutStream *this) {
    OutStream outstream = { &IStringOutStream, this };
    return outstream;
}

void StringOutStream_copy(StringOutStream *this, String *string) {
    String_copy_buffer(string, this->buffer.data, this->buffer.size);
}

void StringOutStream_move(StringOutStream *this, String *string) {
    String_move_buffer(string, this->buffer.data, this->buffer.capacity, this->buffer.size);
    Vector_nullify(&this->buffer);
    StringOutStream_close(this);
}