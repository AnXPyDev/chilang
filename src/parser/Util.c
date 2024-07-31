typedef int ParserChar;
#define PARSER_EOF EOF

bool ParserUtil_isNewline(ParserChar c) {
    switch (c) {
        case 10: // lf
        case 13: // cr
            return true;
    }

    return false;
}

bool ParserUtil_isWhitespace(ParserChar c) {
    switch (c) {
        case 9: // tab
        case 32: // space
            return true;
    }

    return false;
}

bool ParserUtil_isTokenChar(ParserChar c) {
    static const int inclusiveChars[] = { '~', '!', '@', '#', '$', '%', '^', '&', '*', '_', '+', '-', '=', '/', '<', '>', '?' };

    for (Size i = 0; i < ARRSIZE(inclusiveChars); i++) {
        if (c == inclusiveChars[i]) {
            return true;
        }
    }

    if (
        ('a' <= c && c <= 'z') ||
        ('A' <= c && c <= 'Z') ||
        ('0' <= c && c <= '9')
    ) {
        return true;
    }

    return false;
}

bool ParserUtil_isTokenBeginChar(ParserChar c) {
    if (!ParserUtil_isTokenChar(c)) {
        return false;
    }

    if (
        ('0' <= c && c <= '9')
    ) {
        return false;
    }

    return true;
}

bool ParserUtil_isExpressionBeginChar(ParserChar c) {
    return !ParserUtil_isWhitespace(c);
}

bool ParserUtil_isExpressionEndChar(ParserChar c) {
    switch (c) {
        case ';':
            return true;
    }

    return false;
}