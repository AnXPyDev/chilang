typedef struct {
    Type type;
    char data[];
} SimValue;

void SimValue_repr()