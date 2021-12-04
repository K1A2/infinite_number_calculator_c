#include <assert.h>
#include <stdio.h>

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