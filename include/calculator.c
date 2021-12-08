#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "express/number.h"
#include "express/stack.h"
#include "error.h"
#include "calculator.h"

Expression *addition(Expression *a, Expression *b) {
    if (a->data->isPositive != b->data->isPositive) {
        // subtraction()
        return NULL;
    } else {
        int down_digits_count_a = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail);
        int down_digits_count_b = get_count_digits(b->data->down_decimal_point_head, b->data->down_decimal_point_tail);
        int up_digits_count_a = get_count_digits(a->data->up_decimal_point_head, a->data->up_decimal_point_tail);
        int up_digits_count_b = get_count_digits(b->data->up_decimal_point_head, b->data->up_decimal_point_tail);
        int decimal_point_pos_result = down_digits_count_a >= down_digits_count_b ? down_digits_count_a : down_digits_count_b;

        int i = 0;
        if (up_digits_count_a > up_digits_count_b) {
            for (i = 0;i < up_digits_count_a - up_digits_count_b;i++) {
                digit_insert_head('0', b->data->up_decimal_point_head);
            }
        } else if (up_digits_count_a < up_digits_count_b) {
            for (i = 0;i < up_digits_count_b - up_digits_count_a;i++) {
                digit_insert_head('0', a->data->up_decimal_point_head);
            }
        }
        if (down_digits_count_a > down_digits_count_b) {
            for (i = 0;i < down_digits_count_a - down_digits_count_b;i++) {
                digit_insert_tail('0', b->data->down_decimal_point_tail);
            }
        } else if (down_digits_count_a < down_digits_count_b) {
            for (i = 0;i < down_digits_count_b - down_digits_count_a;i++) {
                digit_insert_tail('0', a->data->down_decimal_point_tail);
            }
        }

        Expression *result = init_expression_empty_noode_number();
        result->data->isPositive = true;
        Digits *num_now_a = a->data->down_decimal_point_tail->prev, *num_now_b = b->data->down_decimal_point_tail->prev;
        unsigned int carry = 0;

        while (num_now_a != a->data->down_decimal_point_head) {
            unsigned int num_a = num_now_a->data - '0', num_b = num_now_b->data - '0';
            unsigned int result_int = num_a + num_b;

            result_int += carry;
            carry = result_int / 10;
            result_int %= 10;

            digit_insert_head(result_int + '0', result->data->up_decimal_point_head);

            num_now_a = num_now_a->prev;
            num_now_b = num_now_b->prev;
        }
        num_now_a = a->data->up_decimal_point_tail->prev;
        num_now_b = b->data->up_decimal_point_tail->prev;
        while (num_now_a != a->data->up_decimal_point_head) {
            unsigned int num_a = num_now_a->data - '0', num_b = num_now_b->data - '0';
            unsigned int result_int = num_a + num_b;

            result_int += carry;
            carry = result_int / 10;
            result_int %= 10;

            digit_insert_head(result_int + '0', result->data->up_decimal_point_head);

            num_now_a = num_now_a->prev;
            num_now_b = num_now_b->prev;
        }

        printf("%d %d\n", decimal_point_pos_result, get_count_digits(result->data->up_decimal_point_head, result->data->up_decimal_point_tail));
        // if (decimal_point_pos_result > 0) {
        //     Digits *now = result->data->up_decimal_point_tail->prev;
        //     for (int i = 0;i < decimal_point_pos_result - 1;i++) {
        //         now = now->prev;
        //     }
        //     Digits *up_last = now->prev;
        //     result->data->down_decimal_point_tail->prev = result->data->up_decimal_point_tail->prev;
        //     result->data->up_decimal_point_tail->prev->next = result->data->down_decimal_point_tail;
        //     now->prev = result->data->down_decimal_point_head;
        //     result->data->down_decimal_point_head->next = now;
        //     up_last->next = result->data->up_decimal_point_tail;
            // result->data->up_decimal_point_tail->prev = up_last;
        // }

        if (a->data->isPositive) {
            result->data->isPositive = true;
        } else {
            result->data->isPositive = false;
        }

        release_numbers(a->data);
        release_numbers(b->data);
        free(a);
        free(b);

        return result;
    }
}

Expression *subtraction(Expression *a, Expression *b) {

}

