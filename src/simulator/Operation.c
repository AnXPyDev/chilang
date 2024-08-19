typedef enum {
    VM_OPERATION_NOP = 0,
    VM_OPERATION_RETURN,
    VM_OPERATION_JUMP,
    VM_OPERATION_CALL,
    VM_OPERATION_COPY,
    VM_OPERATION_COPY_PTR,
    VM_OPERATION_PRINT_U32,
    VM_OPERATION_PRINT_ENDL,
    VM_OPERATION_INCREMENT_U32,
    VM_OPERATION_SAYHI,
} VmOperation_Code;

typedef struct {
    VmOperation_Code opcode;
    char data[64];
} VmOperation;

typedef struct {
    Size address;
} VmOperation_JUMP;

typedef struct {
    struct VmProcedure *procedure;
} VmOperation_CALL;

typedef struct {
    Size n;
    VmAddress dest;
    VmAddress src;
} VmOperation_COPY;

typedef struct {
    Size n;
    VmAddress dest;
    void *src;
} VmOperation_COPY_PTR;