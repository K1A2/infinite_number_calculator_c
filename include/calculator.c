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
        // a와 b의 부호가 다르다면 빼기연산으로 계산
        if (!(*b)->data->isPositive) {
            // b 부호가 - 라면 +로 변경 (a+(-b)) = (a-(+b))
            (*b)->data->isPositive = true;
        } else {
            // a 부호가 - 라면 +로 변경 (b+(-a)) = (b-(+a))
            (*a)->data->isPositive = true;
            Expression *tmp = *a;
            *a = *b;
            *b = tmp;
        }
        return subtraction(a, b); // 뺄샘 진행
    } else {
        Expression *result = init_expression_empty_noode_number(); // 결과를 저장할 변수

        make_down_place_count_same(&((*a)->data), &((*b)->data), &(result->data)); // 소수점 아래 자리수 개수 0을 넣어 맞춤
        make_up_place_count_same(&((*a)->data), &((*b)->data)); // 소수점 윗 자리수 개수 0을 넣어 맞춤

        // a, b의 자리수를 하나씩 가져와 저장
        Digits *now_a = (*a)->data->tail->prev;
        Digits *now_b = (*b)->data->tail->prev;
        unsigned int carry = 0; // 올림 수 저장
        while (now_a != (*a)->data->head) { // a 자리수 끝까지 저장
            unsigned int num_res = (now_a->data - '0') + (now_b->data - '0'); // a. b  자리수를 하나씩 가져와 +
            num_res += carry; // 올림 수 +
            carry = num_res / 10; // 올림 수 업데이트
            num_res %= 10; // 결과 업데이트

            digit_insert_head(num_res + '0', result->data->head); // 결과 변수에 추가
            
            // 모두 다음으로 업데이트
            now_a = now_a->prev;
            now_b = now_b->prev;
        }

        if (carry != 0) {
            // 올림수가 0이 아니라면 맨 오른쪽에 추가
            digit_insert_head(carry + '0', result->data->head);
        }

        if (!(*a)->data->isPositive) {
            // ab가 모두 음수라면 결과값도 음수처리
            result->data->isPositive = false;
        }

        // 모든 변수 메모리 해제
        release_numbers(&((*a)->data));
        release_numbers(&((*b)->data));
        free(*a);
        free(*b);

        return result; // 결과값 리턴
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
        return addition(a, b); // 더하기로 계산
    } else if (!(*a)->data->isPositive && (*b)->data->isPositive) { // a = -, b = +
        (*b)->data->isPositive = false;
        return addition(a, b); // 더하기로 계산
    }
    Expression *result = init_expression_empty_noode_number(); // 계산 결과를 저장할 구조체

    make_down_place_count_same(&((*a)->data), &((*b)->data), &(result->data)); // 소수점 아래 자리수의 개수를 같게 만듬

    bool is_swap = false; // a와 b를 바꿨는지 여부

    unsigned int a_total_count = get_count_digits((*a)->data->head, (*a)->data->tail), b_total_count = get_count_digits((*b)->data->head, (*b)->data->tail); // a,b의 전체 자리수 개수
    unsigned int a_up_place_count = a_total_count - (*a)->data->deciaml_point, b_up_place_count = b_total_count - (*b)->data->deciaml_point; // a와 b의 소수점 윗 자리수 개수
    if (a_up_place_count == b_up_place_count) { // a,b의 소수점 윗 자리수 개수가 같다면
        Digits *a_now = (*a)->data->head->next, *b_now = (*b)->data->head->next; // a, b 모두 왼쪽부터 한 자리수씩 가져와서 대소비교
        bool is_same = true; // 두 수가 완전히 같은지 여부
        while (a_now != (*a)->data->tail) { // a,b모두 끝까지 가져옴
            if (a_now->data > b_now->data) {
                // a 자리수가 b 자리수보다 숫자가 클 때
                is_same = false;
                break;
            } else if (a_now->data < b_now->data) {
                // b 자리수가 a 자리수보다 숫자가 클 때
                is_same = false;
                is_swap = true; // swap true
                Expression *tmp = *a;
                *a = *b;
                *b = tmp;
                break;
            }

            // a, b 모두 다음 자리수로 업데이트
            a_now = a_now->next;
            b_now = b_now->next;
        }

        if (is_same) {
            // 두 수가 완전히 같다면 0으로 리턴
            release_numbers(&((*a)->data));
            release_numbers(&((*b)->data));
            free(*a);
            free(*b);

            result->data->deciaml_point = 0;
            return result;
        }
    } else {
        // a, b의 소수점 위 자리수 개수가 다르다면 더 많은쪽을 a가 되도록 지정
        if (a_up_place_count < b_up_place_count) {
            Expression *tmp = *a;
            *a = *b;
            *b = tmp;
            is_swap = true;
        }
    }

    make_up_place_count_same(&((*a)->data), &((*b)->data)); // 윗 자리수도 개수가 같도록 0을 넣어 업데이트

    Digits *a_now = (*a)->data->tail->prev, *b_now = (*b)->data->tail->prev; // a와 b를 한 자리씩 가져와 저장하는 변수
    bool is_down = false; // 내림 여부
    while (b_now != (*b)->data->head) { // b 끝까지 반복
        unsigned int a_num = a_now->data - '0', b_num = b_now->data - '0'; // a, b 모두 각 자리수를 하나씩 가져옴
        if (is_down) { // 내림 true
            if (a_num == 0) { // 가져온 a 자리수가 0이라면
                a_num += 9; // 9 더하고 내림 여부 true 유지
            } else { // 0이 아니라면
                a_num -= 1; // a 자리수에서 1을 빼고 내림 여부 false
                is_down = false;
            }
        }
        if (a_num < b_num) { // b 자리수가 a 자리수에서 가져온 수 보다 클 경우
            is_down = true; // 내림 true 후 a 에 10 더함
            a_num += 10;
        }
        digit_insert_head(a_num - b_num + '0', result->data->head); // a - b

        // a아 b 모두 다음 자리수로 업데이트
        a_now = a_now->prev;
        b_now = b_now->prev;
    }

    result->data->deciaml_point = (*a)->data->deciaml_point; // 결과값 소수점 위치를 a의 위치로 지정
    result->data->deciaml_point -= deletee_zero_down_deciaml(result->data->head, result->data->tail, result->data->deciaml_point); // 소수점 아래 자릿수에서 가장 아래 자리수부터 0인 부분을 지움 예) 323.430000 -> 323.43
    deletee_zero_up_deciaml(result->data->head, result->data->tail); // 소수점 위 자리수에서 맨 앞부터 0인 부분을 지움 예) 000032.3 -> 32.3

    if (is_swap) { // a와 b를 바꿨다면
        if ((*a)->data->isPositive) {
            // a 가 양수라면 결과는 음수 -(a - b)
            result->data->isPositive = false;
        } else {
            // a 가 음수라면 결과는 음수 -(-a - b)
            result->data->isPositive = true;
        }
    } else { // a와 b를 바꾸지 않았다면
        if ((*a)->data->isPositive) {
            // a 가 양수라면 결과는 음수 (a - b)
            result->data->isPositive = true;
        } else {
            // a 가 음수라면 결과는 음수 (-a - b)
            result->data->isPositive = false;
        }
    }

    // 모든 변수 메모리 해제
    release_numbers(&((*a)->data));
    release_numbers(&((*b)->data));
    free(*a);
    free(*b);

    return result; // 결과값 리턴
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

    int ten = 0; // 계산 결과 뒤에 0을 몇개 붙여줄지
    while (now_b != (*b)->data->head) { // b의 자리수를 하나씩 끝날때까지 반복
        unsigned int carry = 0; // 반올림 수 저장
        Digits *now_a = (*a)->data->tail->prev; // a의 한 자리수
        Expression *tmp = init_expression_empty_noode_number(); // 계산 과정에서 중간 과정을 저장할 변수
        tmp->data->deciaml_point = 0; // 소수점 위치 0으로 초기화

        while (now_a != (*a)->data->head) { // a의 자리수를 하나씩 끝날때까지 반복
            unsigned int num_res = (now_a->data - '0') * (now_b->data - '0'); // a 자리수 * b 자리수
            num_res += carry; // 올림 수 더해주기
            carry = num_res / 10; // 올림 수 업데이트
            num_res %= 10; // 계산 결과 자리수 업데이트
            digit_insert_head(num_res + '0', tmp->data->head); // 자리수 계산 결과 맨 앞에 추가
            now_a = now_a->prev; // a 자리수 다음으로 업데이트
        }

        if (carry != 0) {
            // 계산이 끝난 이후에 올림이 0 이상이라면 따로 추가
            digit_insert_head(carry + '0', tmp->data->head);
        }

        for (int i = 0;i < ten;i++) {
            // 자리수에 따라 10을 곱해줌
            digit_insert_tail('0', tmp->data->tail);
        }

        result = addition(&result, &tmp); // 결과괎과 자릿수 계산 결과를 더해줌
        ten++; // 10 자리수 개수 +1
        now_b = now_b->prev; // b 숫자 다음으로 업데이트
    }

    result->data->deciaml_point = (*a)->data->deciaml_point + (*b)->data->deciaml_point; // 결과값 소수점 위치 업데이트
    result->data->deciaml_point -= deletee_zero_down_deciaml(result->data->head, result->data->tail, result->data->deciaml_point); // 소수점 아래 자릿수에서 가장 아래 자리수부터 0인 부분을 지움 예) 323.430000 -> 323.43

    if ((*a)->data->isPositive == (*b)->data->isPositive) {
        // 두 수의 부호가 같다면 결과값은 양수
        result->data->isPositive = true;
    } else {
        // 다르다면 음수
        result->data->isPositive = false;
    }

    // 모든 변수 메모리 해제
    release_numbers(&((*a)->data));
    release_numbers(&((*b)->data));
    free(*a);
    free(*b);

    return result; // 결과 리턴
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