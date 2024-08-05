typedef struct {
    Type result;
    StringBuffer token;
} GetExpression;

#define this ((GetExpression*)vthis)

#undef this
