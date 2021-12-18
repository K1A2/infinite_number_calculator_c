#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "express/number.h"
#include "express/stack.h"
#include "error.h"
#include "calculator.h"

/*
두 수의 소수점 아래 자리수의 개수를 같게 만드는 함수

Number **a: 소수점 개수를 비교할 숫자 1
Number **b: 소수점 개수를 비교할 숫자 2
Number **target: 계산 결과를 저장할 Number 구조체. a와 b의 아래 자리수의 개수를 가져와 같게 설정하기 위해 필요
*/
void make_down_place_count_same(Number **a, Number **b, Number **target) {
    if ((*a)->deciaml_point >= (*b)->deciaml_point) {
        for (int i = 0;i < (*a)->deciaml_point - (*b)->deciaml_point;i++) {
            digit_insert_tail('0', (*b)->tail);
        }
        (*b)->deciaml_point = (*a)->deciaml_point;
    } else {
        for (int i = 0;i < (*b)->deciaml_point - (*a)->deciaml_point;i++) {
            digit_insert_tail('0', (*a)->tail);
        }
        (*a)->deciaml_point = (*b)->deciaml_point;
    }
    (*target)->deciaml_point = (*a)->deciaml_point;
}

/*
두 수의 소수점 윗 자리수의 개수를 같게 만드는 함수

Number **a: 소수점 윗 자리수 개수를 비교할 숫자 1
Number **b: 소수점 윗 자리수 개수를 비교할 숫자 2
*/
void make_up_place_count_same(Number **a, Number **b) {
    unsigned int a_total_count = get_count_digits((*a)->head, (*a)->tail), b_total_count = get_count_digits((*b)->head, (*b)->tail);
    if (a_total_count >= b_total_count) {
        for (int i = 0;i < a_total_count - b_total_count;i++) {
            digit_insert_head('0', (*b)->head);
        }
    } else {
        for (int i = 0;i < b_total_count - a_total_count;i++) {
            digit_insert_head('0', (*a)->head);
        }
    }
}

/*
더하기 연산을 수행하는 함수. a + b

Number **a: 더하기 연산을 수행할 숫자 (왼쪽)
Number **b: 더하기 연산을 수행할 숫자 (오른쪽)
*/
Expression *addition(Expression **a, Expression **b) {
    if ((*a)->data->isPositive != (*b)->data->isPositive) {
        if (!(*b)->data->isPositive) {
            (*b)->data->isPositive = true;
        } else {
            (*a)->data->isPositive = true;
            Expression *tmp = *a;
            *a = *b;
            *b = tmp;
        }
        return subtraction(a, b);
    } else {
        Expression *result = init_expression_empty_noode_number();

        make_down_place_count_same(&((*a)->data), &((*b)->data), &(result->data));
        make_up_place_count_same(&((*a)->data), &((*b)->data));

        Digits *now_a = (*a)->data->tail->prev;
        Digits *now_b = (*b)->data->tail->prev;
        unsigned int carry = 0;
        while (now_a != (*a)->data->head) {
            unsigned int num_res = (now_a->data - '0') + (now_b->data - '0');
            num_res += carry;
            carry = num_res / 10;
            num_res %= 10;

            digit_insert_head(num_res + '0', result->data->head);

            now_a = now_a->prev;
            now_b = now_b->prev;
        }

        if (carry != 0) {
            digit_insert_head(carry + '0', result->data->head);
        }

        if (!(*a)->data->isPositive) {
            result->data->isPositive = false;
        }

        release_numbers(&((*a)->data));
        release_numbers(&((*b)->data));
        free(*a);
        free(*b);

        return result;
    }
}

