#include <stdbool.h>
#include <stdlib.h>

#include "number.h"
#include "stack.h"
#include "../error.h"

bool is_empty(Stack *target) {
    if (target->next == NULL) {
        return true;
    } else {
        return false;
    }
}

Stack *init_stack() { 
    Stack *s = malloc(sizeof(Stack));
    s->data = NULL;
    s->next = NULL;
    return s;
}

void push(Stack *target, Expression *data) {
    Stack *newNode = malloc(sizeof(Stack));
    newNode->data = data;
    newNode->next = target->next;
    target->next = newNode;
}

Expression* pop(Stack *target) {
    if (is_empty(target)) {
        return NULL;   
    } else {
        Stack *poped = target->next;
        target->next = poped->next;
        poped->next = NULL;
        return poped->data;
    }
}

char peek(Stack *target) {
    return target->next->data->opr;
}

int prec(char op) {
    switch (op) { 
        case '(': case ')':
            return 0;
        case '+': case '-':
            return 1;
        case '*': case '/':
            return 2; 
        default:
            return -1;
    }
}

/*
식에 오류가 있을때 오류 출력과 추가하던 구조체를 모두 삭제하는 함수 

ExpressHeadTail *eht: 메모리 해제를 진행할 노드 주소값 (인자값으로 받아온 ExpressHeadTail)
ExpressHeadTail *eht2: 메모리 해제를 진행할 노드 주소값 (함수 안에서 새로 생성한 ExpressHeadTail)
ERROR_TYPE type: 에러 종류
*/
ExpressHeadTail* print_error_stack(ExpressHeadTail *eht, ExpressHeadTail *eht2, ERROR_TYPE type) {
    alert_error(type);
    release_all(eht);
    release_all(eht2);
    return NULL;
}

ExpressHeadTail *infix_to_postfix(ExpressHeadTail *expht) {
    Stack *stack = init_stack();
    Expression *tail = expht->tail;
    Expression *now = expht->head->next;
    ExpressHeadTail *result = init_expression();

    while (now != tail) {
        now->prev->next = now->next;
        now->next->prev = now->prev;
        now->next = NULL;
        now->prev = NULL;

        char ch = now->opr;
        if (now->type == TYPE_DIGIT) {
            // Expression 타입이 숫자일 경우
            expression_insert_tail(now, result->tail);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            while (!is_empty(stack) && (prec(ch) <= prec(peek(stack)))) {
                expression_insert_tail(pop(stack), result->tail);
            }
            // 자기자신을 스택에 추가한다. 
            push(stack, now); 
        } else if (ch == '(') {
            push(stack, now);
        } else if (ch == ')') {
            Expression *poped = pop(stack);
            if (poped == NULL) {
                return print_error_stack(expht, result, ERROR_RIGHT_BRACKET_FRIST);
            }
            while (poped->opr != '(') {
                if (poped == NULL) {
                    return print_error_stack(expht, result, ERROR_RIGHT_BRACKET_FRIST);
                }
                expression_insert_tail(poped, result->tail);
                poped = pop(stack);
            }
        } 
        now  = expht->head->next;
    }

    while (!is_empty(stack)) { 
        expression_insert_tail(pop(stack), result->tail);
    } 

    free(expht);
    return result;
}