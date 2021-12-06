typedef struct _Stack {
    Expression *data;
    struct _Stack *next;
} Stack;

ExpressHeadTail *infix_to_postfix(ExpressHeadTail *expht);