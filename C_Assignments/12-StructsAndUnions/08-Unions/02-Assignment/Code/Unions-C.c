#include<stdio.h>

union MyUnion
{
    int i;
    float f;
    double d;
    char c;
};

int main(void)
{
    // variable decalarations
    union MyUnion u1, u2;

    //code
    // *** MyUnion u1 ***
    printf("\n\n");
    printf("Members Of Union u1 Are : \n\n");

    u1.i = 6;
    u1.f = 1.2f;
    u1.d = 8.333333;
    u1.c = 'S';

    printf("u1.i = %d\n\n", u1.i);
    printf("u1.f = %f\n\n", u1.f);
    printf("u1.d = %lf\n\n", u1.d);
    printf("u1.c = %c\n\n", u1.c);

    printf("Addresses Of Members Of Union u1 Are : \n\n");
    printf("u1.i = %p\n\n", &u1.i);
    printf("u1.f = %p\n\n", &u1.f);
    printf("u1.d = %p\n\n", &u1.d);
    printf("u1.c = %p\n\n", &u1.c);

    printf("MyUnion u1 = %p\n\n", &u1);
 
    // *** MyUnion u2 ***
    printf("\n\n");
    printf("Members Of Union u2 Are : \n\n");

    u2.i = 3;
    printf("u2.i = %d\n\n", u2.i);

    u2.f = 4.5f;
    printf("u2.f = %f\n\n", u2.f);

    u2.d = 5.12764;
    printf("u2.d = %lf\n\n", u2.d);

    u2.c = 'A';
    printf("u2.c = %c\n\n", u2.c);


    printf("Addresses Of Members Of Union u2 Are : \n\n");
    printf("u2.i = %p\n\n", &u2.i);
    printf("u2.f = %p\n\n", &u2.f);
    printf("u2.d = %p\n\n", &u2.d);
    printf("u2.c = %p\n\n", &u2.c);

    printf("MyUnion u2 = %p\n\n", &u2);

    return(0);
}