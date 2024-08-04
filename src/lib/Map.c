#ifndef LIB_MAP_BUCKETS
#define LIB_MAP_BUCKETS 16
#endif

#define MAP_BUCKETS LIB_MAP_BUCKETS

#define ENTRY_ITEM(entry) ((char*)entry + sizeof(struct Map_Entry))
#define ENTRY_KEY(entry, item_size) (ENTRY_ITEM(entry) + item_size)

typedef unsigned int Map_Hash;
typedef unsigned int Map_EntryIndex;

typedef struct Map_Entry {
    struct Map_Entry *next;
    Map_EntryIndex index;
    Map_Hash hash;
    Size key_size;
    // data[Map.item_size]; data[key_size]
} Map_Entry;

typedef struct {
    Allocator allocator;
    Size item_size;
    Map_EntryIndex counter;
    Size nitems;
    struct Map_Entry *buckets[MAP_BUCKETS];
} Map;

typedef BufferView Map_Key;

void Map_create(Map *this, Allocator allocator, Size item_size) {
    this->allocator = allocator;
    this->item_size = item_size;
    this->counter = 0;
    this->nitems = 0;
    for (Size i = 0; i < MAP_BUCKETS; i++) {
        this->buckets[i] = NULL;
    }
}

Map Map_new(Allocator allocator, Size item_size) {
    Map this;
    Map_create(&this, allocator, item_size);
    return this;
}

struct Map_Entry *Map_createEntry(Map *this, Map_Hash hash, Map_Key key) {
    struct Map_Entry *entry = Allocator_calloc(this->allocator, sizeof(struct Map_Entry) + key.size + this->item_size);
    entry->key_size = key.size;
    entry->hash = hash;
    entry->index = this->counter;
    this->counter += 1;
    memcpy(ENTRY_KEY(entry, this->item_size), key.data, key.size);
    return entry;
}

void Map_destroyEntry(Map *this, struct Map_Entry *entry) {
    Allocator_free(this->allocator, entry);
}

Map_Hash Map_hash(Map_Key key) {
    Size size = key.size;
    const char *data = key.data;

    Map_Hash hash = 5381;
	while ( size != 0 ) {
        hash = ((hash << 5) + hash) + *data;
        data++;
        size--;
	}
    return hash;
}

#define MAP_BUCKET(hash) (hash % MAP_BUCKETS)

struct Map_Entry *Map_pushKey(Map *this, Map_Hash hash, Map_Key key) {
    struct Map_Entry *entry = Map_createEntry(this, hash, key);
    struct Map_Entry *next_entry = this->buckets[MAP_BUCKET(hash)];
    entry->next = next_entry;
    this->buckets[MAP_BUCKET(hash)] = entry;
    this->nitems++;
    return entry;
}

void Map_removeEntry(Map *this, struct Map_Entry **entryp) {
    struct Map_Entry *next = (*entryp)->next;
    Map_destroyEntry(this, *entryp);
    *entryp = next;
    this->nitems--;
}

struct Map_Entry *Map_findEntry(Map *this, Map_Hash hash, Map_Key key) {
    struct Map_Entry *entry = this->buckets[MAP_BUCKET(hash)];
    while (1) {
        if (entry == NULL) { return NULL; }
        if (entry->hash == hash && entry->key_size == key.size && memcmp(ENTRY_KEY(entry, this->item_size), key.data, key.size) == 0) {
            return entry;
        }
        entry = entry->next;
    }
}

struct Map_Entry **Map_findEntryP(Map *this, Map_Hash hash, Map_Key key) {
    struct Map_Entry **entryp = this->buckets + MAP_BUCKET(hash);
    while (1) {
        struct Map_Entry *entry = *entryp;
        if (entry == NULL) { return NULL; }
        if (entry->hash == hash && entry->key_size == key.size && memcmp(ENTRY_KEY(entry, this->item_size), key.data, key.size) == 0) {
            return entryp;
        }
        entryp = &entry->next;
    }
}

