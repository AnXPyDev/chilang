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
        fprintf(stderr, "\nABORT: Allocator %p failed (Allocator_malloc | %llu)\n", this.object, size);
        fflush(stderr);
        abort();
    }

    return buf; 
}

/*
void *Allocator_calloc(Allocator this, Size size) {
    if (this.interface->calloc != NULL) {
        void *buf = this.interface->calloc(this.object, size);
        if (buf == NULL) {
            fprintf(stderr, "\nABORT: Allocator %p failed (Allocator_calloc | %llu)\n", this.object, size);
            fflush(stderr);
            abort();
        }
        return buf;
    }

    void *buf = Allocator_malloc(this, size);
    memset(buf, 0, size);
    return buf;
}
*/

void *Allocator_realloc(Allocator this, void *mem, Size size) {
    void *buf = this.interface->realloc(this.object, mem, size);
    if (buf == NULL) {
        fprintf(stderr, "\nABORT: Allocator %p failed (Allocator_realloc | %llu)\n",this.object, size);
        fflush(stderr);
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

#define Allocator_new(allocator, T) ((T*)Allocator_malloc((allocator), sizeof(T)))