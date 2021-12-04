typedef enum {ERROR, OK} ErrorOk;
typedef enum {
    ERROR_FILE_IO,
    ERROR_INVALID_CHARACTER
} ERROR_TYPE;

void alert_error(ERROR_TYPE type);
ErrorOk detect_error_invalid_chracter(char ch);
ErrorOk detect_error_all_cases(ExpressHeadTail expht);