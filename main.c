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
            printf("\n입력된 식\n");
            print_all(&exp); // 변환 된 식을 출력
            putchar('\n');
            exp = infix_to_postfix(&exp); // 중위식 후위식으로 변환
            if (exp != NULL) {
                printf("\n중위식으로 변환된 식\n");
                print_all(&exp); // 후위식으로 변환된 식 출력
                Number *number = calculation(&exp); // 후위식 계산
                if (number != NULL) {
                    // 계산에 오류가 없다면 계산 결과 출력
                    printf("\n답\n");
                    print_numbers(number);
                    putchar('\n');
                    release_numbers(&number);
                } else {
                    printf("\n식을 계산하는 과정에서 오류가 발생했습니다.\n");
                }
            } else {
                printf("\n후위연산식으로 변환하는 과정에서 오류가 발생했습니다.\n");
            }
        } else {
            printf("\n파일을 읽어오는 과정에서 오류가 발생했습니다.\n");
        }
    }
    return 0;
}