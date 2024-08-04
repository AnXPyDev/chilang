#define ADD_PTYPE(token, type) Scope_add_type(scope, strview(token), PrimitiveType_upcast(type))
#define ADD_KWORD(token, kword) Scope_add_keyword(scope, strview(token), kword)

int GlobalScope_init(Scope *scope) {
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

    ADD_KWORD("=", KEYWORD_ASSIGN);
    ADD_KWORD("print", KEYWORD_PRINT);

    return 0;
}

#undef ADD_PTYPE
#undef ADD_KWORD