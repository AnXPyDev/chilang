typedef struct {
    int (*putc)(void *this, int c);
    int (*puts)(void *this, const char *s);
    int (*write)(void *this, CDataBuffer buf);
    int (*flush)(void *this);
    int (*close)(void *this);
} IOutStream;

typedef struct {
    const IOutStream *interface;
    void *object;
} OutStream;

typedef struct {
    void (*print)(const void *this, OutStream stream);
    const void *object;
} Printable;

int OutStream_putc(OutStream this, int c) {
    return this.interface->putc(this.object, c);
}

int OutStream_puts(OutStream this, const char *s) {
    return this.interface->puts(this.object, s);
}

int OutStream_write(OutStream this, CDataBuffer buf) {
    return this.interface->write(this.object, buf);
}

int OutStream_flush(OutStream this) {
    return this.interface->flush(this.object);
}

int OutStream_close(OutStream this) {
    return this.interface->close(this.object);
}

void OutStream_print(OutStream this, Printable printable) {
    printable.print(printable.object, this);
}
