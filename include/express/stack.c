#include <stdbool.h>
#include <stdlib.h>

#include "number.h"
#include "stack.h"

Stack *init_stack() { 
    Stack *s = malloc(sizeof(Stack));
    s->data = 0x0;
    s->next = 0x0;
    return s;
}

void push(Stack *target, Expression *data) {

}

Expression* pop(Stack *target) {

}

Expression* peek(Stack *target) {

}

ExpressHeadTail *infix_to_postfix(ExpressHeadTail *expht) {
    Stack *stack = init_stack();
}