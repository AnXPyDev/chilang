typedef struct {
    char *data;
    Size size;
} DataBuffer;

typedef struct {
    const char *data;
    Size size;
} CDataBuffer;

typedef CDataBuffer StringView;

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