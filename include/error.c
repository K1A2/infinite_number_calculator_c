#include "error.h"
#include <stdio.h>

void alert_error(ERROR_TYPE type) {
    switch (type) {
        case ERROR_INVALID_CHARACTER:
            printf("ERROR: Input invalid character.\n");
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