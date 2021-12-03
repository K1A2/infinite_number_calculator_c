#include "input.h"

void readFile() {
    char in_ch;
    FILE *fp = fopen("input", "r");
    if (fp == NULL) {
        printf("file open error\n");
    } else {
        while ((in_ch = fgetc(fp)) != EOF) {
            printf("%c", in_ch);
        }
        putchar('\n');
    }
    fclose(fp);
}