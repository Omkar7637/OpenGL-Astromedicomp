#include<stdio.h>
#include<stdlib.h> // mmalloc() free()

int main(void)
{
    // variable declarations
    int *ptr_iArray = NULL;
    unsigned int intArrayLength = 0;
    int i;

    // code
    printf("\n\n");
    printf("Enter The Number Of Eements You Want In your Intger Array : ");
    scanf("%d", &intArrayLength);

    ptr_iArray = (int *)malloc(sizeof(int) * intArrayLength);
    if(ptr_iArray == NULL)
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR INTEGER ARRAY HAS FAILED !!! EXITING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR INTGER ARRAY HAS SUCCEEDED !!!\n\n");
        printf("MEMORY ADDRESSES FROM %p TO %p HAVE BEEN ALLOCATED TO INTGER ARRAY !!! \n\n", ptr_iArray, (ptr_iArray + (intArrayLength - 1)));
        
    }

    printf("\n\n");
    printf("Enter %d Elements For The Intger Array : \n\n", intArrayLength);
    for(i = 0; i < intArrayLength; i++)
    {
        scanf("%d", (ptr_iArray + i));
    }

    printf("\n\n");
    printf("The intger Array Entrerd By you, Consisting of %d Elemnets : \n\n", intArrayLength);

    for(i = 0; i < intArrayLength; i++)
    {
        printf("ptr_iArray[%d] = %d \t \t At Address &ptr_iArray[%d] : %p\n", i, ptr_iArray[i], i, &ptr_iArray[i]);
    }

    printf("\n\n");
    for(i = 0; i < intArrayLength; i++)
    {
        printf("*(ptr_iArray + %d) = %d \t \t At Address (ptr_iArray + %d) : %p \n", i, *(ptr_iArray + i), i, (ptr_iArray + i));
    }

    if(ptr_iArray)
    {
        free(ptr_iArray);
        ptr_iArray = NULL;

        printf("\n\n");
        printf("MEOMERY ALLOCATED FOR INTGER ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    return(0);
}