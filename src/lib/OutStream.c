typedef struct {
    void (*putc)(void *this, int c);
    void (*puts)(void *this, const char *s);
    void (*write)(void *this, CDataBuffer buf);
    void (*flush)(void *this);
    void (*close)(void *this);

    void (*begin_item)(void *this);
    void (*end_item)(void *this);
} IOutStream;

typedef struct {
    const IOutStream *interface;
    void *object;
} OutStream;

typedef struct {
    void (*print)(const void *this, OutStream stream);
    const void *object;
} Printable;

void OutStream_putc(OutStream this, int c) {
    this.interface->putc(this.object, c);
}

void OutStream_write(OutStream this, CDataBuffer buf) {
    if (this.interface->write != NULL) {
        this.interface->write(this.object, buf);
        return;
    }

    for (Size i = 0; i < buf.size; i++) {
        OutStream_putc(this, (int)buf.data[i]);
    }
}

void OutStream_puts(OutStream this, const char *s) {
    if (this.interface->puts != NULL) {
        this.interface->puts(this.object, s);
        return;
    }

    OutStream_write(this, strview(s));
}

void OutStream_writes(OutStream this, CDataBuffer buf) {
    if (buf.data == NULL) {
        OutStream_puts(this, "(null)");
        return;
    }

    OutStream_write(this, buf);
}


void OutStream_flush(OutStream this) {
    if (this.interface->flush != NULL) {
        this.interface->flush(this.object);
        return;
    }
}

void OutStream_close(OutStream this) {
    if (this.interface->close != NULL) {
        this.interface->close(this.object);
        return;
    }
}

void OutStream_beginItem(OutStream this) {
    if (this.interface->begin_item != NULL) {
        this.interface->begin_item(this.object);
    }
}

void OutStream_endItem(OutStream this) {
    if (this.interface->end_item != NULL) {
        this.interface->end_item(this.object);
    }
}

#define OutStream_begin_item OutStream_beginItem
#define OutStream_end_item OutStream_endItem

void OutStream_print(OutStream this, Printable printable) {
    printable.print(printable.object, this);
}

