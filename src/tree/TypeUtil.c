bool Type_isSignedInteger(Type this) {
    if (!Type_isPrimitive(this)) {
        return false;
    }

    return PrimitiveType_isSignedInteger(*(EPrimitiveType*)this.object);
}

bool Type_isUnsignedInteger(Type this) {
    if (!Type_isPrimitive(this)) {
        return false;
    }

    return PrimitiveType_isUnsignedInteger(*(EPrimitiveType*)this.object);
}

bool Type_isInteger(Type this) {
    return Type_isSignedInteger(this) || Type_isUnsignedInteger(this);
}