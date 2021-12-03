#include <stdlib.h>

typedef struct Digits {
    char data;
    struct Digits *next;
    struct Digits *prev;
} Digits;

typedef struct {
    Digits *up_decimal_point_head;
    Digits *down_decimal_point_head;
    Digits *up_decimal_point_tail;
    Digits *down_decimal_point_tail;
} Number;

Number* init_number();
void release_all(Number *number);
void insert_tail(char value, Digits* node);
void print_nodes_from_head(Digits* node, Digits* to_node);
void insert_head(char value, Digits* node);