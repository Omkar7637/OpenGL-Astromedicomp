#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    // variable declarations
    int iArray[NUM_ELEMENTS];
    int i, num, j, count = 0;

    // code
    printf("\n\n");

    // *** ARRAY ELEMENTS INPUT ***
    printf("Enter Integer Elements For Array : \n\n");
    for(i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &num);

        // If 'num' is negative ( < 0), then convert it to positive (multiply by -1)
        if(num < 0)
        {
            num = -1 * num;
        }

        iArray[i] = num;
    }

    // *** PRINTING ENTIRE ARRAY ***
    printf("\n\n");
    printf("Array Elements Are : \n\n");
    for(i = 0; i < NUM_ELEMENTS; i++)
    {
        printf("%d\n", iArray[i]);
    }

    // *** SEPARATING OUT EVEN NUMBERS FROM ARRAY ELEMENTS ***
    printf("\n\n");
    printf("Prime Numbers Amongst The Array Elements Are : \n\n");
    for(i = 0; i < NUM_ELEMENTS; i++)
    {
        for(j = 1; j <= iArray[i]; j++)
        {
            if((iArray[i] % j) == 0)
                count++;
        }

        if(count == 2)
        {
            printf("%d\n", iArray[i]);
        }
        count = 0; // reset count to 0 for checking Next Number...
    }

    return(0);
}