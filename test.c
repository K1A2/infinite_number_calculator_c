#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
} Test;

void v(Test *b) {
    printf("%p\n", b);
    printf("%p\n", *b);
    b->a = 20;
}

void t(Test *b) {
    printf("%p\n", b);
    printf("%p\n", *b);
    v(b);
}

int main(int argc, char const *argv[])
{
    // Test a;
    // a.a = 10;

    // printf("%p\n", &a);
    // printf("%p\n", a);
    // t(&a);

    // printf("%d\n", a.a);




    // for (char b = '(';b <= '9';b++) {
    //     printf("%c\n", b);
    //     unsigned char a = b, p = 0b10000000;
    //     printf("%c\n", a);

    //     for (int i = 0;i < 8;i++) {
    //         if ((a & p) == p) {
    //             printf("1");
    //             // printf("%d\n", a & p);
    //         } else {
    //             printf("0");
    //         }
    //         p >>= 1;
    //     }
    //     putchar('\n');
    // }


    for (char a = '0';a <= '9';a++) {
        unsigned char c = a;
        unsigned int b = a - '0';
        printf("%c %c %d %c\n", c, a, b, (char)b);
    }

    return 0;
}
