TYPE_DECL(struct, SimValue);

struct SimValue {
    Type type;
    char data[];
};

void SimValue_repr();