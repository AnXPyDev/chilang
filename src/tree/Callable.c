typedef struct {
    CallableType type;
    MemberList mlArgs;
    Expression expression;
} Callable;

void Callable_repr(Callable *this, OutStream os) {
    char buf[32];
    sprintf(buf, "callable[%p]", this);
    OutStream_puts(os, buf)
}