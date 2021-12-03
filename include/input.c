#include "input.h"

int is_digit(char ch) {
    if ('0' <= ch && '9' >= ch) {
        return 1;
    } else {
        return 0;
    }
}

ExpressHeadTail* read_and_anlyze(char *filename) {
    int is_first = 1;
    char in_ch;

    FILE *fp = fopen(filename, "r");
    ExpressHeadTail *eht = init_expression();
    Number *number = NULL;

    if (fp == NULL) {
        printf("file open error\n");
    } else {
        while ((in_ch = fgetc(fp)) != EOF) {
            if (detect_error_invalid_chracter(in_ch) == ERROR) {
                alert_error(ERROR_INVALID_CHARACTER);
                release_all(number);
                return NULL;
            } else {
                if (is_digit(in_ch)) {
                    if (number == NULL) {
                        number = init_number();
                    }
                    insert_tail(in_ch, (*number).up_decimal_point_tail);
                } else {
                    if (number != NULL) {
                        expression_insert_tail(TYPE_DIGIT, number, '[ßß', (*eht).tail);
                        number = NULL;
                    }
                    expression_insert_tail(TYPE_OPR, NULL, in_ch, (*eht).tail);
                }
            }
            is_first = 0;
        }
        putchar('\n');
    }
    fclose(fp);

    return eht;
}