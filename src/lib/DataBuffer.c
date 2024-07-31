typedef struct {
    char *data;
    Size size;
} DataBuffer;

typedef struct {
    const char *data;
    Size size;
} CDataBuffer;

typedef CDataBuffer StringView;

const DataBuffer DataBuffer_NULL = {
    .data = NULL,
    .size = 0
};

const CDataBuffer CDataBuffer_NULL = {
    .data = NULL,
    .size = 0
};

const StringView StringView_NULL = CDataBuffer_NULL;

StringView strview(const char *str) {
    return (StringView) {
        .data = str,
        .size = strlen(str)
    };
}

typedef struct {
    void *data;
    Size size;
} Array;

typedef struct {
    const void *data;
    Size size;
} CArray;

const Array Array_NULL = {
    .data = NULL,
    .size = 0
};

const CArray CArray_NULL = {
    .data = NULL,
    .size = 0
};


CArray CDataBuffer_CArray(CDataBuffer buf) {
    return (CArray) {
        .data = (const void*)buf.data,
        .size = buf.size
    };
}

CDataBuffer CArray_CDataBuffer(CArray arr) {
    return (CDataBuffer) {
        .data = (const char*)arr.data,
        .size = arr.size
    };
}

typedef CArray VectorView;

#define toArray(arr) ((Array) { .data = &arr[0], .size = ARRSIZE(arr) } )
#define toCArray(arr) ((CArray) { .data = &arr[0], .size = ARRSIZE(arr) } )

