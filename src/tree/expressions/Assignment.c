typedef struct {
    StringView token;
    Expression expression;
} AssignmentExpression;

Expression AssignmentExpression_upcast(AssignmentExpression *this);

#define this ((AssignmentExpression*)vthis)

#undef this

const IExpression IAssignmentExpression = {
};