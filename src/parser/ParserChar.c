typedef int ParserChar;
#define PARSER_EOF EOF

bool ParserChar_isNewline(ParserChar c) {
    switch (c) {
        case 10: // lf
        case 13: // cr
            return true;
    }

    return false;
}

bool ParserChar_isWhitespace(ParserChar c) {
    switch (c) {
        case 9: // tab
        case 32: // space
            return true;
    }

    return ParserChar_isNewline(c);
}

bool ParserChar_isNumericLiteralBegin(ParserChar c) {
    if ('0' <= c && c <= '9') {
        return true;
    }
    return false;
}

bool ParserChar_isNumericLiteral(ParserChar c) {
    return ParserChar_isNumericLiteralBegin(c);
}

bool ParserChar_isLiteralBegin(ParserChar c) {
    return ParserChar_isNumericLiteralBegin(c);
}

bool ParserChar_isToken(ParserChar c) {
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

bool ParserChar_isTokenBegin(ParserChar c) {
    return ParserChar_isToken(c) && !ParserChar_isLiteralBegin(c);
}

bool ParserChar_isExpressionBegin(ParserChar c) {
    return !ParserChar_isWhitespace(c);
}

bool ParserChar_isBlockBegin(ParserChar c) {
    return c == '{';
}

bool ParserChar_isBlockEnd(ParserChar c) {
    return c == '}';
}

bool ParserChar_isExpressionEndInclusive(ParserChar c) {
    switch (c) {
        case ';':
        case ')':
            return true;
    }

    return false;
}

bool ParserChar_isExpressionEndExclusive(ParserChar c) {
    switch (c) {
        case '}': // block end
            return true;
    }
    return false;
}

bool ParserChar_isListDelimiterChar(ParserChar c) {
    switch (c) {
        case ',':
            return true;
    }

    return false;
}
