typedef struct {
    int (*getc)(void *this);
    Size (*read)(void *this, DataBuffer buf);
    bool (*end)(void *this);
    void (*close)(void *this);
    void *(*info)(void *this);
} IInStream;

typedef struct {
    const IInStream *interface;
    void *object;
} InStream;


int InStream_getc(InStream this) {
    return this.interface->getc(this.object);
}

Size InStream_read(InStream this, DataBuffer buf) {
    return this.interface->read(this.object, buf);
}

bool InStream_end(InStream this) {
    return this.interface->end(this.object);
}

void InStream_close(InStream this) {
    if (this.interface->close != NULL) {
        this.interface->close(this.object);
    }
}

void *InStream_info(InStream this) {
    if (this.interface->info != NULL) {
        return this.interface->info(this.object);
    }

    return NULL;
}