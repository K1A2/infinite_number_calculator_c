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
    bool isPositive;
    Digits *up_decimal_point_head;
    Digits *down_decimal_point_head;
    Digits *up_decimal_point_tail;
    Digits *down_decimal_point_tail;
} Number;

typedef struct Expression {
    Number data;
    char opr;
    EXPRESSION_TYPE type;
    struct Expression *next;
    struct Expression *prev;
} Expression;

typedef struct {
    Expression *head;
    Expression *tail;
} ExpressHeadTail;

int get_count_digits(Digits* node, Digits* tail);

Number init_number();
void insert_tail(char value, Digits* node);
void print_nodes_from_head(Digits* node, Digits* to_node);
void insert_head(char value, Digits* node);
void release_numbers(Number number);

ExpressHeadTail init_expression();
void expression_insert_tail(EXPRESSION_TYPE type, Number data, char opr, Expression* node);

void release_all(ExpressHeadTail exp);
void print_all(ExpressHeadTail expHT);