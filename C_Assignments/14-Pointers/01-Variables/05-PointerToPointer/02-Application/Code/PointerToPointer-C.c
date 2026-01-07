#include<stdio.h>

int main(void)
{
    // variable decalartions
    int num;
    int *ptr = NULL;
    int **pptr = NULL;

    // code
    num = 10;
    printf("\n\n");

    printf(" ****** BEFORE ptr = &num ******\n\n");
    printf("Value Of 'num'              = %d\n\n", num);
    printf("Address Of 'num'            = %p\n\n", &num);
    printf("Value At Address Of 'num'   = %p\n\n", (&num));

    ptr = &num;

    printf("\n\n");

    printf(" ****** AFETR ptr = &num ******\n\n");
    printf("Value Of 'num'              = %d\n\n", num);
    printf("Address Of 'num'            = %p\n\n", &num);
    printf("Value At Address Of 'num'   = %p\n\n", (&num));

    pptr = &ptr;

    printf("\n\n");

    printf(" ****** AFETR pptr = &ptr ******\n\n");
    printf("Value Of 'num'                               = %d\n\n", num);
    printf("Address Of 'num' (ptr)                       = %p\n\n", ptr);
    printf("Address Of 'ptr' (pptr)                      = %p\n\n", pptr);
    printf("Value At Address Of 'ptr' (*pptr)            = %p\n\n", *pptr);
    printf("Value At Address Of 'num' (*ptr) (*pptr)     = %d\n\n", **pptr);




}