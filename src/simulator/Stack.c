#define VM_STACK_SIZE 1000000

typedef Size VmAddress;

typedef struct {
    char *fptr;
    char begin[VM_STACK_SIZE];
} VmStack;

static inline void *VmStack_getPtr(VmStack *this, VmAddress addr) {
    return (void*)(this->fptr + addr);
}