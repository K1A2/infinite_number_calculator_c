#include <stdio.h>
#include <stdbool.h>

#include "include/express/number.h"
#include "include/error.h"
#include "include/input.h"
#include "include/express/stack.h"
#include "include/calculator.h"

int main(void) {
    char strs[1][50] = {"input"};
    for (int i = 0;i < 1;i++) {
        ExpressHeadTail *exp = read_and_anlyze(strs[i]); // 파일에서 식 읽어온 후 구조체로 변환 후 반환
        if (exp != NULL) { // *exp가 null이면 식에 오류가 있음을 뜻함
            print_all(&exp); // 변환 된 식을 출력
            exp = infix_to_postfix(&exp); // 중위식 후위식으로 변환
            print_all(&exp);
            Number *number = calculation(&exp);
           if (number != NULL) {
               printf("\nAnswer: ");
                print_numbers(number);
                putchar('\n');
           }
            // putchar('\n');
            // if (exp != NULL) {
            //     // 후위식 변환 과정에서 문제가 없었다면
            //     print_all(exp);
            //     Number *numbers = calculation(exp);
            //     putchar('\n');
            //     if (!numbers->isPositive) {
            //         putchar('-');
            //     }
            //     print_nodes_from_head(numbers->up_decimal_point_head, numbers->up_decimal_point_tail);
            //     putchar('.');
            //     print_nodes_from_head(numbers->down_decimal_point_head, numbers->down_decimal_point_tail);
            //     putchar('\n');
            //     release_all(exp);
            // }
        }
    }
    return 0;
}