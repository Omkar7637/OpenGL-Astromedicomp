#include<stdio.h>

#define NUM_ELEMNETS 10

int main(void)
{
    // variable declarations 
    int iArray[NUM_ELEMNETS];
    int i, num, sum = 0;

    // code
    printf("\n\n");
    printf("ENter Integer Elements For Array : \n\n");
    for(i = 0; i < NUM_ELEMNETS; i++)
    {
        scanf("%d", &num);
        iArray[i] = num;
    }

    for(i = 0; i <  NUM_ELEMNETS; i++)
    {
        sum = sum + iArray[i];
    }

    printf("\n\n");
    printf("Sum Of ALL Elements Of Array = %d\n\n", sum);
    return (0);

}