#define ELEMENT(index) ((void*)(this->data + this->item_size * (index)))

typedef struct {
    Allocator allocator;
    Size item_size;
    Size size;
    Size capacity;
    char *data;
} Vector;

void Vector_create(Vector *this, Allocator allocator, Size item_size) {
    this->allocator = allocator;
    this->item_size = item_size;
    this->size = 0;
    this->capacity = 0;
    this->data = NULL;
}

Vector Vector_new(Allocator allocator, Size item_size) {
    Vector this;
    Vector_create(&this, allocator, item_size);
    return this;
}

void Vector_init(Vector *this, Size capacity) {
    this->capacity = capacity;
    this->size = 0;
    this->data = Allocator_malloc(this->allocator, this->item_size * this->capacity);
}

void Vector_copy(Vector *this, ArrayView other) {
    this->data = Allocator_malloc(this->allocator, other.size * this->item_size);
    memcpy(this->data, other.data, other.size * this->item_size);
    this->capacity = other.size;
    this->size = other.size;
}

void Vector_nullify(Vector *this) {
    this->data = NULL;
    this->size = 0;
    this->capacity = 0;
}

bool Vector_isNull(Vector *this) {
    return this->data == NULL;
}

void Vector_destroy(Vector *this) {
    Allocator_free(this->allocator, this->data);
    Vector_nullify(this);
}

void Vector_resize(Vector *this, Size capacity) {
    this->data = Allocator_realloc(this->allocator, this->data, this->item_size * capacity);
    this->capacity = capacity;
    if (this->size > this->capacity) {
        this->size = this->capacity;
    }
}

void Vector_expand(Vector *this) {
    this->capacity = (this->capacity + 1) * 2;
    this->data = Allocator_realloc(this->allocator, this->data, this->item_size * this->capacity);
}

void Vector_expand_to(Vector *this, Size capacity) {
    if (capacity < this->capacity) {
        Vector_resize(this, capacity);
        return;
    }
    Size newCapacity = this->capacity;
    while (newCapacity < capacity) {
        newCapacity = (newCapacity + 1) * 2;
    }
    this->capacity = newCapacity;
    this->data = Allocator_realloc(this->allocator, this->data, this->item_size * this->capacity);
}

void *Vector_push(Vector *this) {
    if (this->size + 1 > this->capacity) {
        Vector_expand(this);
    }
    this->size++;
    return ELEMENT(this->size - 1);
}

void *Vector_push_unsafe(Vector *this) {
    this->size++;
    return ELEMENT(this->size - 1);
}

void *Vector_pop(Vector *this) {
    if (this->size > 0) {
        this->size--;
        return ELEMENT(this->size);
    }

    return NULL;
}

void *Vector_get(Vector *this, Size index) {
    if (index >= this->size) {
        return NULL;
    }

    return ELEMENT(index);
}

const void *Vector_cget(const Vector *this, Size index) {
    if (index >= this->size) {
        return NULL;
    }

    return ELEMENT(index);
}

#define Vector_begin(this) ((void*)((this)->data))
#define Vector_end(this) ((void*)((this)->data + (this)->item_size * (this)->size))

void Vector_join(Vector *this, ArrayView other) {
    if (this->capacity < this->size + other.size) {
        Vector_resize(this, this->size + other.size);
    }
    memcpy(Vector_end(this), other.data, this->item_size * other.size);
    this->size += other.size;
}

void Vector_append(Vector *this, ArrayView other) {
    if (this->capacity < this->size + other.size) {
        Vector_expand_to(this, this->size + other.size);
    }
    memcpy(Vector_end(this), other.data, this->item_size * other.size);
    this->size += other.size;
}

void Vector_remove(Vector *this, Size index) {
    if (this->size <= index) {
        return;
    }

    if (index == this->size - 1) {
        this->size--;
        return;
    }

    memmove(ELEMENT(index), ELEMENT(index + 1), this->item_size * (this->size - index - 1));
    this->size--;
}

void Vector_clear(Vector *this) {
    this->size = 0;
}


Array Vector_array(Vector *this) {
    return (Array) {
        .data = this->data,
        .size = this->size
    };
}

ArrayView Vector_view(const Vector *this) {
    return (ArrayView) {
        .data = this->data,
        .size = this->size
    };
}

#undef ELEMENT