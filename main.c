#include <stdio.h>
#include "include/input.h"

int main(void) {
    char strs[2][20] = {"input", "input_error1"};
    for (int i = 0;i < 2;i++) {
        ExpressHeadTail *exp = read_and_anlyze(strs[i]);
    }
    return 0;
}