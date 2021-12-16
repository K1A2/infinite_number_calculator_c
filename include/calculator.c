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

void make_up_place_count_same(Number **a, Number **b) {
    unsigned int a_total_count = get_count_digits((*a)->head, (*a)->tail), b_total_count = get_count_digits((*b)->head, (*b)->tail);
    if (a_total_count >= b_total_count) {
        for (int i = 0;i < a_total_count - b_total_count;i++) {
            digit_insert_head('0', (*b)->head);
        }
    } else {
        for (int i = 0;i < b_total_count - a_total_count;i++) {
            digit_insert_head('0', (*a)->head);
        }
    }
}

Expression *addition(Expression **a, Expression **b) {
    if ((*a)->data->isPositive != (*b)->data->isPositive) {
        if (!(*b)->data->isPositive) {
            (*b)->data->isPositive = true;
        } else {
            (*a)->data->isPositive = true;
            Expression *tmp = *a;
            *a = *b;
            *b = tmp;
        }
        return subtraction(a, b);
    } else {
        Expression *result = init_expression_empty_noode_number();

        make_down_place_count_same(&((*a)->data), &((*b)->data), &(result->data));
        make_up_place_count_same(&((*a)->data), &((*b)->data));

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
    if ((*a)->data->isPositive && !(*b)->data->isPositive) { // a = +, b = -
        (*b)->data->isPositive = true;
        return addition(a, b);
    } else if (!(*a)->data->isPositive && (*b)->data->isPositive) { // a = -, b = +
        (*b)->data->isPositive = false;
        return addition(a, b);
    }
    Expression *result = init_expression_empty_noode_number();

    make_down_place_count_same(&((*a)->data), &((*b)->data), &(result->data));

    bool is_swap = false;

    unsigned int a_total_count = get_count_digits((*a)->data->head, (*a)->data->tail), b_total_count = get_count_digits((*b)->data->head, (*b)->data->tail);
    unsigned int a_up_place_count = a_total_count - (*a)->data->deciaml_point, b_up_place_count = b_total_count - (*b)->data->deciaml_point;
    if (a_up_place_count == b_up_place_count) {
        Digits *a_now = (*a)->data->head->next, *b_now = (*b)->data->head->next;
        bool is_same = true;
        while (a_now != (*a)->data->tail) {
            if (a_now->data > b_now->data) {
                is_same = false;
                break;
            } else if (a_now->data < b_now->data) {
                is_same = false;
                is_swap = true;
                Expression *tmp = *a;
                *a = *b;
                *b = tmp;
                break;
            }

            a_now = a_now->next;
            b_now = b_now->next;
        }

        if (is_same) {
            release_numbers(&((*a)->data));
            release_numbers(&((*b)->data));
            free(*a);
            free(*b);

            result->data->deciaml_point = 0;
            return result;
        }
    } else {
        if (a_up_place_count < b_up_place_count) {
            Expression *tmp = *a;
            *a = *b;
            *b = tmp;
            is_swap = true;
        }
    }

    make_up_place_count_same(&((*a)->data), &((*b)->data));

    Digits *a_now = (*a)->data->tail->prev, *b_now = (*b)->data->tail->prev;
    bool is_down = false;
    while (b_now != (*b)->data->head) {
        unsigned int a_num = a_now->data - '0', b_num = b_now->data - '0';
        if (is_down) {
            if (a_num == 0) {
                a_num += 9;
            } else {
                a_num -= 1;
                is_down = false;
            }
        }
        if (a_num < b_num) {
            is_down = true;
            a_num += 10;
        }
        digit_insert_head(a_num - b_num + '0', result->data->head);

        a_now = a_now->prev;
        b_now = b_now->prev;
    }

    result->data->deciaml_point = (*a)->data->deciaml_point;
    result->data->deciaml_point -= deletee_zero_down_deciaml(result->data->head, result->data->tail, result->data->deciaml_point);
    deletee_zero_up_deciaml(result->data->head, result->data->tail);

    if (is_swap) {
        if ((*a)->data->isPositive) {
            result->data->isPositive = false;
        } else {
            result->data->isPositive = true;
        }
    } else {
        if ((*a)->data->isPositive) {
            result->data->isPositive = true;
        } else {
            result->data->isPositive = false;
        }
    }

    release_numbers(&((*a)->data));
    release_numbers(&((*b)->data));
    free(*a);
    free(*b);

    return result;
}

Expression *multiplication(Expression **a, Expression **b) {
    Expression *result = init_expression_empty_noode_number();

    unsigned int a_total_count = get_count_digits((*a)->data->head, (*a)->data->tail), b_total_count = get_count_digits((*b)->data->head, (*b)->data->tail);
    if (a_total_count < b_total_count) {
        Expression *tmp = *a;
        *a = *b;
        *b = tmp;
    }

    Digits *now_b = (*b)->data->tail->prev;

    int ten = 0;
    while (now_b != (*b)->data->head) {
        unsigned int carry = 0;
        Digits *now_a = (*a)->data->tail->prev;
        Expression *tmp = init_expression_empty_noode_number();
        tmp->data->deciaml_point = 0;

        while (now_a != (*a)->data->head) {
            unsigned int num_res = (now_a->data - '0') * (now_b->data - '0');
            num_res += carry;
            carry = num_res / 10;
            num_res %= 10;
            digit_insert_head(num_res + '0', tmp->data->head);
            now_a = now_a->prev;
        }

        if (carry != 0) {
            digit_insert_head(carry + '0', tmp->data->head);
        }

        for (int i = 0;i < ten;i++) {
            digit_insert_tail('0', tmp->data->tail);
        }

        result = addition(&result, &tmp);
        ten++;
        now_b = now_b->prev;
    }

    result->data->deciaml_point = (*a)->data->deciaml_point + (*b)->data->deciaml_point;
    result->data->deciaml_point -= deletee_zero_down_deciaml(result->data->head, result->data->tail, result->data->deciaml_point);

    if ((*a)->data->isPositive == (*b)->data->isPositive) {
        result->data->isPositive = true;
    } else {
        result->data->isPositive = false;
    }

    release_numbers(&((*a)->data));
    release_numbers(&((*b)->data));
    free(*a);
    free(*b);

    return result;
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
            Expression *two = pop(stack), *one = pop(stack), *result = NULL;
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