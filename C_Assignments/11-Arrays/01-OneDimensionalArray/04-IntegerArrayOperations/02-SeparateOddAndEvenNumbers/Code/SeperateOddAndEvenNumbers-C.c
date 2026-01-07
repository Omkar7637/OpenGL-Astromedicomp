#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    // variable decalarations
    int iArray[NUM_ELEMENTS];
    int i, num, sum = 0;

    // code
    printf("\n\n");

    // *** ARRAY ELEMENTS INPUT ***
    printf("Enter Integer Elements For Array : \n\n");
    for(i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &num);
        iArray[i] = num;
    }

    // *** SEPARATING OUT EVEN FROM ARRAY ELEMENTS ***
    printf("\n\n");
    printf("Even Numbers Amongst The Array Elements Are : \n\n");
    for(i = 0; i < NUM_ELEMENTS; i++)
    {
        if((iArray[i] % 2) == 0)
        {
            printf("%d\n", iArray[i]);
        }
    }

    return (0); 
}