void Scope_add_type(Scope *scope, StringView token, Type type) {
    Entity *ent = Scope_add_entity(scope, token);
    ent->mutable = false;
    TypeEntity_create(ent, type, scope->allocator);
}

#define ADD_PTYPE(token, type) Scope_add_type(scope, strview(token), PrimitiveType_Type(type))

void GlobalScope_init(Scope *scope) {
    ADD_PTYPE("namespace", TYPE_NAMESPACE);
    ADD_PTYPE("alias", TYPE_ALIAS);
    ADD_PTYPE("interface", TYPE_INTERFACE);
    ADD_PTYPE("template", TYPE_TEMPLATE);
    ADD_PTYPE("struct", TYPE_STRUCT);
    ADD_PTYPE("union", TYPE_UNION);

    ADD_PTYPE("type", TYPE_TYPE);
    ADD_PTYPE("auto", TYPE_INFER);
    ADD_PTYPE("any", TYPE_ANY);
    
    ADD_PTYPE("void", TYPE_VOID);
    ADD_PTYPE("bool", TYPE_BOOL);
    ADD_PTYPE("u8", TYPE_U8);
    ADD_PTYPE("u32", TYPE_U32);
    ADD_PTYPE("i8", TYPE_I8);
    ADD_PTYPE("i32", TYPE_I32);
}

#undef ADD_PTYPE