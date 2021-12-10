typedef enum {
    TYPE_DIGIT,
    TYPE_OPR
} EXPRESSION_TYPE;

typedef struct Digits {
    unsigned char data;
    struct Digits *next;
    struct Digits *prev;
} Digits;

typedef struct {
    bool isPositive;
    Digits *head;
    Digits *tail;
    unsigned int deciaml_point;
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

int get_count_digits(Digits* node, Digits* tail);
int deletee_zero_up_deciaml(Digits* head, Digits* tail);
int deletee_zero_down_deciaml(Digits* head, Digits* tail);

Number *init_number();
void digit_insert_head(char value, Digits* node);
void print_nodes_from_head(Digits* node, Digits* to_node);
void digit_insert_tail(char value, Digits* node);
void release_numbers(Number **number);
void release_numbers_concatenated(Number *number);

ExpressHeadTail *init_expression();
Expression *init_expression_empty_noode_number();
void number_concatenate(Expression *exp);
void expression_insert_tail(Expression *newNode, Expression* node);
void expression_insert_tail_new_node(EXPRESSION_TYPE type, Number *data, char opr, Expression* node);

void release_all(ExpressHeadTail **exp);
void print_all(ExpressHeadTail **expHT);
void print_numbers(Number *numbers);