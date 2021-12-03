#include <stdio.h>
#include "include/input.h"
#include "include/express/number.h"

int main(void) {
    char strs[2][20] = {"input", "input_error1"};
    for (int i = 0;i < 2;i++) {
        readFile(strs[i]);
    }
    
    Number number = *init_number();
    insert_head('1', number.up_decimal_point_head);
    insert_head('2', number.up_decimal_point_head);
    insert_head('3', number.up_decimal_point_head);
    print_nodes_from_head(number.up_decimal_point_head, number.up_decimal_point_tail);
    release_all(&number);
    return 0;
}