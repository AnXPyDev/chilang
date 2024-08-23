#include <stdio.h>
#include <stddef.h>

typedef size_t Size;
#define ARENA_ALLOCATOR_ALIGNMENT 4

static inline Size ArenaAllocator_align(Size offset) {
    Size d = offset % ARENA_ALLOCATOR_ALIGNMENT;
    return offset + (ARENA_ALLOCATOR_ALIGNMENT - d) * !!d;
}

int main() {
    for (Size i = 0; i < 99; i++) {
        printf("%llu -> %llu\n", i, ArenaAllocator_align(i));
    }
    return 0;
}