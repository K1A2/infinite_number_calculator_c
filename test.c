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
    Test a;
    a.a = 10;

    printf("%p\n", &a);
    printf("%p\n", a);
    t(&a);

    printf("%d\n", a.a);

    return 0;
}
