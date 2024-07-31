//#define LIB_STRING_ZERO_COMPAT

#ifdef LIB_STRING_ZERO_COMPAT
#define STRING_ZERO false
#define STRING_PAD 0
#else
#define STRING_ZERO true
#define STRING_PAD 1
#endif

typedef struct {
    Allocator allocator;
    Size length;
    Size capacity;
    char *data;
} String;

StringView String_view(const String *this) {
    return (StringView) {
        .data = this->data,
        .size = this->length
    };
}

const String String_NULL = {
    .allocator = Allocator_NULL,
    .length = 0,
    .capacity = 0,
    .data = NULL
};


#define String_begin(this) ((this)->data)
#define String_end(this) ((this)->data + (this)->length)

bool String_isNull(String *this) {
    return this->data == NULL;
}

void String_create(String *this, Allocator allocator) {
    this->allocator = allocator;
    this->length = 0;
    this->capacity = 0;
    this->data = NULL;
}

String String_new(Allocator allocator) {
    String this;
    String_create(&this, allocator);
    return this;
}

void String_destroy(String *this) {
    this->capacity = 0;
    this->length = 0;
    Allocator_free(this->allocator, this->data);
    this->data = NULL;
}

void String_zero(String *this) {
    if (!STRING_ZERO) {
        return;
    }

    this->data[this->length] = 0;
}

void String_blank(String *this, Size capacity) {
    this->data = Allocator_malloc(this->allocator, capacity + STRING_PAD);
    this->capacity = capacity;
    this->length = 0;
    String_zero(this);
}

void String_copy(String *this, StringView other) {
    this->data = Allocator_malloc(this->allocator, other.size + STRING_PAD);
    this->length = other.size;
    this->capacity = other.size;
    memcpy(this->data, other.data, other.size);
    String_zero(this);
}


void String_append(String *this, StringView other) {
    Size newlen = other.size + this->length;
    if (newlen > this->capacity) {
        this->data = Allocator_realloc(this->allocator, this->data, newlen + STRING_PAD);
        this->capacity = newlen;
    }
    memcpy(String_end(this), other.data, other.size);
    this->length = newlen;
    String_zero(this);
}

void String_print(const String *this, OutStream outstream) {
    OutStream_puts(outstream, this->data);
}

#undef STRING_PAD
#undef STRING_ZERO