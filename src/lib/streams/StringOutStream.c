typedef struct {
    Vector buffer;
} StringOutStream;

#define this ((StringOutStream*)vthis)

void StringOutStream_putc(void *vthis, int c) {
    *(char*)Vector_push(&this->buffer) = (char)c;
}

void StringOutStream_write(void *vthis, CDataBuffer buf) {
    Vector_append(&this->buffer, (CArray) { .data = (const void*)buf.data, .size = buf.size });
}

void StringOutStream_puts(void *vthis, const char *str) {
    StringOutStream_write(vthis, strview(str));
}

void StringOutStream_close(void *vthis) {
    Vector_destroy(&this->buffer);
}

#undef this

const IOutStream IStringOutStream = {
    .putc = StringOutStream_putc,
    .puts = StringOutStream_puts,
    .write = StringOutStream_write,
    .flush = NULL,
    .close = StringOutStream_close,
    
    .begin_item = NULL,
    .end_item = NULL
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

OutStream StringOutStream_upcast(StringOutStream *this) {
    return (OutStream) {
        .interface = &IStringOutStream,
        .object = (void*)this
    };
}

void StringOutStream_copy(StringOutStream *this, String *string) {
    String_copy(string, CArray_CDataBuffer(Vector_view(&this->buffer)));
}

void StringOutStream_destroy(StringOutStream *this) {
    Vector_destroy(&this->buffer);
}