Expression *multiplication(Expression *a, Expression *b) {
    int decimal_point_pos_a = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail);
    int decimal_point_pos_b = get_count_digits(b->data->down_decimal_point_head, b->data->down_decimal_point_tail);
    int up_digits_count_a = get_count_digits(a->data->up_decimal_point_head, a->data->up_decimal_point_tail);
    int up_digits_count_b = get_count_digits(b->data->up_decimal_point_head, b->data->up_decimal_point_tail);
    int decimal_point_pos_result = decimal_point_pos_a + decimal_point_pos_b;
    int digits_total_count_a = decimal_point_pos_a + up_digits_count_a, digits_total_count_b = decimal_point_pos_b + up_digits_count_b;
    unsigned int now_place = 0, carry = 0;

    if (digits_total_count_b > digits_total_count_a) {
        Expression *tmp = b;
        b = a;
        a = tmp;
    }

    number_concatenate(a);
    number_concatenate(b);


    deletee_zero_up_deciaml(a->data->up_decimal_point_head, a->data->down_decimal_point_tail);
    deletee_zero_up_deciaml(b->data->up_decimal_point_head, b->data->down_decimal_point_tail);

    Expression *result = init_expression_empty_noode_number(), *tmp = init_expression_empty_noode_number();
    result->data->isPositive = true;
    tmp->data->isPositive = true;
    digit_insert_tail('0', result->data->up_decimal_point_tail);
    digit_insert_tail('0', result->data->down_decimal_point_tail);
    digit_insert_tail('0', tmp->data->down_decimal_point_tail);
    
    Digits *num_now_b = b->data->down_decimal_point_tail->prev;
    while (num_now_b != b->data->up_decimal_point_head) {
        Digits *num_now_a = a->data->down_decimal_point_tail->prev;
        unsigned int int_b = num_now_b->data - '0';
        carry = 0;

        while (num_now_a != a->data->up_decimal_point_head) {
            unsigned int int_a = num_now_a->data - '0';
            unsigned int result_int = int_b * int_a;

            result_int += carry;
            carry = result_int / 10;
            result_int %= 10;

            digit_insert_head(result_int + '0', tmp->data->up_decimal_point_head);

            num_now_a = num_now_a->prev;
        }
        for (int i = 0;i < now_place;i++) {
            digit_insert_tail('0', tmp->data->up_decimal_point_tail);
        }
        result = addition(result, tmp);
        tmp = init_expression_empty_noode_number();
        tmp->data->isPositive = true;
        digit_insert_tail('0', tmp->data->down_decimal_point_tail);
        num_now_b = num_now_b->prev;
        now_place += 1;
    }

    // if (decimal_point_pos_result > 0) {
    //     if (get_count_digits(result->data->up_decimal_point_head, result->data->up_decimal_point_tail) < decimal_point_pos_result) {

    //     } else {
    //         Digits *now = result->data->up_decimal_point_tail->prev;
    //         for (int i = 0;i < decimal_point_pos_result - 1;i++) {
    //             now = now->prev;
    //         }
    //         Digits *up_last = now->prev;
    //         result->data->down_decimal_point_tail->prev = result->data->up_decimal_point_tail->prev;
    //         result->data->up_decimal_point_tail->prev->next = result->data->down_decimal_point_tail;
    //         now->prev = result->data->down_decimal_point_head;
    //         result->data->down_decimal_point_head->next = now;
    //         up_last->next = result->data->up_decimal_point_tail;
    //         result->data->up_decimal_point_tail->prev = up_last;
    //     }
    // }

    if (a->data->isPositive == b->data->isPositive) {
        result->data->isPositive = true;
    } else {
        result->data->isPositive = false;
    }

    release_numbers_concatenated(a->data);
    release_numbers_concatenated(b->data);
    free(a);
    free(b);
    return result;
}

Number *print_error_calculation(ExpressHeadTail *expht, Stack *stack, Expression *one, Expression *two, Expression *result, ERROR_TYPE error) {
    alert_error(error);
    delete_stack(stack);
    if (one != NULL) {
        free(one);
    }
    if (two != NULL) {
        free(two);
    }
    if (result != NULL) {
        free(result);
    }
    free(expht);
    return NULL;
}

Number *calculation(ExpressHeadTail *expht) {
    Stack *stack = init_stack();
    Expression *now = expht->head->next;
    Expression *tail = expht->tail;

    while (now != tail) {
        now->prev->next = now->next;
        now->next->prev = now->prev;
        now->next = NULL;
        now->prev = NULL;

        if (now->type == TYPE_DIGIT) {
            push(stack, now);
        } else {
            Expression *one = pop(stack), *two = pop(stack), *result = NULL;
            char op = now->opr;

            if (one == NULL || two == NULL) {
                return print_error_calculation(expht, stack, one, two, result, ERROR_CALCULATION_ERROR);
            }

            switch (op) {
                case '+':
                    result = addition(one, two);
                    break;
                case '-':
                    result = subtraction(one, two);
                    break;
                case '*':
                    result = multiplication(one, two);
                    break;
            }
            if (result == NULL) {
                return print_error_calculation(expht, stack, one, two, result, ERROR_CALCULATION_ERROR);
            } else {
                Number *numbers = result->data;
                putchar('\n');
                if (!numbers->isPositive) {
                    putchar('-');
                }
                print_nodes_from_head(numbers->up_decimal_point_head, numbers->up_decimal_point_tail);
                putchar('.');
                print_nodes_from_head(numbers->down_decimal_point_head, numbers->down_decimal_point_tail);
                putchar('\n');
                push(stack, result);
            }
        }
        // free(now);
        now = expht->head->next;
    }

    return pop(stack)->data;
}