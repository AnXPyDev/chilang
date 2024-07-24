typedef struct {
    OutStream os;
    StringView delimiter;
    bool first;
} DelimOutStream;

#define this ((DelimOutStream*)vthis)

void DelimOutStream_putc(void *vthis, int c) {
    OutStream_putc(this->os, c);
}

void DelimOutStream_puts(void *vthis, const char *str) {
    OutStream_puts(this->os, str);
}

void DelimOutStream_write(void *vthis, CDataBuffer buf) {
    OutStream_write(this->os, buf);
}

void DelimOutStream_close(void *vthis) {
    OutStream_close(this->os);
}

void DelimOutStream_flush(void *vthis) {
    OutStream_flush(this->os);
}

void DelimOutStream_begin_item(void *vthis) {
    OutStream_begin_item(this->os);

    if (this->first) {
        this->first = false;
        return;
    }

    OutStream_write(this->os, this->delimiter);
}

void DelimOutStream_end_item(void *vthis) {
    OutStream_end_item(this->os);
}

#undef this

const IOutStream IDelimOutStream = {
    .putc = &DelimOutStream_putc,
    .puts = &DelimOutStream_puts,
    .write = &DelimOutStream_write,
    .close = &DelimOutStream_close,
    .flush = &DelimOutStream_flush,

    .begin_item = &DelimOutStream_begin_item,
    .end_item = &DelimOutStream_end_item
};

void DelimOutStream_create(DelimOutStream *this, OutStream parent, StringView delimiter) {
    this->os = parent;
    this->delimiter = delimiter;
    this->first = true;
}

DelimOutStream DelimOutStream_new(OutStream parent, StringView delimiter) {
    DelimOutStream result;
    DelimOutStream_create(&result, parent, delimiter);
    return result;
}

OutStream DelimOutStream_OutStream(DelimOutStream *this) {
    return (OutStream) {
        .interface = &IDelimOutStream,
        .object = (void*)this
    };
}