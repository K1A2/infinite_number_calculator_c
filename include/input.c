#include <stdio.h>
#include <stdbool.h>

#include "express/number.h"
#include "error.h"
#include "input.h"

/*
읽어온 문자가 숫자인지 아닌지 판별하는 함수
*/
EXPRESSION_TYPE is_digit(char ch) {
    if ('0' <= ch && '9' >= ch) {
        return TYPE_DIGIT;
    } else {
        return TYPE_OPR;
    }
}

// Number 구조체를 초기화 하는 부분
void init_number_struct(Number **number, bool *is_positive_num, bool *is_decimal_show, bool is_decimal_show_value, bool *is_number_struct, bool *is_left_bracket) {
    (*number) = init_number(); // Number 구조체 초기화
    (*number)->isPositive = *is_positive_num; // 음수 여부
    *is_decimal_show = is_decimal_show_value; // 소숫점이 보였는지 여부 대입
    *is_number_struct = true; // Number 구조체에 입력중인지 여부 true 대입
    *is_left_bracket = false; // (가 나왔는지 여부 false
}

// Number 구조체를 Expression 구조체에 추가하는 함수
void add_number(Number *number, Expression* tail, bool *is_number_struct, bool *is_positive_num, bool *is_decimal_show) {
    expression_insert_tail(TYPE_DIGIT, number, ' ', tail); // 입력 되던 숫자 추가
    *is_number_struct = false; // 숫자가 입력중인지 여부 false로 초기화
    *is_positive_num = true; // 숫자가 양수인지 여부 true 초기화
    *is_decimal_show = false; // 소숫점이 보였는지 여부 false로 초기화
}

/*
식에 오류가 있을때 오류 출력과 추가하던 구조체를 모두 삭제하는 함수 
*/
ExpressHeadTail* print_error(ExpressHeadTail *eht, ERROR_TYPE type) {
    alert_error(type);
    release_all(*eht);
    return NULL;
}

