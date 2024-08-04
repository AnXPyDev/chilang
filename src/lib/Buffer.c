typedef struct {
    char *data;
    Size size;
} Buffer;

typedef struct {
    const char *data;
    Size size;
} BufferView;

/*
typedef Buffer DataBuffer;
typedef BufferView CDataBuffer;
*/

typedef Buffer StringBuffer;
typedef BufferView StringView;

const Buffer Buffer_NULL = {
    .data = NULL,
    .size = 0
};

/*
#define DataBuffer_NULL Buffer_NULL 
#define CDataBuffer_NULL Buffer_NULL
*/

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
} ArrayView;

/*
typedef ArrayView CArray;
typedef ArrayView VectorView;
*/

const Array Array_NULL = {
    .data = NULL,
    .size = 0
};

//#define CArray_NULL Array_NULL

ArrayView BufferView_toArray(BufferView buf) {
    return (ArrayView) {
        .data = (const void*)buf.data,
        .size = buf.size
    };
}

BufferView CharArrayView_toBuffer(ArrayView arr) {
    return (BufferView) {
        .data = (const char*)arr.data,
        .size = arr.size 
    };
}

BufferView Buffer_view(Buffer buf) {
    return (BufferView) {
        .size = buf.size,
        .data = buf.data
    };
}

/*
#define CArray_CDataBuffer ArrayView_toBuffer
#define CDataBuffer_CArray BufferView_toArray
*/

#define toBuffer(arr) ((Buffer) { .data = &arr[0], .size = sizeof(arr) })
#define toBufferView(arr) ((BufferView) { .data = &arr[0], .size = sizeof(arr) })
#define toArray(arr) ((Array) { .data = &arr[0], .size = ARRSIZE(arr) })
#define toArrayView(arr) ((ArrayView) { .data = &arr[0], .size = ARRSIZE(arr) })

/*
#define toCArray toArrayView
*/

#define LocalBuffer(sz) ((Buffer) { .data = (char[sz]) {0}, .size = (sz) })
#define LocalArray(T, sz) ((Array) { .data = (T[sz]) {0}, .size = (sz) })

