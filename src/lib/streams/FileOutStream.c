typedef struct {
    FILE *fp;
} FileOutStream;


#define this ((FileOutStream*)vthis)

void FileOutStream_putc(void *vthis, int c) {
    fputc(c, this->fp);
}

void FileOutStream_puts(void *vthis, const char *s) {
    fputs(s, this->fp);
}

void FileOutStream_write(void *vthis, CDataBuffer buf) {
    fwrite(buf.data, 1, buf.size, this->fp);
}

void FileOutStream_flush(void *vthis) {
    fflush(this->fp);
}

void FileOutStream_close(void *vthis) {
    fclose(this->fp);
}

#undef this


const IOutStream IFileOutStream = {
    .putc = &FileOutStream_putc,
    .puts = &FileOutStream_puts,
    .write = &FileOutStream_write,
    .flush = &FileOutStream_flush,
    .close = &FileOutStream_close,
    
    .begin_item = NULL,
    .end_item = NULL
};

void FileOutStream_create(FileOutStream *this, FILE *fp) {
    this->fp = fp;
}

FileOutStream FileOutStream_new(FILE *fp) {
    FileOutStream this;
    FileOutStream_create(&this, fp);
    return this;
}

OutStream FileOutStream_OutStream(FileOutStream *this) {
    OutStream outstream = { &IFileOutStream, this };
    return outstream;
}