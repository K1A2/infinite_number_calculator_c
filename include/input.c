#include <stdio.h>
#include <stdbool.h>

#include "express/number.h"
#include "error.h"
#include "input.h"

EXPRESSION_TYPE is_digit(char ch) {
    if ('0' <= ch && '9' >= ch) {
        return TYPE_DIGIT;
    } else {
        return TYPE_OPR;
    }
}

ExpressHeadTail* print_error(ExpressHeadTail eht, ERROR_TYPE type) {
    alert_error(type);
    release_all(eht);
    return NULL;
}

void add_number() {

}

ExpressHeadTail* read_and_anlyze(char *filename) {
    bool is_first = true;
    bool is_number = false;
    bool is_decimal_show = false;
    bool is_positive_num = true;
    bool is_right_bracket = false;
    bool is_left_bracket = false;

    char in_ch;
    int count_op_continue = 0;

    FILE *fp = fopen(filename, "r");
    ExpressHeadTail eht = init_expression();
    Number number;

    if (fp == NULL) {
        alert_error(ERROR_FILE_IO);
        return NULL;
    } else {
        while ((in_ch = fgetc(fp)) != EOF) {
            if (detect_error_invalid_chracter(in_ch) == ERROR) {
                return print_error(eht, ERROR_INVALID_CHARACTER);
            } else {
                if (is_digit(in_ch) == TYPE_DIGIT) {
                    count_op_continue = 0;
                    is_first = false;
                    if (!is_number) {
                        number = init_number();
                        number.isPositive = is_positive_num;
                        is_decimal_show = false;
                        is_number = true;
                    }
                    if (!is_decimal_show) {
                        insert_tail(in_ch, number.up_decimal_point_tail);
                    } else {
                        insert_tail(in_ch, number.down_decimal_point_tail);
                    }
                } else {
                    if (count_op_continue >= 2) {
                        return print_error(eht, ERROR_TOO_MANY_OPERATOR);
                    } else if(is_first && count_op_continue >= 1) {
                        return print_error(eht, ERROR_OPERATOR_FIRST_ERROR);
                    } else {
                        count_op_continue += 1;
                        if (in_ch == '.') {
                            if (!is_decimal_show) {
                                if (!is_number) {
                                    number = init_number();
                                    number.isPositive = is_positive_num;
                                    is_decimal_show = true;
                                    is_number = true;
                                }
                                is_decimal_show = true;
                                count_op_continue = 0;
                            } else {
                                return print_error(eht, ERROR_TOO_MANY_DECIMAL_POINT);
                            }
                        } else if (in_ch == '-') {
                            is_left_bracket = false;
                            if (is_number) {
                                if (is_right_bracket) {
                                    Number n;
                                    expression_insert_tail(TYPE_OPR, n, '*', eht.tail);
                                }
                                expression_insert_tail(TYPE_DIGIT, number, ' ', eht.tail);
                                is_number = false;
                                is_positive_num = true;
                                is_decimal_show = false;
                                Number n;
                                expression_insert_tail(TYPE_OPR, n, in_ch, eht.tail);
                            } else {
                                if (is_first || count_op_continue == 2) {
                                    is_positive_num = false;
                                } else if (count_op_continue == 1) {
                                    Number n;
                                    expression_insert_tail(TYPE_OPR, n, in_ch, eht.tail);
                                }
                            }
                            is_right_bracket = false;
                        } else if (in_ch == '+') {
                            is_left_bracket = false;
                            if (is_number) {
                                if (is_right_bracket) {
                                    Number n;
                                    expression_insert_tail(TYPE_OPR, n, '*', eht.tail);
                                }
                                expression_insert_tail(TYPE_DIGIT, number, ' ', eht.tail);
                                is_number = false;
                                is_positive_num = true;
                                is_decimal_show = false;
                                Number n;
                                expression_insert_tail(TYPE_OPR, n, in_ch, eht.tail);
                            } else {
                                is_positive_num = true;
                            }
                            is_right_bracket = false;
                        } else if (in_ch == '(') {
                            if (count_op_continue >= 3) {
                                return print_error(eht, ERROR_TOO_MANY_OPERATOR_BEFORE_BRACKET);
                            } else if (count_op_continue == 1) {
                                if (is_right_bracket) {
                                    Number n;
                                    expression_insert_tail(TYPE_OPR, n, '*', eht.tail);
                                }
                                if (is_number) {
                                    expression_insert_tail(TYPE_DIGIT, number, ' ', eht.tail);
                                    is_number = false;
                                    is_positive_num = true;
                                    is_decimal_show = false;
                                    if (!is_left_bracket) {
                                        Number n;
                                        expression_insert_tail(TYPE_OPR, n, '*', eht.tail);
                                    }
                                }
                            }
                            Number n;
                            expression_insert_tail(TYPE_OPR, n, in_ch, eht.tail);
                            count_op_continue = 0;
                            is_first = true;
                            is_left_bracket = true;
                            is_right_bracket = false;
                        } else if (in_ch == ')') {
                            is_left_bracket = false;
                            if (is_first) {
                                return print_error(eht, ERROR_RIGHT_BRACKET_WRONG_POSITION);
                            } else if (count_op_continue >= 2) {
                                return print_error(eht, ERROR_RIGHT_BRACKET_AFTER_OPERATOR);
                            } else {
                                if (is_number) {
                                    if (is_right_bracket) {
                                        Number n;
                                        expression_insert_tail(TYPE_OPR, n, '*', eht.tail);
                                    }
                                    expression_insert_tail(TYPE_DIGIT, number, ' ', eht.tail);
                                    is_number = false;
                                    is_positive_num = true;
                                    is_decimal_show = false;
                                }
                                Number n;
                                expression_insert_tail(TYPE_OPR, n, in_ch, eht.tail);
                                count_op_continue = 0;
                                is_first = false;
                            }
                            is_right_bracket = true;
                        } else { // *, /
                            is_left_bracket = false;
                            if (is_first) {
                                return print_error(eht, ERROR_MUTIPLE_OPERATOR_WRONG_POSITION);
                            } else if (count_op_continue >= 2) {
                                return print_error(eht, ERROR_TOO_MANY_OPERATOR);
                            } else {
                                if (is_number) {
                                    if (is_right_bracket) {
                                        Number n;
                                        expression_insert_tail(TYPE_OPR, n, '*', eht.tail);
                                    }
                                    expression_insert_tail(TYPE_DIGIT, number, ' ', eht.tail);
                                    is_number = false;
                                    is_positive_num = true;
                                    is_decimal_show = false;
                                    Number n;
                                    expression_insert_tail(TYPE_OPR, n, in_ch, eht.tail);
                                } else {
                                    return print_error(eht, ERROR_OPERATOR_WRONG_ORDER);
                                }
                            }
                            is_right_bracket = false;
                        }
                    }
                }
            }
        }
        if (is_number) {
            if (is_right_bracket) {
                Number n;
                expression_insert_tail(TYPE_OPR, n, '*', eht.tail);
            }
            expression_insert_tail(TYPE_DIGIT, number, ' ', eht.tail);
        }
    }
    fclose(fp);

    return &eht;
}