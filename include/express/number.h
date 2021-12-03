#include <stdlib.h>

typedef enum {
    TYPE_DIGIT,
    TYPE_OPR
} EXPRESSION_TYPE;

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

typedef struct Expression {
    Number *data;
    char opr;
    EXPRESSION_TYPE type;
    struct Expression *next;
    struct Expression *prev;
} Expression;

typedef struct {
    Expression *head;
    Expression *tail;
} ExpressHeadTail;

Number* init_number();
void release_all(Number *number);
void insert_tail(char value, Digits* node);
void print_nodes_from_head(Digits* node, Digits* to_node);
void insert_head(char value, Digits* node);

ExpressHeadTail* init_expression();
void expression_insert_tail(EXPRESSION_TYPE type, Number *data, char opr, Expression* node);