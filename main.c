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
            putchar('\n');
            exp = infix_to_postfix(&exp); // 중위식 후위식으로 변환
            print_all(&exp); // 후위식으로 변환된 식 출력
            Number *number = calculation(&exp); // 후위식 계산
           if (number != NULL) {
               // 계산에 오류가 없다면 계산 결과 출력
               printf("\nAnswer: ");
               print_numbers(number);
               putchar('\n');
               release_numbers(&number);
           }
        }
    }
    return 0;
}