/*
파일에서 식을 읽어와 구조체로 바꾸는 함수.
*/
ExpressHeadTail* read_and_anlyze(char *filename) {
    bool is_first = true; // 식에서 첫 부분인지 판별하는 변수
    bool is_number_struct = false; // 지금 읽어오고 있는 문자가 숫자의 일부인지 확인하는 변수
    bool is_decimal_show = false; // 숫자에서 소수점이 보였는지를 판별하는 변수
    bool is_positive_num = true; // 숫자가 양수인지 음수인지 판별하는 변수
    bool is_right_bracket = false; // )가 이전에 있었는지 판별하는 변수
    bool is_left_bracket = false; // (가 이전에 있었는지 판별하는 변수

    char in_ch; // 읽어온 문자
    int count_op_continue = 0; // operator(연산자)가 연속으로 몇개 있는지 판별하는 함수

    FILE *fp = fopen(filename, "r");
    ExpressHeadTail *eht = init_expression(); // 전체 식을 표현하는 구조체
    Number *number, *none = 0x0; // 숫자를 표현하는 구조체, 빈 구조체

    if (fp == NULL) { // 파일을 읽어오는 과정에서 오류 발생 야부
        //오류 발생 시
        alert_error(ERROR_FILE_IO);
        return NULL;
    } else {
        while ((in_ch = fgetc(fp)) != EOF) { // 파일이 끝날때까지 한 문자씩 읽어오는 반복문
            if (detect_error_invalid_chracter(in_ch) == ERROR) { // 읽어온 문자가 숫자, 연산자, () 외에 다른 문자인지 여부
                // 허용된 문자 이외의 문자일 시
                return print_error(eht, ERROR_INVALID_CHARACTER);
            } else {
                if (is_digit(in_ch) == TYPE_DIGIT) { // 읽어온 문자가 숫자인지 여부
                    // 읽어온 문자가 숫자일 시
                    count_op_continue = 0; // 연속으로 나온 연산자의 갯수 0으로 초기화
                    is_first = false; // 식의 첫 부분인지 여부 false로 초기화
                    if (!is_number_struct) {
                        // Number 구조체에 입력 중이 아니었다면 (직전에 숫자가 나온적이 없다면)
                        init_number_struct(&number, &is_positive_num, &is_decimal_show, false, &is_number_struct, &is_left_bracket);
                    }
                    if (!is_decimal_show) {
                        // 이전에 소숫점이 보인적이 없다면 up 부분에 삽입
                        insert_tail(in_ch, number->up_decimal_point_tail);
                    } else {
                        // 이전에 소수점이 보인적이 있다면 down 부분에 삽입
                        insert_tail(in_ch, number->down_decimal_point_tail);
                    }
                } else {
                    if (count_op_continue >= 2) {
                        // 연속으로 나온 연산자 개수가 3개 이상이라면 오류 발생 (예. 43-++43, 23+--43 등)
                        return print_error(eht, ERROR_TOO_MANY_OPERATOR);
                    } else if(is_first && count_op_continue >= 1) {
                        // 식의 처음 부분에 연속으로 나온 연산자 개수가 2개 이상이라면 오류 발생 (예. -+39.3, +-344.2 등)
                        return print_error(eht, ERROR_OPERATOR_FIRST_ERROR);
                    } else {
                        // 연산자는 +, -, *, /만 해당. (), . 은 아님
                        count_op_continue += 1; // 연속으로 나온 연산자 수 +1
                        if (in_ch == '.') {
                            // 들어온 문자가 소수점 일때
                            if (!is_decimal_show) {
                                // 이전에 소수점이 보인적이 없다면
                                if (!is_number_struct) {
                                    // 숫자보다 소숫점이 먼저 보였다면 Number 구조체 초기화 (예. .09490 입략 -> 0.09490 으로 처리)
                                    init_number_struct(number, &is_positive_num, &is_decimal_show, true, &is_number_struct, &is_left_bracket);
                                }
                                is_decimal_show = true; // 소수점이 보였다는 뜻으로 true 대입
                                count_op_continue = 0; // 연속으로 보인 연산자 수 0으로 초기화
                            } else {
                                // 이전에 소수점이 보인적이 있다면 오류 발생 (예. 323.44.4 등)
                                return print_error(eht, ERROR_TOO_MANY_DECIMAL_POINT);
                            }
                        } else if (in_ch == '-' || in_ch == '+') {
                            // 입력된 문자가 +, - 연산자 일때
                            is_left_bracket = false; // 이전에 ( 가 보인적이 있는지 여부 false로 초기화
                            if (is_number_struct) {
                                // 직전에 숫자가 입력 중 이었다면
                                if (is_right_bracket) {
                                    // 이전에 ) 가 입력된 적이 있다면 * 연산자 추가
                                    // 예. (32.44+33.3)49.44 -> (32.44+33.3)*49.44 로 변환
                                    expression_insert_tail(TYPE_OPR, none, '*', eht->tail); // * 연산자 추가
                                }
                                add_number(number, eht->tail, &is_number_struct, &is_positive_num, &is_decimal_show); // 입력 됬던 숫자 추가
                                expression_insert_tail(TYPE_OPR, none, in_ch, eht->tail); // 현재 입력된 연산자 - 추가
                            } else {
                                // 직전에 숫자가 입력 중이 아니었다면
                                if (is_first || count_op_continue == 2) {
                                    // -가 식 맨 앞에나 직전에 다른 연산자가 있었을 경우 연산자가 아니라 음수 기호로 판별
                                    if (in_ch == '-') {
                                        is_positive_num = false; // 숫자 양수 여부 false 대입
                                    } else {
                                        is_positive_num = true; // 숫자 양수 여부 talse 대입
                                    }
                                } else if (count_op_continue == 1) {
                                    // 직전에 다른 연산자가 나오지 않았을 경우 연산자로 판별
                                    expression_insert_tail(TYPE_OPR, none, in_ch, eht->tail); // 현재 입력된 연산자 - 추가
                                } else {
                                    // 연속으로 나온 연산자 개수가 3개 이상이라면 오류 발생 (예. 43-++43, 23+--43 등)
                                    return print_error(eht, ERROR_TOO_MANY_OPERATOR);
                                } // count_op_continues는 반드시 1 이상
                            }
                            is_right_bracket = false; // ) 가 입력됬었는지 여부 false로 초기화
                        } else if (in_ch == '(') {
                            // 입력된 문자가 ( 일때
                            if (count_op_continue >= 3) {
                                // ( 입력 이전에 연산자가 연속으로 2개 이상이 나왔을때 오류 처리
                                return print_error(eht, ERROR_TOO_MANY_OPERATOR_BEFORE_BRACKET);
                            } else if (count_op_continue == 1) {
                                // ( 입력 이전에 연산자가 0개 입력되었을 때
                                if (is_right_bracket) {
                                    // 직전에 ) 괄호가 입력되었을 때 * 연산자 추가
                                    // 예) (444.33-33)33.3+122.323 -> (444.33-33)*33.3+122.323 처리
                                    expression_insert_tail(TYPE_OPR, none, '*', eht->tail);
                                }
                                if (is_number_struct) {
                                    // 이전에 숫자가 입력된 적이 있다면
                                    add_number(number, eht->tail, &is_number_struct, &is_positive_num, &is_decimal_show);
                                    if (!is_left_bracket) {
                                        // ( 가 숫자 다음에 처음 나온거라면 * 기호 추가
                                        // 예) 333.22(444.33-33) -> 333.22*(444.33-33) 처리
                                        expression_insert_tail(TYPE_OPR, none, '*', eht->tail);
                                    }
                                }
                            }
                            expression_insert_tail(TYPE_OPR, none, in_ch, eht->tail); // 현재 입력된 ( 추가
                            count_op_continue = 0; // 연산자가 연속으로 나온 횟수 0으로 초기화. (는 연산자 기호가 아님.
                            is_first = true; // ( 기호 다음은 식의 처음 부분이라고 판단. 식의 처음 부분과 같이 연산자가 연속 2개 이상 나올 수 없기 때문.
                            is_left_bracket = true; // ( 가 나왔는지 여부 true
                            is_right_bracket = false; // ) 가 나왔는지 여부 false
                        } else if (in_ch == ')') {
                            // 입력된 문자가 ) 일때
                            if (is_left_bracket) {
                                // 직전에 (가 나왔을때 오류 처리 ()
                                return print_error(eht, ERROR_NOTHING_IN_BRACKETS);
                            }
                            if (is_first) {
                                // 식의 첫 부분이라면 오류 처리
                                return print_error(eht, ERROR_RIGHT_BRACKET_WRONG_POSITION);
                            } else if (count_op_continue >= 2) {
                                // ) 앞에 연산자가 하나라도 있으면 오류 처리
                                return print_error(eht, ERROR_RIGHT_BRACKET_AFTER_OPERATOR);
                            } else {
                                if (is_number_struct) {
                                    // 직전에 숫자가 입력중 이었다면
                                    if (is_right_bracket) {
                                        // 숫자 앞에 ) 가 있었다면 * 기호 추가
                                        // 예) (444.33-33)33.3+122.323 -> (444.33-33)*33.3+122.323 처리
                                        expression_insert_tail(TYPE_OPR, none, '*', eht->tail);
                                    }
                                    add_number(number, eht->tail, &is_number_struct, &is_positive_num, &is_decimal_show); // 숫자 추가
                                }
                                expression_insert_tail(TYPE_OPR, none, in_ch, eht->tail); // ) 기호 추가
                                count_op_continue = 0; // 연산자가 연속으로 나온 횟수 0으로 초기화. ) 는 연산자 기호가 아님.
                                is_first = false; // ) 다음은 식의 처음 부분이 아니라고 판단.
                            }
                            is_right_bracket = true; // ) 가 나왔는지 여부 true
                        } else { // *, /
                            is_left_bracket = false; // ( 가 나왔는지 여부 false
                            if (is_first) {
                                // *가 식의 첫 부분에 나왔다면 오류 처리
                                return print_error(eht, ERROR_MUTIPLE_OPERATOR_WRONG_POSITION);
                            } else if (count_op_continue >= 2) {
                                // * 연산자 앞에 다른 연산자가 하나 이상 나왔을때 오류 처리
                                return print_error(eht, ERROR_TOO_MANY_OPERATOR);
                            } else {
                                if (is_number_struct) {
                                    // 이전에 숫자가 입력된 적이 있다면
                                    if (is_right_bracket) {
                                        // ) 가 숫자 앞에 있다면 * 연산자 추가
                                        // 예) (444.33-33)33.3+122.323 -> (444.33-33)*33.3+122.323 처리
                                        expression_insert_tail(TYPE_OPR, none, '*', eht->tail);
                                    }
                                    add_number(number, eht->tail, &is_number_struct, &is_positive_num, &is_decimal_show); // 숫자 추가
                                    expression_insert_tail(TYPE_OPR, none, in_ch, eht->tail);
                                } else {
                                    // 앞에 숫자가 없다면 오류 처리
                                    // 예) *443
                                    return print_error(eht, ERROR_OPERATOR_WRONG_ORDER);
                                }
                            }
                            // )가 았는지 여부 false
                            is_right_bracket = false;
                        }
                    }
                }
            }
        }
        if (is_number_struct) {
            // 추가되지 않은 숫자가 있다면 추가
            if (is_right_bracket) {
                // ) 가 숫자 앞에 있다면 * 연산자 추가
                // 예) (444.33-33)33.3+122.323 -> (444.33-33)*33.3+122.323 처리
                expression_insert_tail(TYPE_OPR, none, '*', eht->tail);
            }
            add_number(number, eht->tail, &is_number_struct, &is_positive_num, &is_decimal_show);
        }
    }
    fclose(fp);

    return eht;
}