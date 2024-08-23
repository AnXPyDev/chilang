#define ARENA_ALLOCATOR_ALIGNMENT 8
#define ARENA_ALLOCATOR_BUCKETS 16
#define ARENA_ALLOCATOR_PREDICTED_HEADERS 32

typedef struct ArenaAllocator_Chunk {
    struct ArenaAllocator_Chunk *previous;
    Size size;
    Size offset;
    char data[];
} ArenaAllocator_Chunk;

typedef struct {
    Allocator provider;
    Size chunkSize;
    ArenaAllocator_Chunk *chunk;
} ArenaAllocator;

static inline Size ArenaAllocator_align(Size offset) {
    Size d = offset % ARENA_ALLOCATOR_ALIGNMENT;
    return offset + (ARENA_ALLOCATOR_ALIGNMENT - d) * !!d;
}

#define ALIGN ArenaAllocator_align

typedef struct {
    Size size;
} ArenaAllocator_BufHeader;

#define HEADER_SIZE ALIGN(sizeof(ArenaAllocator_BufHeader))

void ArenaAllocator_push(ArenaAllocator *this, Size size) {
    ArenaAllocator_Chunk *chunk = Allocator_malloc(this->provider, sizeof(ArenaAllocator_Chunk) + ALIGN(size) + HEADER_SIZE * ARENA_ALLOCATOR_PREDICTED_HEADERS);
    chunk->previous = this->chunk;
    chunk->offset = 0;
    chunk->size = size;
    this->chunk = chunk;
    //fprintf(stderr, "push %llu\n", size);
}

void ArenaAllocator_create(ArenaAllocator *this, Allocator provider, Size chunkSize) {
    this->provider = provider;
    this->chunkSize = chunkSize;
    this->chunk = NULL;
    ArenaAllocator_push(this, this->chunkSize);
}

#define BUFSIZE(buf) (Size*)()

#define this ((ArenaAllocator*)vthis)

void *ArenaAllocator_malloc(void *vthis, Size size) {
    //fprintf(stderr, "malloc %llu\n", size);

    if (this->chunk->offset + size > this->chunk->size) {
        ArenaAllocator_push(this, MAX2(size, this->chunkSize));
    }

    const Size offset = this->chunk->offset;
    ArenaAllocator_BufHeader *header = (ArenaAllocator_BufHeader*)(this->chunk->data + offset);
    header->size = size;
    this->chunk->offset = ALIGN(offset + size + HEADER_SIZE);

    return (char*)header + HEADER_SIZE;
}

void ArenaAllocator_free(void *vthis, void *buf) {
    //fprintf(stderr, "free\n");
}

void *ArenaAllocator_realloc(void *vthis, void *buf, Size size) {
    //fprintf(stderr, "realloc\n");
    ArenaAllocator_BufHeader *header = (ArenaAllocator_BufHeader*)((char*)buf - HEADER_SIZE);
    void *newBuf = ArenaAllocator_malloc(this, size);
    memcpy(newBuf, buf, MIN2(header->size, size));
    return newBuf;
}

void ArenaAllocator_destroy(void *vthis) {
    ArenaAllocator_Chunk *chunk = this->chunk;
    while (chunk != NULL) {
        ArenaAllocator_Chunk *prev = chunk->previous;
        Allocator_free(this->provider, chunk);
        chunk = prev;
    }
}

#undef this

const IAllocator IArenaAllocator = {
    .free = &ArenaAllocator_free,
    .malloc = &ArenaAllocator_malloc,
    .realloc = &ArenaAllocator_realloc,
    .destroy = &ArenaAllocator_destroy
};

Allocator ArenaAllocator_upcast(ArenaAllocator *this) {
    return (Allocator) {
        .interface = &IArenaAllocator,
        .object = this
    };
}

#undef ALIGN
#undef HEADER_SIZE