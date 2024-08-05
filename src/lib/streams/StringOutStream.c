typedef struct {
    Vector buffer;
} StringOutStream;

#define this ((StringOutStream*)vthis)

void StringOutStream_putc(void *vthis, int c) {
    *(char*)Vector_push(&this->buffer) = (char)c;
}

void StringOutStream_write(void *vthis, BufferView buf) {
    Vector_append(&this->buffer, BufferView_toArray(buf));
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
    String_copy(string, CharArrayView_toBuffer(Vector_view(&this->buffer)));
}

void StringOutStream_destroy(StringOutStream *this) {
    Vector_destroy(&this->buffer);
}

BufferView StringOutStream_view(StringOutStream *this) {
    return CharArrayView_toBuffer(Vector_view(&this->buffer));
}

#define QuickSOS(name, allocator) StringOutStream_create(&name, (allocator)); OutStream os_##name = StringOutStream_upcast(&name);