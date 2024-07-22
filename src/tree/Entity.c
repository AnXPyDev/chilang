typedef struct {
    Type type;
    void *object;
} Entity;

const Entity Entity_NULL = {
    .type = Type_NULL,
    .object = NULL
};

void Entity_destroy(Entity *this, Allocator allocator) {
   Type_destroy(this->type, allocator);
   // TODO: destroy object 
}