/*
빼기 연산을 수행하는 함수. a - b

Number **a: 빼기 연산을 수행할 숫자 (왼쪽)
Number **b: 빼기 연산을 수행할 숫자 (오른쪽)
*/
Expression *subtraction(Expression**a, Expression **b) {
    if ((*a)->data->isPositive && !(*b)->data->isPositive) { // a = +, b = -
        (*b)->data->isPositive = true;
        return addition(a, b);
    } else if (!(*a)->data->isPositive && (*b)->data->isPositive) { // a = -, b = +
        (*b)->data->isPositive = false;
        return addition(a, b);
    }
    Expression *result = init_expression_empty_noode_number();

    make_down_place_count_same(&((*a)->data), &((*b)->data), &(result->data));

    bool is_swap = false;

    unsigned int a_total_count = get_count_digits((*a)->data->head, (*a)->data->tail), b_total_count = get_count_digits((*b)->data->head, (*b)->data->tail);
    unsigned int a_up_place_count = a_total_count - (*a)->data->deciaml_point, b_up_place_count = b_total_count - (*b)->data->deciaml_point;
    if (a_up_place_count == b_up_place_count) {
        Digits *a_now = (*a)->data->head->next, *b_now = (*b)->data->head->next;
        bool is_same = true;
        while (a_now != (*a)->data->tail) {
            if (a_now->data > b_now->data) {
                is_same = false;
                break;
            } else if (a_now->data < b_now->data) {
                is_same = false;
                is_swap = true;
                Expression *tmp = *a;
                *a = *b;
                *b = tmp;
                break;
            }

            a_now = a_now->next;
            b_now = b_now->next;
        }

        if (is_same) {
            release_numbers(&((*a)->data));
            release_numbers(&((*b)->data));
            free(*a);
            free(*b);

            result->data->deciaml_point = 0;
            return result;
        }
    } else {
        if (a_up_place_count < b_up_place_count) {
            Expression *tmp = *a;
            *a = *b;
            *b = tmp;
            is_swap = true;
        }
    }

    make_up_place_count_same(&((*a)->data), &((*b)->data));

    Digits *a_now = (*a)->data->tail->prev, *b_now = (*b)->data->tail->prev;
    bool is_down = false;
    while (b_now != (*b)->data->head) {
        unsigned int a_num = a_now->data - '0', b_num = b_now->data - '0';
        if (is_down) {
            if (a_num == 0) {
                a_num += 9;
            } else {
                a_num -= 1;
                is_down = false;
            }
        }
        if (a_num < b_num) {
            is_down = true;
            a_num += 10;
        }
        digit_insert_head(a_num - b_num + '0', result->data->head);

        a_now = a_now->prev;
        b_now = b_now->prev;
    }

    result->data->deciaml_point = (*a)->data->deciaml_point;
    result->data->deciaml_point -= deletee_zero_down_deciaml(result->data->head, result->data->tail, result->data->deciaml_point);
    deletee_zero_up_deciaml(result->data->head, result->data->tail);

    if (is_swap) {
        if ((*a)->data->isPositive) {
            result->data->isPositive = false;
        } else {
            result->data->isPositive = true;
        }
    } else {
        if ((*a)->data->isPositive) {
            result->data->isPositive = true;
        } else {
            result->data->isPositive = false;
        }
    }

    release_numbers(&((*a)->data));
    release_numbers(&((*b)->data));
    free(*a);
    free(*b);

    return result;
}

/*
곱하기 연산을 수행하는 함수. a * b

Number **a: 곱하기 연산을 수행할 숫자 (왼쪽)
Number **b: 곱하기 연산을 수행할 숫자 (오른쪽)
*/
Expression *multiplication(Expression **a, Expression **b) {
    Expression *result = init_expression_empty_noode_number(); // 결과를 저장할 표현식 초기화

    // a와 b의 전체 자릿수를 비교해서 자릿수가 더 많은 숫자가 a가 되도록 조정
    unsigned int a_total_count = get_count_digits((*a)->data->head, (*a)->data->tail), b_total_count = get_count_digits((*b)->data->head, (*b)->data->tail);
    if (a_total_count < b_total_count) {
        Expression *tmp = *a;
        *a = *b;
        *b = tmp;
    }

    Digits *now_b = (*b)->data->tail->prev; // b의 숫자 한 자리씩 저장할 변수

    int ten = 0;
    while (now_b != (*b)->data->head) {
        unsigned int carry = 0;
        Digits *now_a = (*a)->data->tail->prev;
        Expression *tmp = init_expression_empty_noode_number();
        tmp->data->deciaml_point = 0;

        while (now_a != (*a)->data->head) {
            unsigned int num_res = (now_a->data - '0') * (now_b->data - '0');
            num_res += carry;
            carry = num_res / 10;
            num_res %= 10;
            digit_insert_head(num_res + '0', tmp->data->head);
            now_a = now_a->prev;
        }

        if (carry != 0) {
            digit_insert_head(carry + '0', tmp->data->head);
        }

        for (int i = 0;i < ten;i++) {
            digit_insert_tail('0', tmp->data->tail);
        }

        result = addition(&result, &tmp);
        ten++;
        now_b = now_b->prev;
    }

    result->data->deciaml_point = (*a)->data->deciaml_point + (*b)->data->deciaml_point;
    result->data->deciaml_point -= deletee_zero_down_deciaml(result->data->head, result->data->tail, result->data->deciaml_point);

    if ((*a)->data->isPositive == (*b)->data->isPositive) {
        result->data->isPositive = true;
    } else {
        result->data->isPositive = false;
    }

    release_numbers(&((*a)->data));
    release_numbers(&((*b)->data));
    free(*a);
    free(*b);

    return result;
}

