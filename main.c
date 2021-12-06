#include <stdio.h>
#include <stdbool.h>

#include "include/express/number.h"
#include "include/error.h"
#include "include/input.h"
#include "include/express/stack.h"

int main(void) {
    char strs[1][50] = {"input"};
    for (int i = 0;i < 1;i++) {
        ExpressHeadTail *exp = read_and_anlyze(strs[i]); // 파일에서 식 읽어온 후 구조체로 변환 후 반환
        if (exp != NULL) { // *exp가 null이면 식에 오류가 있음을 뜻함
            print_all(*exp); // 변환 된 식을 출력
            detect_error_all_cases(*exp); // 후위 연산으로 변환하며 2차 검사를 함
        }
    }
    return 0;
}