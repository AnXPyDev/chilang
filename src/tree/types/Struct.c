typedef struct {
    Type type;
    String name;
} StructEntry;

void StructEntry_copy(StructEntry *this, StructEntry *source, Allocator allocator) {
    this->type = Type_copy(source->type, allocator);
    String_create(&this->name, allocator);
    String_copy(&this->name, &source->name);
}

typedef struct {
    Size ntypes;
} StructTypeHeader;

typedef struct {
    StructTypeHeader header;
    Type types[1] /* types[ntypes] */;
} StructType;

#define STRUCT_TYPE_SIZE(ntypes) (sizeof(StructTypeHeader) + sizeof(Type) * ntypes)

void StructType_new() {}