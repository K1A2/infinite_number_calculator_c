#include <stdbool.h>
#include <stdlib.h>

#include "number.h"
#include "stack.h"
#include "../error.h"

/*
스택이 비었는지 확인하는 함수

Stack *target: 비었는지 확인 할 스택
*/
bool is_empty(Stack *target) {
    if (target->next == NULL) {
        return true;
    } else {
        return false;
    }
}

/*
스택을 초기화 하는 함수
*/
Stack *init_stack() { 
    Stack *s = malloc(sizeof(Stack));
    s->data = NULL;
    s->next = NULL;
    return s;
}

/*
스택에서 맨 위에 값을 추가하는 함수

Stack *target: 값을 추가할 스택
Expression *data: 추가 될 값
*/
void push(Stack *target, Expression *data) {
    Stack *newNode = malloc(sizeof(Stack));
    newNode->data = data;
    newNode->next = target->next;
    target->next = newNode;
}

/*
스택에서 맨 위에 값을 가져오고 스택에서 지우는 함수

Stack *target: 값을 가져올 스택
*/
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

/*
스택에서 맨 위에 값을 확인하는 함수

Stack *target: 값을 확인할 스택
*/
char peek(Stack *target) {
    return target->next->data->opr;
}

/*
연산자의 우선순위를 반환하는 함수

char op: 우선순위를 확인할 연산자
*/
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

/*
중위 연산식을 후위 연산식으로 바꾸는 함수

ExpressHeadTail *expht: 후위 연산식으로 변경할 표현식
*/
ExpressHeadTail *infix_to_postfix(ExpressHeadTail *expht) {
    Stack *stack = init_stack(); // 스택 초기화
    Expression *tail = expht->tail; // Expression 링크드 리스트의 마지막 노드
    Expression *now = expht->head->next; // Expression 링크드 리스트의 두 번째 노드. 첫 번째 노드는 head노드.
    ExpressHeadTail *result = init_expression(); // 후위 연산식을 저장할 ExpressHeadTail 구조체

    while (now != tail) { // now가 tail일 때 까지 반복
        // Expression 링크드 리스트에서 now를 삭제하고 가져오는 부분
        now->prev->next = now->next;
        now->next->prev = now->prev;
        now->next = NULL;
        now->prev = NULL;

        char ch = now->opr; // now의 연산자를 가져옴
        if (now->type == TYPE_DIGIT) {
            // Expression 타입이 숫자일 경우
            expression_insert_tail(now, result->tail); // 후위 연산식 링크드 리스트에 바로 추가
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            // Expression 타입이 연산자일 경우
            while (!is_empty(stack) && (prec(ch) <= prec(peek(stack)))) {
                // 현재 연산자와 스택에 저장된 연산자의 우선순위를 비교
                // 현재 연산자보다 스택 최상위에 있는 연산자의 우선순위가 더 높을 경우 pop 후 후위 연산식 링크드 리스트에 추가
                // 스택이 비었다면 중단
                expression_insert_tail(pop(stack), result->tail);
            }
            // 자기자신을 스택에 추가한다. 
            push(stack, now); 
        } else if (ch == '(') {
            // ( 은 무조건 후위 연산식 링크드 리스트에 추가
            push(stack, now);
        } else if (ch == ')') {
            Expression *poped = pop(stack);
            if (poped == NULL) {
                // stack 안에 아무것도 없을 경우 ( 보다 ) 가 먼저 나왔으므로 오류 처리
                return print_error_stack(expht, result, ERROR_RIGHT_BRACKET_FRIST);
            }
            while (poped->opr != '(') { // ( 이 나올 때 까지 반복
                if (poped == NULL) {
                    // stack 안에 아무것도 없을 경우 ( 보다 ) 가 먼저 나왔으므로 오류 처리
                    return print_error_stack(expht, result, ERROR_RIGHT_BRACKET_FRIST);
                }
                expression_insert_tail(poped, result->tail); // ( 이 나올 때 까지 pop 후 후위 연산식 링크드 리스트에 추가
                poped = pop(stack);
            }
        }
        now  = expht->head->next; // now를 head의 다음으로 지정
    }

    while (!is_empty(stack)) {
        // 추가되지 않고 스택에 남은 연산자 모두 pop 후 후위 연산식 링크드 리스트에 추가
        expression_insert_tail(pop(stack), result->tail);
    }

    free(expht); // 중위 연산식 ExpressHeadTail는 메모리 해제
    return result; // 후위 연산식 반환
}