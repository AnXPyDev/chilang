typedef struct {
    FILE *fp;
} FileOutStream;


#define this ((FileOutStream*)vthis)

int FileOutStream_putc(void *vthis, int c) {
    return fputc(c, this->fp);
}

int FileOutStream_puts(void *vthis, const char *s) {
    return fputs(s, this->fp);
}

int FileOutStream_write(void *vthis, CDataBuffer buf) {
    return fwrite(buf.data, 1, buf.size, this->fp);
}

int FileOutStream_flush(void *vthis) {
    return fflush(this->fp);
}

int FileOutStream_close(void *vthis) {
    return fclose(this->fp);
}

#undef this


const IOutStream IFileOutStream = {
    .putc = &FileOutStream_putc,
    .puts = &FileOutStream_puts,
    .write = &FileOutStream_write,
    .flush = &FileOutStream_flush,
    .close = &FileOutStream_close
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