void Map_destroyBucket(Map *this, struct Map_Entry *entry) {
    struct Map_Entry *next;
    while (1) {
        if (entry == NULL) { return; }
        next = entry->next;
        Map_destroyEntry(this, entry);
        entry = next;
    }
}

void *Map_add(Map *this, Map_Key key) {
    Map_Hash hash = Map_hash(key);
    struct Map_Entry *entry = Map_pushKey(this, hash, key);
    return ENTRY_ITEM(entry);
}

void *Map_ensure(Map *this, Map_Key key) {
    Map_Hash hash = Map_hash(key);
    struct Map_Entry *entry = Map_findEntry(this, hash, key);
    if (entry == NULL) {
        entry = Map_pushKey(this, hash, key);
    }
    return ENTRY_ITEM(entry);
}

void *Map_get(Map *this, Map_Key key) {
    Map_Hash hash = Map_hash(key);
    struct Map_Entry *entry = Map_findEntry(this, hash, key);
    if (entry == NULL) {
        return NULL;
    }
    return ENTRY_ITEM(entry);
}

void Map_delete(Map *this, Map_Key key) {
    Map_Hash hash = Map_hash(key);
    struct Map_Entry **entryp = Map_findEntryP(this, hash, key);
    if (entryp == NULL) {
        return;
    }
    Map_removeEntry(this, entryp);
}

void Map_destroy(Map *this) {
    for (Size i = 0; i < MAP_BUCKETS; i++) {
        Map_destroyBucket(this, this->buckets[i]);
        this->buckets[i] = NULL;
    }
}

void *Map_foreach(Map *this, void *(*loop)(void *item, void *payload), void *payload) {
    for (Size i = 0; i < MAP_BUCKETS; i++) {
        struct Map_Entry *entry = this->buckets[i];
        while (entry) {
            void *r = loop(ENTRY_ITEM(entry), payload);
            if (r) {
                return r;
            }
            entry = entry->next;
        }
    }
    return NULL;
}

typedef void *(*Map_kvloop_fn)(BufferView key, void *item, void *payload);

#define CALL_KVLOOP(entry) loop((BufferView) { .data = ENTRY_KEY(entry, this->item_size), .size = entry->key_size }, ENTRY_ITEM(entry), payload)

void *Map_foreach_kv(Map *this, Map_kvloop_fn loop, void *payload) {
    for (Size i = 0; i < MAP_BUCKETS; i++) {
        struct Map_Entry *entry = this->buckets[i];
        while (entry) {
            void *r = CALL_KVLOOP(entry);
            if (r) {
                return r;
            }
            entry = entry->next;
        }
    }
    return NULL;
}

void *Map_foreach_stable_kv(Map *this, Map_kvloop_fn loop, void *payload) {
    Map_Entry *buckets[MAP_BUCKETS];
    for (Size i = 0; i < MAP_BUCKETS; i++) {
        buckets[i] = this->buckets[i];
    }

    Map_Entry **sorted_entries = (Map_Entry**)malloc(sizeof(Map_Entry*) * this->nitems);
    Map_Entry **ep = sorted_entries;

    while (1) {
        Map_Entry **min = NULL;
        for (Size i = 0; i < MAP_BUCKETS; i++) {
            if (buckets[i] != NULL && (min == NULL || buckets[i]->index > (*min)->index)) {
                min = &buckets[i];
            }
        }

        if (min == NULL) {
            break;
        }

        *ep = *min;
        ep++;
        *min = (*min)->next;
    }

    void *result = NULL;

    for (Size i = this->nitems; i > 0; i--) {
        result = CALL_KVLOOP(sorted_entries[i - 1]);
        if (result) {
            goto end;
        }
        
    }

    end:;
    free(sorted_entries);
    return result;
}

#undef CALL_KVLOOP
#undef ENTRY_ITEM
#undef ENTRY_KEY
#undef MAP_BUCKET
#undef MAP_BUCKETS