#include <lib/include.h>
#include <tree/include.h>

void printInfo(OutStream os, TypeInfo info) {
    char buf[256];
    snprintf(buf, 256, "{ valid = %u, meta = %u, size_known = %u, size = %llu }",
        info.valid,
        info.meta,
        info.size_known,
        info.size
    );
    OutStream_puts(os, buf);
}

void printType(OutStream os, Type type) {
    TypeInfo ti = Type_info(type);
    OutStream_puts(os, "   ");
    Type_repr(type, os);
    OutStream_puts(os, ": ");
    printInfo(os, ti);
}

int main(int argc, char **argv) {
    std_streams_init();

    OutStream_puts(os_stderr, "Primitive types:\n");

    for (EPrimitiveType t = 0; t < TYPE__END; t++) {
        Type tt = PrimitiveType_Type(t);
        printType(os_stderr, tt);
        OutStream_putc(os_stderr, '\n');
    }

    OutStream_puts(os_stderr, "\nPointer types:\n");

    Type i32ptr = PointerType_wrap(PrimitiveType_Type(TYPE_I32), standardAllocator);

    printType(os_stderr, i32ptr);
    OutStream_putc(os_stderr, '\n');

    
    OutStream_puts(os_stderr, "\nQualifier types:\n");

    Type i32const = QualifierType_wrap_copy(TypeQualifiers_make(TYPEQUALIFIER_CONST), i32ptr, standardAllocator);

    printType(os_stderr, i32const);
    OutStream_putc(os_stderr, '\n');

    Type_destroy(i32const, standardAllocator);
    Type_destroy(i32ptr, standardAllocator);
    
    

    return 0;
}