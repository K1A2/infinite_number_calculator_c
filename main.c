#include <stdio.h>
#include <stdbool.h>

#include "include/express/number.h"
#include "include/error.h"
#include "include/input.h"

int main(void) {
    char strs[1][50] = {"input"};
    for (int i = 0;i < 1;i++) {
        ExpressHeadTail *exp = read_and_anlyze(strs[i]);
        if (exp != NULL) {
            printf("succes\n");
            print_all(*exp);
            detect_error_all_cases(*exp);
        }
    }
    return 0;
}