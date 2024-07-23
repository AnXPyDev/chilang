void format(OutStream os, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    bool escaped = false;

    while (1) {
        int c = *fmt;
        switch (c) {
            case '\0':
                goto end;
            case '\\':
                if (escaped) {
                    goto put;
                }
                escaped = true;
                goto next;
            case '{':
                if (escaped) {
                    goto put;
                }
                goto read_fmt_spec;

        }

        put:;
        escaped = false;
        OutStream_putc(os, c);
        goto next;

        read_fmt_spec:;
        
        fmt++;
        char spec[65];
        spec[0] = '%';
        char *sp = spec + 1;

        while (1) {
            int c = *fmt;
            switch (c) {
                case '\0':
                    goto end;
                case '}':
                    goto process_fmt_spec;
            }

            *sp = c;
            sp++;
            fmt++;
        }

        process_fmt_spec:;

        if (sp == spec + 1) {
            goto process_printable;
        }

        if (strcmp(spec + 1, "sw") == 0) {
            goto process_stringview;
        }

        char buf[256];
        vsnprintf(buf, 256, spec, args);
        OutStream_puts(os, buf);
        goto next;

        process_printable:;
        Printable p = va_arg(args, Printable);
        p.print(p.object, os);
        goto next;

        process_stringview:;
        StringView sw = va_arg(args, StringView);
        OutStream_write(os, sw);
        goto next;

        next:;
        fmt++;
        continue;

        end:;
        break;
    }

    va_end(args);
}