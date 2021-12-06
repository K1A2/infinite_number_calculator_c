#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include "express/number.h"
#include "error.h"

void alert_error(ERROR_TYPE type) {
    switch (type) {
        case ERROR_INVALID_CHARACTER:
            printf("ERROR: Input invalid character.\n");
            break;
        case ERROR_FILE_IO:
            printf("ERROR: Can't read file.\n");
            break;
        case ERROR_TOO_MANY_DECIMAL_POINT:
            printf("ERROR: Too many decimal point in one number.\n");
            break;
        case ERROR_TOO_MANY_OPERATOR:
            printf("ERROR: Too many operators exist in a row.\n");
            break;
        case ERROR_OPERATOR_WRONG_ORDER:
            printf("ERROR: The order of the operators is wrong.\n");
            break;
        case ERROR_TOO_MANY_OPERATOR_BEFORE_BRACKET:
            printf("ERROR: Too many operators exist before left brackets.\n");
            break;
        case ERROR_RIGHT_BRACKET_WRONG_POSITION:
            printf("ERROR: Right bracket wrong position.\n");
            break;
        case ERROR_RIGHT_BRACKET_AFTER_OPERATOR:
            printf("ERROR: Right bracket is placed after a operator.\n");
            break;
        case ERROR_MUTIPLE_OPERATOR_WRONG_POSITION:
            printf("ERROR: Multiolication operator wrong position.\n");
            break;
        case ERROR_OPERATOR_FIRST_ERROR:
            printf("ERROR: Operators can only one in first position.\n");
            break;
        case ERROR_NOTHING_IN_BRACKETS:
            printf("ERROR: There's nothing in barckets.\n");
            break;
    }
}

ErrorOk detect_error_invalid_chracter(char ch) {
    if (('0' <= ch && '9' >= ch) || ch == '+' || ch == '*' || ch == '-' || ch == '.' || ch == '(' || ch == ')') {
        return OK;
    } else {
        return ERROR;
    }
}

ErrorOk detect_error_all_cases(ExpressHeadTail expht) {

}