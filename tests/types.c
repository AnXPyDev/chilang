#include <lib/include.h>
#include <tree/include.h>

int main(int argc, char **argv) {
    std_streams_init();

    char buf[256];

    OutStream_puts(os_stderr, "Primitive types:\n");

    for (EPrimitiveType t = 0; t < TYPE__END; t++) {
        Type tt = PrimitiveType_Type(t);
        TypeInfo ti = Type_info(tt);
        OutStream_puts(os_stderr, "   ");
        Type_repr(tt, os_stderr);
        OutStream_puts(os_stderr, ": ");
        snprintf(buf, 256, "{ valid = %u, meta = %u, size_known = %u, size = %llu }",
            ti.valid,
            ti.meta,
            ti.size_known,
            ti.size
        );
        OutStream_puts(os_stderr, buf);
        OutStream_putc(os_stderr, '\n');
    }

    return 0;
}