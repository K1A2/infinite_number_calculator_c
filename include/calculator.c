#include <stdbool.h>
#include <stdlib.h>

#include "express/number.h"
#include "express/stack.h"
#include "error.h"
#include "calculator.h"

Expression *addition(Expression *a, Expression *b) {

}

Expression *subtraction(Expression *a, Expression *b) {

}

Expression *multiplication(Expression *a, Expression *b) {

}

Number *print_error_calculation(ExpressHeadTail *expht, Stack *stack, Expression *one, Expression *two, Expression *result, ERROR_TYPE error) {
    alert_error(error);
    delete_stack(stack);
    if (one != NULL) {
        free(one);
    }
    if (two != NULL) {
        free(two);
    }
    if (result != NULL) {
        free(result);
    }
    free(expht);
    return NULL;
}

Number *calculation(ExpressHeadTail *expht) {
    Stack *stack = init_stack();
    Expression *now = expht->head->next;
    Expression *tail = expht->tail;

    while (now != tail) {
        now->prev->next = now->next;
        now->next->prev = now->prev;
        now->next = NULL;
        now->prev = NULL;

        if (now->type == TYPE_DIGIT) {
            push(stack, now);
        } else {
            Expression *one = pop(stack), *two = pop(stack), *result = NULL;
            char op = now->opr;

            if (one == NULL || two == NULL) {
                return print_error_calculation(expht, stack, one, two, result, ERROR_CALCULATION_ERROR);
            }

            switch (op) {
                case '+':
                    result = addition(one, two);
                    break;
                case '-':
                    result = subtraction(one, two);
                    break;
                case '*':
                    result = multiplication(one, two);
                    break;
            }
            if (result == NULL) {
                return print_error_calculation(expht, stack, one, two, result, ERROR_CALCULATION_ERROR);
            } else {
                push(stack, result);
            }
        }
        free(now);
        now = expht->head->next;
    }

    return pop(stack)->data;
}