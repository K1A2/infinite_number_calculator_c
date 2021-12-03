#include "input.h"

EXPRESSION_TYPE is_digit(char ch) {
    if ('0' <= ch && '9' >= ch) {
        return TYPE_DIGIT;
    } else {
        return TYPE_OPR;
    }
}

ExpressHeadTail* read_and_anlyze(char *filename) {
    int is_first = 1;
    char in_ch;

    FILE *fp = fopen(filename, "r");
    ExpressHeadTail eht = init_expression();
    Number number;
    int is_number = 0;

    if (fp == NULL) {
        printf("file open error\n");
        return NULL;
    } else {
        while ((in_ch = fgetc(fp)) != EOF) {
            if (detect_error_invalid_chracter(in_ch) == ERROR) {
                alert_error(ERROR_INVALID_CHARACTER);
                release_all(eht);
                return NULL;
            } else {
                if (is_digit(in_ch) == TYPE_DIGIT) {
                    if (is_number == 0) {
                        number = init_number();
                        is_number = 1;
                    }
                    insert_tail(in_ch, number.up_decimal_point_tail);
                } else {
                    if (is_number == 1) {
                        expression_insert_tail(TYPE_DIGIT, number, ' ', eht.tail);
                        is_number = 0;
                    }
                    Number n;
                    expression_insert_tail(TYPE_OPR, n, in_ch, eht.tail);
                }
            }
            is_first = 0;
        }
        putchar('\n');
    }
    fclose(fp);

    return &eht;
}