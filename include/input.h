#include <stdio.h>
#include "error.h"
#include "express/number.h"

ExpressHeadTail* read_and_anlyze(char *filename);
ErrorOk detect_error(char ch);
int is_digit(char ch);