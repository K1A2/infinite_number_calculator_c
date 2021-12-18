#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include "express/number.h"
#include "error.h"

/*
에러를 출력하는 함수

ERROR_TYPE type: 에러 종류
*/
void alert_error(ERROR_TYPE type) {
    switch (type) {
        case ERROR_INVALID_CHARACTER: // 허용되지 않은 문자가 있는 오류
            printf("ERROR: Input invalid character.\n");
            break;
        case ERROR_FILE_IO: // 파일을 읽어올 수 없는 오류
            printf("ERROR: Can't read file.\n");
            break;
        case ERROR_TOO_MANY_DECIMAL_POINT: // 한 숫자 표현에 소수점이 두번 이상 나오는 경우
            printf("ERROR: Too many decimal point in one number.\n");
            break;
        case ERROR_TOO_MANY_OPERATOR: // 한번에 연속으로 연산자가 적절한 개수보다 더 많이 나올 경우
            printf("ERROR: Too many operators exist in a row.\n");
            break;
        case ERROR_OPERATOR_WRONG_ORDER: // 연산자 순서가 틀리게 나오는 경우
            printf("ERROR: The order of the operators is wrong.\n");
            break;
        case ERROR_TOO_MANY_OPERATOR_BEFORE_BRACKET: // ( 직전에 연산자가 2개 이상 존재하는 경우
            printf("ERROR: Too many operators exist before left brackets.\n");
            break;
        case ERROR_RIGHT_BRACKET_WRONG_POSITION: // ) 가 적절하지 않은 위치에 있는 경우
            printf("ERROR: Right bracket wrong position.\n");
            break;
        case ERROR_RIGHT_BRACKET_AFTER_OPERATOR: // ) 앞에 연산자가 존재하는 경우
            printf("ERROR: Right bracket is placed after a operator.\n");
            break;
        case ERROR_MUTIPLE_OPERATOR_WRONG_POSITION: // * 연산자가 식의 시작/끝 부분에 위치하는 경우
            printf("ERROR: Multiolication operator wrong position.\n");
            break;
        case ERROR_OPERATOR_FIRST_ERROR: // 식의 첫 부분에서 2개 이상의 연산자가 존재하는 경우
            printf("ERROR: Only one operator can exist in front of the expression or left bracket.\n");
            break;
        case ERROR_NOTHING_IN_BRACKETS: // () 안에 아무 숫자, 표현도 없는 경우
            printf("ERROR: There's nothing in barckets.\n");
            break;
        case ERROR_BRACKERS_COUNT_NOT_SAME: // (와 )가 개수가 서로 다른 경우
            printf("ERROR: The number of left brackets and right brackets is different.\n");
            break;
        case ERROR_RIGHT_BRACKET_FRIST: // 후위변환 과정에서 스택에 (가 없는데 )가 나온 경우
            printf("ERROR: The right bracket came out before the left bracket.\n");
            break;
        case ERROR_CALCULATION_ERROR: // 계산 과정에서 오류가 생긴 경우
            printf("ERROR: An error occurred during calculation.\n");
            break;
    }
}

/*
허용된 문자열인지 검사하는 함수

char ch: 검사 할 문자
*/
ErrorOk detect_error_invalid_chracter(char ch) {
    if (('0' <= ch && '9' >= ch) || ch == '+' || ch == '*' || ch == '-' || ch == '.' || ch == '(' || ch == ')') {
        return OK;
    } else {
        return ERROR;
    }
}