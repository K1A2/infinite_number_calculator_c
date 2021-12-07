typedef struct _Stack {
    Expression *data;
    struct _Stack *next;
} Stack;

ExpressHeadTail *infix_to_postfix(ExpressHeadTail *expht);

Stack *init_stack();
void push(Stack *target, Expression *data);
Expression* pop(Stack *target);
void delete_stack(Stack *head);