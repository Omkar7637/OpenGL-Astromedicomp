#include<stdio.h>

int main(void)
{
    // variable declarations
    int num = 5;
    int* const ptr = &num;

    printf("\n");
    printf("Current Value Of 'num' = %d\n", num);
    printf("Current 'ptr' (Address of 'num') = %p\n", ptr);

    num++;
    printf("\n\n");
    printf("After num++, value of 'num' = %d\n", num);

    (*ptr)++;

    printf("After (*ptr)++, value of 'ptr' = %p\n", ptr);
    printf("Value at this 'ptr' = %d\n", *ptr);
    printf("\n");
    return(0);
}