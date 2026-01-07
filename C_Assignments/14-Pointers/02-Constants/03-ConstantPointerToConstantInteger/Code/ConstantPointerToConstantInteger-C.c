#include<stdio.h>

int main(void)
{
    int num = 5;
    const int* const ptr = &num;

    printf("\n");
    printf("Current Value Of 'num' = %d\n", num);
    printf("Current 'ptr' (Address of 'num') = %p\n", ptr);

    num++;
    printf("\n\n");
    printf("After num++, value of 'num' = %d\n", num);

    return(0);

}