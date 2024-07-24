typedef struct {
    const char *begin;
    const char *end;
    const char *ptr;
} StringInStream;

#define this ((StringInStream*)vthis)

int StringInStream_getc(void *vthis) {
    int c = *this->ptr;
    this->ptr++;
    return c;
}

Size StringInStream_read(void *vthis, DataBuffer buf) {
    Size copied = MIN2(buf.size, this->end - this->ptr);
    memcpy(buf.data, this->ptr, copied);
    this->ptr += copied;
    return copied;
}

bool StringInStream_end(void *vthis) {
    return this->ptr >= this->end;
}

#undef this

const IInStream IStringInStream = {
    .getc = &StringInStream_getc,
    .read = &StringInStream_read,
    .end = &StringInStream_end,
    .close = NULL
};

void StringInStream_create(StringInStream *this, CDataBuffer buf) {
    this->begin = buf.data;
    this->end = buf.data + buf.size;
    this->ptr = buf.data;
}

StringInStream StringInStream_new(CDataBuffer buf) {
    StringInStream this;
    StringInStream_create(&this, buf);
    return this;
}

StringInStream StringInStream_newString(const String *string) {
    return StringInStream_new(String_view(string));
}

InStream StringInStream_InStream(StringInStream *this) {
    InStream instream = { &IStringInStream, this };
    return instream;
}