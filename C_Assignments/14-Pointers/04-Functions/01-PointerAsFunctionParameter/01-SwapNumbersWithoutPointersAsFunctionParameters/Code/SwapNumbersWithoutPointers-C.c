#include<stdio.h>

int main(void)
{
    // function declarations
    void SwapNumbers(int, int);

    // variable declarations
    int a;
    int b;

    // code
    printf("\n\n");
    printf("Enter Value For 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Value For 'B' : ");
    scanf("%d", &b);

    printf("\n\n");
    printf("****** BEFORE SWAPPING ******\n\n");
    printf("Value Of 'A' = %d\n\n", a);
    printf("Value Of 'B' = %d\n\n", b);

    SwapNumbers(a, b);

    printf("\n\n");
    printf("****** AFTER SWAPPING ******\n\n");
    printf("Value Of 'A' = %d\n\n", a);
    printf("Value Of 'B' = %d\n\n", b);

    return(0);
}

void SwapNumbers(int x, int y)
{
    // varible declarations
    int temp;

    // code
    printf("\n\n");
    printf("****** BEFORE SWAPPING ******\n\n");
    printf("Value Of 'X' = %d\n\n", x);
    printf("Value Of 'Y' = %d\n\n", y);

    temp = x;
    x = y;
    y = temp;

    printf("\n\n");
    printf("****** AFTER SWAPPING ******\n\n");
    printf("Value Of 'X' = %d\n\n", x);
    printf("Value Of 'Y' = %d\n\n", y);

}