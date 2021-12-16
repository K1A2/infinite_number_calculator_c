#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "express/number.h"
#include "express/stack.h"
#include "error.h"
#include "calculator.h"

Expression *addition(Expression *a, Expression *b) {
    
    int down_a = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail); //숫자의 자리 수 개수를 구하는 함수
    printf("down_a : %c \n", a->data);
    int down_b = get_count_digits(b->data->down_decimal_point_head, b->data->down_decimal_point_tail);
    printf("down_b : %c \n", b->data);
    int up_a = get_count_digits(a->data->up_decimal_point_head, a->data->up_decimal_point_tail);
    int up_b = get_count_digits(b->data->up_decimal_point_head, b->data->up_decimal_point_tail);
    

    Expression *expression = init_expression_empty_noode_number();

    if(down_a <=down_b)
    {
        while (down_a != down_b)
        {
            digit_insert_tail('0', a->data->down_decimal_point_tail);
            down_a = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail);
            
        }
    }else if(down_a >=down_b)
    {
        while (down_a != down_b)
        {
            digit_insert_tail('0', b->data->down_decimal_point_tail);
            down_b = get_count_digits(b->data->down_decimal_point_head, b->data->down_decimal_point_tail);

        }
    }
    
    if(up_a <= up_b)
    {
        while (up_a != up_b)
        {
            digit_insert_head('0', a->data->up_decimal_point_head);
            up_a = get_count_digits(a->data->up_decimal_point_head, a->data->up_decimal_point_tail);
        }
    }else if(up_a >= up_b)
    {
        while (up_a != up_b)
        {
            digit_insert_head('0', b->data->up_decimal_point_head);
            up_b = get_count_digits(b->data->up_decimal_point_head, b->data->up_decimal_point_tail);
        }
    }
    
    int quotient = 0, remainder = 0; //quotient=올림숫자, remainder = 나머지 

    // 1~3 구현 ,어디선가 Segmentation fault: 11 오류남


 
    for(int i = 1; i <= down_a; i++){
        
        int a_d = num(a->data->down_decimal_point_tail) - '0';
        int b_d = num(b->data->down_decimal_point_tail) - '0';
        int add = a_d + b_d + quotient;

        quotient = add/10;
        remainder = add%10;
        //나머지


        printf("i: %d\n add: %d \n q: %d\n r: %d\n\n",i, add,quotient, remainder);
        digit_insert_head(remainder+'0', expression->data->down_decimal_point_head);

    }
    

   for(int i = 0; i < down_a; i++){
        
        int a_d = num(a->data->up_decimal_point_tail) - '0';
        int b_d = num(b->data->up_decimal_point_tail) - '0';
        
        int add = a_d + b_d + quotient;
        
        quotient = add/10;
        remainder = add%10;

        digit_insert_head(remainder+'0', expression->data->up_decimal_point_head);

    }
    
    release_numbers(a->data);
    release_numbers(b->data);
    release_numbers(expression->data);
    return expression;

    
    return 0;
}


Expression *subtraction(Expression *a, Expression *b) {
     return 0;
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