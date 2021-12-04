#include <stdio.h>

#include "include/express/number.h"
#include "include/error.h"
#include "include/input.h"

int main(void) {
    char strs[2][20] = {"input", "input_error1"};
    for (int i = 0;i < 2;i++) {
        ExpressHeadTail *exp = read_and_anlyze(strs[i]);
        // print_all(*exp);
    }
    return 0;
}