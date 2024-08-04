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
    void *buf = this.interface->malloc(this.object, size);
    if (buf == NULL) {
        fprintf(stderr, "\nAllocation of buffer failed (Allocator_malloc | %llu)\n", size);
        abort();
    }

    return buf; 
}

void *Allocator_calloc(Allocator this, Size size) {
    void *buf = this.interface->calloc(this.object, size);
    if (buf == NULL) {
        fprintf(stderr, "\nAllocation of buffer failed (Allocator_calloc | %llu)\n", size);
        abort();
    }

    return buf;
}

void *Allocator_realloc(Allocator this, void *mem, Size size) {
    void *buf = this.interface->realloc(this.object, mem, size);
    if (buf == NULL) {
        fprintf(stderr, "\nAllocation of buffer failed (Allocator_realloc | %llu)\n", size);
        abort();
    }

    return buf;
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