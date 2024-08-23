typedef struct {
    VmExitCode code;
} VmOperation_RET;

typedef struct {
    Size n;
    VmAddress dest;
    void *src;
} VmOperation_READ;

typedef struct {
    Size n;
    void *dest;
    VmAddress src;
} VmOperation_WRITE;

typedef struct {
    Size n;
    VmAddress dest;
    VmAddress src;
} VmOperation_CPY;

typedef struct {
    VmAddress r;
    VmAddress a;
    VmAddress b;
} VmOperation_GENERIC;

typedef struct {
    Size target;
    VmAddress cond;
} VmOperation_JMP;

typedef struct {
    VmProcedure *procedure;
    VmAddress frame;
} VmOperation_CALL;

typedef struct {
    VmAddress dest;
    VmInt val;
} VmOpeartion_SET;