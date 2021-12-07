typedef enum {ERROR, OK} ErrorOk;
typedef enum {
    ERROR_FILE_IO,
    ERROR_INVALID_CHARACTER,
    ERROR_TOO_MANY_DECIMAL_POINT,
    ERROR_BRACKERS_COUNT_NOT_SAME,
    ERROR_TOO_MANY_OPERATOR,
    ERROR_TOO_MANY_OPERATOR_BEFORE_BRACKET,
    ERROR_OPERATOR_WRONG_ORDER,
    ERROR_OPERATOR_FIRST_ERROR,
    ERROR_RIGHT_BRACKET_WRONG_POSITION,
    ERROR_RIGHT_BRACKET_AFTER_OPERATOR,
    ERROR_MUTIPLE_OPERATOR_WRONG_POSITION,
    ERROR_NOTHING_IN_BRACKETS,
    ERROR_RIGHT_BRACKET_FRIST,
    ERROR_CALCULATION_ERROR
} ERROR_TYPE;

void alert_error(ERROR_TYPE type);
ErrorOk detect_error_invalid_chracter(char ch);