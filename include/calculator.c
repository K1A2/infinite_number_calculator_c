#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "express/number.h"
#include "express/stack.h"
#include "error.h"
#include "calculator.h"

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

Expression *addition(Expression **a, Expression **b) {
    if ((*a)->data->isPositive != (*b)->data->isPositive) {
        // subtraction()
    } else {
        Expression *result = init_expression_empty_noode_number();

        make_down_place_count_same(&((*a)->data), &((*b)->data), &(result->data));

        unsigned int a_total_count = get_count_digits((*a)->data->head, (*a)->data->tail), b_total_count = get_count_digits((*b)->data->head, (*b)->data->tail);
        if (a_total_count >= b_total_count) {
            for (int i = 0;i < a_total_count - b_total_count;i++) {
                digit_insert_head('0', (*b)->data->head);
            }
        } else {
            for (int i = 0;i < b_total_count - a_total_count;i++) {
                digit_insert_head('0', (*a)->data->head);
            }
        }

        Digits *now_a = (*a)->data->tail->prev;
        Digits *now_b = (*b)->data->tail->prev;
        unsigned int carry = 0;
        while (now_a != (*a)->data->head) {
            unsigned int num_res = (now_a->data - '0') + (now_b->data - '0');
            num_res += carry;
            carry = num_res / 10;
            num_res %= 10;

            digit_insert_head(num_res + '0', result->data->head);

            now_a = now_a->prev;
            now_b = now_b->prev;
        }

        if (carry != 0) {
            digit_insert_head(carry + '0', result->data->head);
        }

        if (!(*a)->data->isPositive) {
            result->data->isPositive = false;
        }

        release_numbers(&((*a)->data));
        release_numbers(&((*b)->data));
        free(*a);
        free(*b);

        return result;
    }
}

Expression *subtraction(Expression**a, Expression **b) {

}

Expression *multiplication(Expression **a, Expression **b) {
    Expression *result = init_expression_empty_noode_number();

    make_down_place_count_same(&((*a)->data), &((*b)->data), &(result->data));

    unsigned int a_total_count = get_count_digits((*a)->data->head, (*a)->data->tail), b_total_count = get_count_digits((*b)->data->head, (*b)->data->tail);
    if (a_total_count < b_total_count) {
        Expression *tmp = *a;
        *a = *b;
        *b = tmp;
    }

    result->data->deciaml_point = (*a)->data->deciaml_point + (*b)->data->deciaml_point;
    Digits *now_b = (*b)->data->tail->prev;
    while (now_b != (*b)->data->head) {
        unsigned int carry = 0;
        Digits *now_a = (*a)->data->tail->prev;
        Expression * tmp = init_expression();
        tmp->data->deciaml_point = result->data->deciaml_point;
        while (now_a != (*a)->data->head) {
            unsigned int num_res = now_a->data * now_b->data;
        }
    }
}

Number *print_error_calculation(ExpressHeadTail **expht, Stack **stack, Expression *one, Expression *two, Expression *result, ERROR_TYPE error) {
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
    free(*expht);
    return NULL;
}

Number *calculation(ExpressHeadTail **expht) {
    Stack *stack = init_stack();
    Expression *now = (*expht)->head->next;
    Expression *tail = (*expht)->tail;

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
                return print_error_calculation(expht, &stack, one, two, result, ERROR_CALCULATION_ERROR);
            }

            switch (op) {
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
                return print_error_calculation(expht, &stack, one, two, result, ERROR_CALCULATION_ERROR);
            } else {
                push(stack, result);
            }
        }
        now = (*expht)->head->next;
    }
    Number *n = pop(stack)->data;

    free(stack);
    release_all(expht);
    return n;
}