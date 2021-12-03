#include "input.h"

void detect_error(char ch) {
    if ('0' <= ch && '9' >= ch) {

    } else {
        printf("error\n");
    }
}

void read_and_anlyze(char *filename) {
    char in_ch;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("file open error\n");
    } else {
        while ((in_ch = fgetc(fp)) != EOF) {
            printf("%c", in_ch);
            detect_error(in_ch);
        }
        putchar('\n');
    }
    fclose(fp);
}