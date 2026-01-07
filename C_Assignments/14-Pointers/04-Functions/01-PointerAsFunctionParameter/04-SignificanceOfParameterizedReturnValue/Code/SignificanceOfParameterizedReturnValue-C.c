#include<stdio.h>

enum
{
    NEGATIVE = -1,
    ZERO, 
    POSITIVE
};

int main(void)
{
    // function declarations
    int Difference(int, int, int *);

    // variable declarations
    int a;
    int b;
    int answer, ret;

    // code
    printf("\n\n");
    printf("Enter Value Of 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Value Of 'B' : ");
    scanf("%d", &b);

    ret = Difference(a, b, &answer);

    printf("Difference Of %d And %d = %d\n\n", a, b, answer);

    if(ret == POSITIVE)
    {
        printf("Difference Of %d And %d Is Positive !!!\n\n", a, b);
    }
    else if(ret == NEGATIVE)
    {
        printf("Difference Of %d And %d Is Negative !!!\n\n", a, b);
    }
    else
    {
        printf("Difference Of %d And %d Is Zero !!!\n\n", a, b);
    }
    
    return(0);
}

int Difference(int x, int y, int *diff)
{
    // code
    *diff = x - y;

    if(*diff > 0)
    {
        return(POSITIVE);
    }
    else if(*diff < 0)
    {
        return(NEGATIVE);
    }
    else
    {
        return(ZERO);
    }
}