/*
계산 과정에서 오류 발생 시 오류 출력과 메모리 해제하는 함수

ExpressHeadTail **expht: 메모리 해제할 표현식 포인터
 Stack **stack :메모리 해제할 스택 포인터
 Expression *one: 스택에서 pop된 노드 포인터
 Expression *two: 스택에서 pop된 노드 포인터
 Expression *result: 연산을 수행한 결과 노드 포인터
 ERROR_TYPE error: 에러 타입
*/
Number *print_error_calculation(ExpressHeadTail **expht, Stack **stack, Expression *one, Expression *two, Expression *result, ERROR_TYPE error) {
    alert_error(error); // 오류 설명 출력
    delete_stack(stack); // 스택 메모리 해제
    if (one != NULL) {
        free(one); // one이 null이 아니라면 메모리 해제
    }
    if (two != NULL) {
        free(two); // two이 null이 아니라면 메모리 해제
    }
    if (result != NULL) {
        free(result); // result이 null이 아니라면 메모리 해제
    }
    release_all(expht); // 저장하던 식 표현 모두 해제
    return NULL;
}

/*
계산을 수행하는 함수

ExpressHeadTail **expht 계산을 수행 할 식
*/
Number *calculation(ExpressHeadTail **expht) {
    Stack *stack = init_stack(); // 스택 초기화
    Expression *now = (*expht)->head->next; // 표현식 데이터가 있는 가장 처음 부분 가져오기
    Expression *tail = (*expht)->tail; // 표현식 끝 부분 가져오기

    while (now != tail) { // 마지막까지 반복문으로 하나씩 불러옴
        // now에 연결된 노드들은 모두 연결을 끊고, now의 앞 뒤 노드를 서로 연결시켜줌
        now->prev->next = now->next;
        now->next->prev = now->prev;
        now->next = NULL;
        now->prev = NULL;

        if (now->type == TYPE_DIGIT) {
            // now가 수 라면
            push(stack, now); // 스택에 psuh
        } else {
            // now기 표현식이라면
            // 스택에 들어있던 숫자 두개를 pop
            Expression *two = pop(stack), *one = pop(stack), *result = NULL;
            char op = now->opr; // 연산자 가져옴

            if (one == NULL || two == NULL) {
                // 숫자가 1개 이하라면 오류처리
                return print_error_calculation(expht, &stack, one, two, result, ERROR_CALCULATION_ERROR);
            }

            switch (op) { // 연산자에 따라 연산 수행
                case '+':
                    result = addition(&one, &two);
                    break;
                case '-':
                    result = subtraction(&one, &two);
                    break;
                case '*':
                    result = multiplication(&one, &two);
                    break;
            }
            if (result == NULL) {
                // 연산 과정에서 오류가 있다면 오류처리
                return print_error_calculation(expht, &stack, one, two, result, ERROR_CALCULATION_ERROR);
            } else {
                // 오류가 없다면 계산 결과 스택에 push
                push(stack, result);
            }
        }
        now = (*expht)->head->next; // now를 다음 노드로 변환
    }
    Number *n = pop(stack)->data; // 결과 가져오기

    free(stack); // 스택 메모리 해제
    release_all(expht); // 식 메모리 해제
    return n; // 계산 결과 리턴
}