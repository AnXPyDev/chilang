typedef struct {
    int (*getc)(void *this);
    Size (*read)(void *this, DataBuffer buf);
    int (*end)(void *this);
    int (*close)(void *this);
} IInStream;

typedef struct {
    const IInStream *interface;
    void *object;
} InStream;


int InStream_getc(InStream this) {
    return this.interface->getc(this.object);
}

int InStream_read(InStream this, DataBuffer buf) {
    return this.interface->read(this.object, buf);
}

int InStream_end(InStream this) {
    return this.interface->end(this.object);
}

int InStream_close(InStream this) {
    return this.interface->close(this.object);
}