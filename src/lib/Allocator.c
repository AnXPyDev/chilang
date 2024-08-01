typedef struct {
    void *(*malloc )(void *this, Size size);
    void *(*calloc )(void *this, Size size);
    void *(*realloc)(void *this, void *mem, Size size);
    void  (*free   )(void *this, void *mem);
    void  (*destroy)(void *this);
} IAllocator;

typedef struct {
    const IAllocator *interface;
    void *object;
} Allocator;

const Allocator Allocator_NULL = {
    .interface = NULL,
    .object = NULL
};

void *Allocator_malloc(Allocator this, Size size) {
    return this.interface->malloc(this.object, size);
}

void *Allocator_calloc(Allocator this, Size size) {
    return this.interface->calloc(this.object, size);
}

void *Allocator_realloc(Allocator this, void *mem, Size size) {
    return this.interface->realloc(this.object, mem, size);
}

void Allocator_free(Allocator this, void *mem) {
    if (mem == NULL) {
        return;
    }
    this.interface->free(this.object, mem);
}

void Allocator_destroy(Allocator this) {
    this.interface->destroy(this.object);
}

DataBuffer Allocator_copy(Allocator this, CDataBuffer buf) {
    DataBuffer result = {
        .size = buf.size,
        .data = (char*)Allocator_malloc(this, buf.size)
    };

    memcpy(result.data, buf.data, result.size);

    return result;
}