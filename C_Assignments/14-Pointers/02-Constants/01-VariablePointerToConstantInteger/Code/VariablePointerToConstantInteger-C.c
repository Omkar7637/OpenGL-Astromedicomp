#include<stdio.h>

int main(void)
{
    // variable declarations
    int num = 5;
    const int* ptr = NULL;

    // code
    ptr = &num;
    printf("\n");
    printf("Curent Value Of 'num' = %d\n", num);
    printf("Current 'ptr' (Address of 'num') = %p\n", ptr);

    num++;
    printf("\n\n");
    printf("After num++, value of 'num' = %d\n", num);

    ptr++;

    printf("\n\n");
    printf("After ptr++, Value of 'ptr' = %p\n", ptr);
    printf("Value at this new 'ptr' = %d\n", *ptr);
    printf("\n");
    return(0);

}