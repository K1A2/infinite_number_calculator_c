typedef struct _Stack {
    Expression *data;
    struct _Stack *next;
} Stack;

void push(Stack *target, Expression *data);
Expression* pop(Stack *target);
Expression* peek(Stack *target);
ExpressHeadTail *infix_to_postfix(ExpressHeadTail *expht);